/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

//
// Network topology
//
//           10Mb/s, 10ms       10Mb/s, 10ms
//       (0_0)          (1_0)  (1_1)         (2_0)
//       n0-----------------n1-----------------n2
//	(Ue)		  (Enb, SPGW) 	   (end-host)	
// 	10.1.3.1:3000	10.1.3.2/10.1.2.1    10.1.2.2:49153
// - Tracing of queues and packet receptions to file 
//   "tcp-large-transfer.tr"
// - pcap traces also generated in the following files
//   "tcp-large-transfer-$n-$i.pcap" where n and i represent node and interface
// numbers respectively
//  Usage (e.g.): ./waf --run tcp-large-transfer


#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/config-store.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpLargeTransfer");

#define kilo 1000
#define KILO 1024
#define TCP_SAMPLING_INTERVAL 0.02 //tcp flow sampling interval in second
#define ONEBIL kilo*kilo*kilo

static double timer = 0;
static uint32_t sim_time = 100;
static uint32_t packet_size = 900;
static std::string sending_rate = "100Mb/s"; //sending rate.
static std::string core_network_bandwidth = "1000Mb/s"; 	//core_network_bandwidth.
static uint32_t core_network_delay = 2;	//core_network_delay in millisenconds.
static uint32_t core_network_mtu = 1500; 	//core_network_mte in Bytes.
static std::string init_radio_bandwidth = "1Mb/s"; 	//radio_link_bandwidth (init).
static uint32_t init_radio_delay = 5;	//radio_link_delay (init) in millisenconds.
static uint32_t init_radio_mtu = 1500; 	//radio_link_mtu (init) in Bytes.
static uint16_t is_tcp = 1;
static Ptr<ns3::FlowMonitor> monitor;
static FlowMonitorHelper flowHelper;
static Ptr<ns3::Ipv4FlowClassifier> classifier;
static std::map <FlowId, FlowMonitor::FlowStats> stats;
static Ipv4Address ue_ip;
static Ipv4Address endhost_ip;
static Ipv4Address enb_ip;

/**sending flowS stats***/
double meanTxRate_send;
double meanRxRate_send;
double meanTcpDelay_send;
uint64_t numOfLostPackets_send;
uint64_t numOfTxPacket_send;
double last_lost = 0;
/***acking flowS stats***/
double meanTxRate_ack;
double meanRxRate_ack;
double meanTcpDelay_ack;
uint64_t numOfLostPackets_ack;
uint64_t numOfTxPacket_ack;
double last_lost_ack = 0;


static void 
CwndTracer (uint32_t oldval, uint32_t newval)
{
  NS_LOG_UNCOND (Simulator::Now().GetMilliSeconds() << " cwnd_from " << oldval << " to " << newval);
}
static void enable_cwnd_trace(Ptr<Application> app);
static void
getTcpPut();


int main (int argc, char *argv[])
{
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //  LogComponentEnable("TcpL4Protocol", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpSocketImpl", LOG_LEVEL_ALL);
  //  LogComponentEnable("PacketSink", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpLargeTransfer", LOG_LEVEL_ALL);

  CommandLine cmd;
    cmd.AddValue("sim_time", "Total duration of the simulation [s])", sim_time);
    cmd.AddValue("packet_size", "Size of each packet", packet_size);
    cmd.AddValue("sending_rate", "Application sending rate", sending_rate);
    cmd.AddValue("core_network_bandwidth", "Core network Data Rate", core_network_bandwidth);
    cmd.AddValue("core_network_delay", "Core network Delay", core_network_delay);
    cmd.AddValue("core_network_mtu", "Core network MTU size", core_network_mtu);
    cmd.AddValue("is_tcp", "Transport protocol used", is_tcp);
 
    /**ConfigStore setting*/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("emulated-in.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Load"));
    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    inputConfig.ConfigureAttributes();
    

  cmd.Parse (argc, argv);

  NodeContainer n0n1;
  n0n1.Create (2);

  NodeContainer n1n2;
  n1n2.Add (n0n1.Get (1));
  n1n2.Create (1);

  Ptr<Node> remote_host = n1n2.Get(1);
  Ptr<Node> ue = n0n1.Get(0);
  Ptr<Node> enb = n1n2.Get(1); 

  // We create the channels first without any IP addressing information
  // First make and configure the helper, so that it will put the appropriate
  // attributes on the network interfaces and channels we are about to install.
  PointToPointHelper core_network_link;
  core_network_link.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (core_network_bandwidth)));
  core_network_link.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (core_network_delay)));
  core_network_link.SetDeviceAttribute ("Mtu", UintegerValue(core_network_mtu));
 
  PointToPointHelper radio_link;
  radio_link.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (init_radio_bandwidth)));
  radio_link.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (init_radio_delay)));
  radio_link.SetDeviceAttribute ("Mtu", UintegerValue(init_radio_mtu));


  // And then install devices and channels connecting our topology.
  NetDeviceContainer radio_dev = radio_link.Install (n0n1); 	//Radio link devices, n0-Ue, n1-Enb,SPGW
  NetDeviceContainer core_dev = core_network_link.Install (n1n2);		//Core network devices, n2-endhost

  // Now add ip/tcp stack to all nodes.
  InternetStackHelper internet;
  internet.InstallAll ();

  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer radio_interfs = ipv4.Assign (radio_dev); 
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer core_interfs = ipv4.Assign (core_dev);
  ue_ip = radio_interfs.GetAddress(0);
  endhost_ip = core_interfs.GetAddress(1);
  enb_ip = radio_interfs.GetAddress(1);
  // and setup ip routing tables to get total ip-level connectivity.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  //*****************************Install and start applications on UEs and remote host****************************//
    uint16_t ulPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

   if (is_tcp == 1){
        				/*********TCP Application********/
       					PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), ulPort));
       					serverApps.Add(sink.Install(ue));

        				OnOffHelper onOffHelper("ns3::TcpSocketFactory", Address ( InetSocketAddress(ue_ip, ulPort) ));
        				onOffHelper.SetConstantRate( DataRate(sending_rate), packet_size );
       					clientApps.Add(onOffHelper.Install(remote_host));
   }
                else{
        					/*********UDP Application********/
        				PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), ulPort));
       					serverApps.Add(sink.Install(ue));

        				OnOffHelper onOffHelper("ns3::UdpSocketFactory", Address ( InetSocketAddress( ue_ip, ulPort) ));
        				onOffHelper.SetConstantRate( DataRate(sending_rate), packet_size );
       					clientApps.Add(onOffHelper.Install(remote_host));
    }

  //===========Flow monitor==============//
  monitor = flowHelper.Install(ue);
  monitor = flowHelper.Install(remote_host);
  monitor = flowHelper.GetMonitor();




  /*******************Start client and server apps***************/
  serverApps.Start (Seconds (0.01));		//All server start at 0.01s.
  clientApps.Start (Seconds(0.5));

  Simulator::ScheduleWithContext (0 ,Seconds (0.0), &getTcpPut);
  Simulator::Schedule(Seconds(0.6) + NanoSeconds(1.0), &enable_cwnd_trace, remote_host->GetApplication(0));///*Note: enable_cwnd_trace must be scheduled after the OnOffApplication starts (OnOffApplication's socket is created after the application starts) 

    /****ConfigStore setting****/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("emulated-out.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Save"));
    ConfigStore outputConfig;
    outputConfig.ConfigureDefaults();
    outputConfig.ConfigureAttributes();



  Simulator::Stop (Seconds (sim_time));
  Simulator::Run ();

  monitor->CheckForLostPackets();
  Ptr<ns3::Ipv4FlowClassifier> classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  std::map <FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();


  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (t.destinationPort >= 3001 && t.destinationPort <= 4000) {
      if (iter->second.rxPackets > 1){
        meanTxRate_send = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/kilo;
        meanRxRate_send = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/kilo;
        meanTcpDelay_send = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_send = iter->second.lostPackets; 
      numOfTxPacket_send = iter->second.txPackets;
    }

    /*ack flow, from Ues (7.0.0.2:200x) to endhost (1.0.0.2:49153)*/
    if (t.destinationPort >= 49153){
      if (iter->second.rxPackets > 1){
        meanTxRate_ack = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_ack = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_ack = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_ack = iter->second.lostPackets; 
      numOfTxPacket_ack = iter->second.txPackets;
    }

    NS_LOG_UNCOND("***Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << "Port " << t.sourcePort << " Dst Addr " << t.destinationAddress << "destination port " << t.destinationPort);
    NS_LOG_UNCOND("Tx Packets " << iter->second.txPackets);
    NS_LOG_UNCOND("Rx Packets " << iter->second.rxPackets);
    NS_LOG_UNCOND("Lost packets " << iter->second.lostPackets);
    NS_LOG_UNCOND("Lost ratio " << double (iter->second.lostPackets)/(iter->second.lostPackets+iter->second.rxPackets));
    if (iter->second.rxPackets > 1){
        NS_LOG_UNCOND("Average delay received " << iter->second.delaySum/iter->second.rxPackets/1000000);
        NS_LOG_UNCOND("Mean received bitrate " << 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024));
        NS_LOG_UNCOND("Mean transmitted bitrate " << 8*iter->second.txBytes/(iter->second.timeLastTxPacket-iter->second.timeFirstTxPacket)*ONEBIL/(1024));
    }
  }
  std::cout << "ue = " << ue_ip << "end-host = " << endhost_ip << "enb = " << enb_ip << std::endl;
  Simulator::Destroy ();
}

static void enable_cwnd_trace(Ptr<Application> app)
{
    Ptr<OnOffApplication> on_off_app = app->GetObject<OnOffApplication>();
    if (on_off_app != NULL)
    {
        Ptr<Socket> socket = on_off_app->GetSocket();
        socket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndTracer));//, stream));
    }
}

static void
getTcpPut(){
    monitor->CheckForLostPackets();
    classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
    stats = monitor->GetFlowStats();

    /*==============Get flows information============*/
   for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (t.destinationPort >= 3000 && t.destinationPort <= 4000) {
      if (iter->second.rxPackets > 1){
        meanTxRate_send = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/kilo;
        meanRxRate_send = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/kilo;
        meanTcpDelay_send = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_send = iter->second.lostPackets;
      if (iter->second.lostPackets > last_lost){
	NS_LOG_UNCOND(Simulator::Now().GetMilliSeconds() << " Tcp lost= " << iter->second.lostPackets - last_lost);
	last_lost = iter->second.lostPackets;
	}
      numOfTxPacket_send = iter->second.txPackets;
    }

     /*ack flow, from Ues (7.0.0.2:200x) to endhost (1.0.0.2:49153)*/
    if (t.destinationPort >= 49153){
      if (iter->second.rxPackets > 1){
        meanTxRate_ack = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_ack = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_ack = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_ack = iter->second.lostPackets; 
      numOfTxPacket_ack = iter->second.txPackets;
      if (iter->second.lostPackets > last_lost_ack){
		NS_LOG_UNCOND(Simulator::Now().GetMilliSeconds() << " Tcp_ack lost= " << iter->second.lostPackets - last_lost_ack);
		last_lost_ack = iter->second.lostPackets;
      }
    }
   }
      NS_LOG_UNCOND (Simulator::Now().GetMilliSeconds() << "\t\t"
                  << ue_ip << "\t\t"
                  << meanRxRate_send << "\t\t"
                  << meanTcpDelay_send << "\t\t"
                  << numOfLostPackets_send << "\t\t"
                  << numOfTxPacket_send << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t"
		  << meanTxRate_send);
    while (timer < sim_time){
        timer += TCP_SAMPLING_INTERVAL;
        Simulator::Schedule(Seconds(timer),&getTcpPut);
    }
}

