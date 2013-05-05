

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 The university of Utah
 *
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
 * Author: Binh Nguyen <binh@cs.utah.edu>
 */


/**
 * NOTE: This code doesn't support MULTIPLE ENODEBs yet (problem might lay on the mobility model, co-location in position of EnodeBs).
 */
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/lte-global-pathloss-database.h"

//#include "ns3/gtk-config-store.h"

#include <map>

using namespace ns3;
/**
 * Topology:     Ues (x numberOfNodes) ---------- EnodeB ---------- |
 *																																	|
 * 							 Ues (x numberOfNodes) ---------- EnodeB ----------	|
 * 							 																										|
 * 							 Ues (x numberOfNodes) ---------- EnodeB ----------	|	SPGW -------------------------- RH
 *
 *                              										n2                 n0     <100Gbps, 0.01ms>      n1
 */
/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */
NS_LOG_COMPONENT_DEFINE ("e2e-tcp-dl");

double
CalculateAverageDelay(std::map <uint64_t, uint32_t> delayArray);

double
getAverage(std::list<double> array);
double
getUlRlcDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t, uint8_t);
double
getUlPdcpDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t, uint8_t);

double
getDlRlcDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t, uint8_t);
double
getDlPdcpDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t, uint8_t);

uint32_t
getUlRlcTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

uint32_t
getDlRlcTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

uint32_t
getUlRlcRxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

uint32_t
getDlRlcRxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

uint32_t
getUlPdcpTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

uint32_t
getDlPdcpTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid);

//static void
//CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd);


int
main (int argc, char *argv[])
{
	//*************Enable logs********************/
    //To enable all components inside the LTE module.
//      lteHelper->EnableLogComponents();
    
    //	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
    //	LogComponentEnable("UdpEchoClientApplication",LOG_PREFIX_ALL);
    	// LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    	// LogComponentEnable("UdpClient",LOG_LEVEL_INFO);
    	 // LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
//		LogComponentEnable("OnOffApplication",LOG_LEVEL_INFO);
//		LogComponentEnable("PacketSink",LOG_LEVEL_INFO);
//         LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);
//     		LogComponentEnable("OnOffApplication",level);
//     		LogComponentEnable("PacketSink",level);
//       LogComponentEnable ("LteRlcUm", level);
    //   LogComponentEnable ("LteHelper",level);
//       LogComponentEnable ("LteUeMac", level);
    //   LogComponentEnable ("LteEnbMac", level);
//       LogComponentEnable ("LtePdcp", level);
//       LogComponentEnable ("LtePhy", level);
    
    
	 //topology
    uint16_t numberOfUeNodes = 1;
    uint16_t numberOfEnodebs = 1;
    
    //S1uLink
    std::string s1uLinkDataRate = "1Gb/s";
    double	s1uLinkDelay = 0.01;
    uint16_t s1uLinkMtu	= 5000;
    
    //p2pLink
    std::string p2pLinkDataRate = "1Gb/s";
    double p2pLinkDelay = 0.01;
    uint16_t p2pLinkMtu = 5000;
    
    //Simulation
    uint32_t numberOfPackets = 0;
    uint32_t packetSize = 900;
    double simTime = 20;	//simulation time for EACH application
    double distance = 100.0;
    double interPacketInterval = 2000;	//in micro seconds, minimum 1.
    uint16_t radioUlBandwidth = 100;	//the radio link bandwidth among UEs and EnodeB (in Resource Blocks). This is the configuration on LteEnbDevice.
    uint16_t radioDlBandwidth = 100;	//same as above, for downlink.
    std::string pcapName = "e2e-tcp-dl";
    std::string dataRate = "1000kb/s";
    
    std::ofstream oFile;
    oFile.open("/Users/binh/Desktop/ns3_play/output-tcp-dl.txt", std::ios::app);
    uint16_t isAMRLC = 1;    
    uint16_t isTcp = 1;
    
    
    // Command line arguments
    CommandLine cmd;
    cmd.AddValue("numberOfUeNodes", "Number of UeNodes", numberOfUeNodes);
    cmd.AddValue("numberOfEnodebs", "Number of eNodebs", numberOfEnodebs);
    cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
    cmd.AddValue("distance", "Distance between eNBs [m]", distance);
    cmd.AddValue("interPacketInterval", "Inter packet interval [us])", interPacketInterval);
    cmd.AddValue("numberOfPackets", "Number of packets to send", numberOfPackets);
    cmd.AddValue("packetSize", "Size of each packet", packetSize);
    cmd.AddValue("s1uLinkDataRate", "S1u Link Data Rate", s1uLinkDataRate);
    cmd.AddValue("s1uLinkDelay", "S1u Link Delay", s1uLinkDelay);
    cmd.AddValue("s1uLinkMtu", "S1u Link Mtu", s1uLinkMtu);
    cmd.AddValue("p2pLinkDataRate", "p2p Link Data Rate", p2pLinkDataRate);
    cmd.AddValue("p2pLinkDelay", "p2p Link Delay", p2pLinkDelay);
    cmd.AddValue("p2pLinkMtu", "p2p Link Mtu", p2pLinkMtu);
    cmd.AddValue("radioUlBandwidth", "Uplink radio bandwidth [RBs] (6,15,25,50,75,100)", radioUlBandwidth);
    cmd.AddValue("radioDlBandwidth", "Downlink radio bandwidth [RBs] (6,15,25,50,75,100)", radioDlBandwidth);
    cmd.AddValue("pcapName", "Prefix of pcap files", pcapName);
    cmd.AddValue("isAMRLC", "Whether using AM RLC (UM RLC if false)", isAMRLC);
    cmd.AddValue("dataRate", "TCP application data rate", dataRate);
    cmd.AddValue("isTcp", "TCP application if true, Udp if false", isTcp);


    /**ConfigStore setting*/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("config-in-tcp-dl.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Load"));
    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    inputConfig.ConfigureAttributes();
    
    cmd.Parse(argc, argv);
    //*************************************************/
    
    
    //************lteHeper, epcHelper**************//
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
    Ptr<EpcHelper>  epcHelper = CreateObject<EpcHelper> ();
    lteHelper->SetEpcHelper (epcHelper);
    lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");
    StringValue RlcType;
    


    //*********************Use epcHelper to get the PGW node********************//
    Ptr<Node> pgw = epcHelper->GetPgwNode ();
    epcHelper->SetAttribute("S1uLinkDataRate", DataRateValue (DataRate (s1uLinkDataRate)));
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue (Seconds (s1uLinkDelay)));
    epcHelper->SetAttribute("S1uLinkMtu", UintegerValue (s1uLinkMtu));
    
    
    
    
    //***********Create a single RemoteHost, install the Internet stack on it*************//
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create (1);
    Ptr<Node> remoteHost = remoteHostContainer.Get (0);
    //Install Internet stack on the remoteHost.
    InternetStackHelper internet;
    internet.Install (remoteHostContainer);
    
    
    //***************Create and install a point to point connection between the SPGW and the remoteHost*****************//
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (p2pLinkDataRate)));
    p2ph.SetDeviceAttribute ("Mtu", UintegerValue (p2pLinkMtu));
    p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (p2pLinkDelay)));
    NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);		//The interfaces between the SPGW and remoteHost were saved in internetDevices.
    
    // Create the Internet
    Ipv4AddressHelper ipv4h;	//Ipv4AddressHelper is used to assign Ip Address for a typical node.
    ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);		//assign IP addresses in starting at "1.0.0.0" to the SPGW and remoteHost.
    // interface 0 is localhost, 1 is the p2p device
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);
    
    
    //***************************Let's the remoteHost know how to route to UE "7.0.0.0"**************************//
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    //get the static routing method to the remoteHost. The parameter for GetStaticRouting() is the Ptr<Ipv4> of the destination.
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());	//remoteHostStaticRouting now knows how to route to the remoteHost.
    remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);	//Add the routing entry to the remoteHostStaticRouting table.
    //"1" means interface #1 of the remoteHost will route to "7.0.0.0/24" (which is default Ipv4 range for UEs??)
    
    //**********************************Create Ue nodes, EnodeBs*******************************//
    NodeContainer ueNodes;
    NodeContainer enbNodes;
    enbNodes.Create(numberOfEnodebs);
    ueNodes.Create(numberOfUeNodes);
    
    
    //*************************Create positions, mobility model, and install Mobility Model************************//
    //************************Install LTE Devices to the nodes (install LTE stack to enodeB and Ue)******************//
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                      "MinX", DoubleValue (0.0),	//zero point
                                      "MinY", DoubleValue (0.0),	//zero point
                                      "DeltaX", DoubleValue (10000.0),	//distance among nodes
                                      "DeltaY", DoubleValue (10000.0),
                                      "GridWidth", UintegerValue (3),	//number of nodes on a line
                                      "LayoutType", StringValue ("RowFirst"));
    enbMobility.Install (enbNodes);
    
    /****************Set radio uplink/downlink bandwidth on eNB. Maximum 100 RBs, correspond to ~70Mbps peak rate***********/
    lteHelper->SetEnbDeviceAttribute("UlBandwidth",UintegerValue(radioUlBandwidth));
    lteHelper->SetEnbDeviceAttribute("DlBandwidth",UintegerValue(radioDlBandwidth));
    
    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);

    
    
    /**********************Ues position allocation*********************/
    NetDeviceContainer::Iterator enbLteDevIt = enbLteDevs.Begin ();
    Vector enbPosition = (*enbLteDevIt)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
    MobilityHelper ueMobility;
    ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",	//nodes are put randomly inside a circle with the central point is (x,y).
                                     "X", DoubleValue (enbPosition.x),
                                     "Y", DoubleValue (enbPosition.y),
                                     "rho", DoubleValue (distance));	//radius of the circle.
    ueMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    ueMobility.Install (ueNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);
    
    
    //**********************Assign Ipv4 addresses for UEs. Install the IP stack on the UEs******************//
    internet.Install (ueNodes);	//internet (InternetStackHelper) again be used to install an Internet stack for a node.

    // Assign IP address to UEs, and install applications
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));

    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        Ptr<Node> ueNode = ueNodes.Get (u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
        ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }


    //**********************Attach all UEs to eNodeB**********************//
    for (uint16_t i = 0; i < numberOfEnodebs; i++)
    {
        for (uint16_t t = 0; t < numberOfUeNodes; ++t){
            lteHelper->Attach (ueLteDevs.Get(t), enbLteDevs.Get(i));    //Attach function takes Interfaces as parameters.
        }
    }

    //*****************************Install and start applications on UEs and remote host****************************//
    uint16_t dlPort = 2000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        ++dlPort;				//each Ue will contact with the remoteHost by a different dlPort (the remoteHost needs this).
        

        if (isTcp == 1){
					/*********TCP Application********/
					//Create a packet sink to receive packet on remoteHost
					PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
					serverApps.Add(sink.Install(ueNodes.Get(u)));

					OnOffHelper onOffHelper("ns3::TcpSocketFactory", Address ( InetSocketAddress(ueIpIface.GetAddress(u) , dlPort) ));
					onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
					if (numberOfPackets != 0)
						onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
					clientApps.Add(onOffHelper.Install(remoteHost));
        }
        else{
					/*********UDP Application********/
					//Create a packet sink to receive packet on remoteHost
					PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
					serverApps.Add(sink.Install(ueNodes.Get(u)));

					OnOffHelper onOffHelper("ns3::UdpSocketFactory", Address ( InetSocketAddress(ueIpIface.GetAddress(u), dlPort) ));
					onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
					if (numberOfPackets != 0)
						onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
					clientApps.Add(onOffHelper.Install(remoteHost));
        }



//        ApplicationContainer apps

//        UdpServerHelper server(dlPort); /***SERVER CONTAINS MULTIPLE APPS, each app is receiving packet for 1 ue****/
//        serverApps.Add (server.Install (remoteHost));	//install a UdpEchoServerApplication on the remoteHost node.
//
//
//        UdpClientHelper ulClient (remoteHostAddr, dlPort);	/*Client app, destination is remoteHostAddr, destination port is dlPort*/
//        interPacketInterval *= (u+1);
//        ulClient.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
//        ulClient.SetAttribute ("MaxPackets", UintegerValue(numberOfPackets));
//        ulClient.SetAttribute("PacketSize", UintegerValue(packetSize));
//        clientApps.Add (ulClient.Install (ueNodes.Get(u)));	//install the client to the Ue node.
    }
    

    //    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
    //    em->SetAttribute ("ErrorRate", DoubleValue (0.00001));
    //    internetDevices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));

//        AsciiTraceHelper asciiTraceHelper;
//        Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");
//    		Ptr<Application> app = clientApps.Get(0);
//    		Ptr<Socket> appsocket = app->GetObject<OnOffApplication>()->GetSocket();
//    		appsocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, stream));


    /*******************Start client and server apps***************/
    serverApps.Start (Seconds (0.01));		//All server start at 0.01s.
    clientApps.Start(Seconds(0.5));
    
    
    /*********Tracing settings***************/
    lteHelper->EnableTraces ();
    lteHelper->GetPdcpStats()->SetAttribute("EpochDuration", TimeValue( Seconds (simTime)) );		//set collection interval for PDCP.
    lteHelper->GetRlcStats()->SetAttribute("EpochDuration", TimeValue ( Seconds (simTime)))	;		//same for RLC
    // Uncomment to enable PCAP tracing
    p2ph.EnablePcapAll("pcaps/"+pcapName);
    


    Ptr<FlowMonitor> monitor;
    FlowMonitorHelper flowHelper;
    monitor = flowHelper.Install(ueNodes);
    monitor = flowHelper.Install(remoteHost);
    monitor = flowHelper.GetMonitor();


    /****ConfigStore setting****/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("config-tcp-out-dl.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Save"));
    ConfigStore outputConfig;
    outputConfig.ConfigureDefaults();
    outputConfig.ConfigureAttributes();

    /**Pathloss**/
    DownlinkLteGlobalPathlossDatabase dlPathlossDb;
    UplinkLteGlobalPathlossDatabase ulPathlossDb;


    /*********Start the simulation*****/
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    

    /**************Simulation stops here. Start printing out information (if needed)***********/
    
    /******Print out simulation settings, client/server sent/received status*******/
    std::cout << "*************DL experiment**************"
    << "\nNumberofUeNodes = " << numberOfUeNodes
    << "\nNumberOfEnodeBs = " << numberOfEnodebs
    << "\nDistance = " << distance
    << "\nInterval = " << interPacketInterval
    << "\nPacket size = " << packetSize
    << "\nRate = " << dataRate
    << "\nNumber of Packets = " << numberOfPackets
    << "\nS1uLink " << s1uLinkDataRate << " " << s1uLinkDelay
    << "\np2pLink " << p2pLinkDataRate << " " << p2pLinkDelay << "\n";
    
    
    /*******Print out simulation results, MULTIPLE CELLs enabled********/
    int SPC = 15;   //width of a column.
    std::cout << std::left << std::setw(SPC) << "CellId"
    << std::left << std::setw(SPC) << "Imsi"
    << std::left << std::setw(SPC) << "PdcpUL(ms)"
    << std::left << std::setw(SPC) << "PdcpDL(ms)"
    << std::left << std::setw(SPC) << "RlcUl(ms)"
    << std::left << std::setw(SPC) << "RlcDl(ms)"
    << std::left << std::setw(SPC) << "ULPDCPTxs"
    << std::left << std::setw(SPC) << "ULRLCTxs"
    << std::left << std::setw(SPC) << "DLPDCPTxs"
    << std::left << std::setw(SPC) << "DLRLCTxs"
    << std::left << std::setw(SPC) << "UlPathloss"
    << std::left << std::setw(SPC) << "DlPathloss"
    << std::left << std::setw(SPC) << "OnOffSent"
    << std::left << std::setw(SPC+5) << "packetSinkReceived"
    << std::left << std::setw(SPC) << "OnOffSentTotal"
    << std::left << std::setw(SPC) << "packetSinkReceivedTotal"
    << std::endl;
    const uint32_t ONEBIL = 1000000000;
    Ptr<LteEnbNetDevice> lteEnbDev;

    for ( uint32_t i = 0 ; i < enbNodes.GetN(); ++i){
        for ( uint32_t j = 0; j < ueNodes.GetN(); ++j){
            lteEnbDev = enbLteDevs.Get (i)->GetObject<ns3::LteEnbNetDevice> ();
            std::cout << std::left << std::setw(SPC) << i+1;
            std::cout << std::left << std::setw(SPC) << j+1;
            std::cout << std::left << std::setw(SPC) << getUlPdcpDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getDlPdcpDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getUlRlcDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getDlRlcDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getUlPdcpTxs( lteHelper, j+1, 3);
            std::cout << std::left << std::setw(SPC) << getUlRlcTxs(lteHelper, j+1, 3);
            std::cout << std::left << std::setw(SPC) << getDlPdcpTxs(lteHelper, j+1, 3);
            std::cout << std::left << std::setw(SPC) << getDlRlcTxs(lteHelper, j+1, 3);
            std::cout << std::left << std::setw(SPC) << ulPathlossDb.GetPathloss(i+1,j+1);
            std::cout << std::left << std::setw(SPC) << dlPathlossDb.GetPathloss(i+1,j+1);
            std::cout << std::left << std::setw(SPC) << clientApps.Get(j)->GetObject<ns3::OnOffApplication>()->GetSent();
            std::cout << std::left << std::setw(SPC) << serverApps.Get(j)->GetObject<ns3::PacketSink>()->GetPacketReceived();
            std::cout << std::left << std::setw(SPC) << clientApps.Get(j)->GetObject<ns3::OnOffApplication>()->GetSentBytes();
            std::cout << std::left << std::setw(SPC) << serverApps.Get(j)->GetObject<ns3::PacketSink>()->GetTotalRx() << std::endl;
        }
    }

  monitor->CheckForLostPackets();
  Ptr<ns3::Ipv4FlowClassifier> classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  std::map <FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

  /**sending flow***/
  ns3::int64x64_t meanTxRate_1 = 0;
  ns3::int64x64_t meanRxRate_1 = 0;
  ns3::int64x64_t meanTcpDelay_1 = 0;
  uint64_t numOfLostPackets_1 = 0;
  uint64_t numOfTxPacket_1 = 0;

  /***ack flow***/
  ns3::int64x64_t meanTxRate_2 = 0;
  ns3::int64x64_t meanRxRate_2 = 0;
  ns3::int64x64_t meanTcpDelay_2 = 0;
  uint64_t numOfLostPackets_2 = 0;
  uint64_t numOfTxPacket_2 = 0;

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flow*/
    if (t.sourceAddress.IsEqual(Ipv4Address("7.0.0.2"))){
      if (iter->second.rxPackets > 1){
        meanTxRate_1 =  8*iter->second.txBytes/(iter->second.timeLastTxPacket-iter->second.timeFirstTxPacket)*ONEBIL/(1024);
        meanRxRate_1 = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024);
        meanTcpDelay_1 = iter->second.delaySum/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_1 = iter->second.lostPackets;
      numOfTxPacket_1 = iter->second.txPackets;
    }

    /*ack flow*/
    if (t.sourceAddress.IsEqual(Ipv4Address("1.0.0.2"))){
      if (iter->second.rxPackets > 1){
        meanTxRate_2 =  8*iter->second.txBytes/(iter->second.timeLastTxPacket-iter->second.timeFirstTxPacket)*ONEBIL/(1024);
        meanRxRate_2 = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024);
        meanTcpDelay_2 = iter->second.delaySum/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_2 = iter->second.lostPackets;
      numOfTxPacket_2 = iter->second.txPackets;
    }

    NS_LOG_UNCOND("***Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
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

  /****Radio link error and HARQ*******/
  uint64_t errorUlRx = lteHelper->GetPhyRxStatsCalculator()->GetTotalErrorUl();	//Error detected on the Ul Received side (eNB).
  uint64_t errorDlRx = lteHelper->GetPhyRxStatsCalculator()->GetTotalErrorDl(); //Error detected on the Dl Received side (UE).
  uint64_t harqUl = lteHelper->GetPhyTxStatsCalculator()->GetTotalUlHarqRetransmission();	//HARQ transmitted from eNB.
  uint64_t harqDl = lteHelper->GetPhyTxStatsCalculator()->GetTotalDlHarqRetransmission();	//HAEQ transmitted from Ue.

  NS_LOG_UNCOND("Total Error UlRx " << errorUlRx);
  NS_LOG_UNCOND("Total Error DlRx " << errorDlRx);
  NS_LOG_UNCOND("Total UlTx HARQ " <<  harqUl);
  NS_LOG_UNCOND("Total DlTx HARQ " << harqDl);



  //#Distance AppRate	MeanTx_1	Goodput(MeanRx)_1	TcpDelay_1	ul pdcp delay		ul rlc delay		ulPdcpTxs 	ulRlcTxs lostPkt_1
  //MeanTx_2	Goodput(MeanRx)_2	TcpDelay_2	dl pdcp delay		dl rlc delay		dlPdcpTxs 	dlRlcTxs	lostPkt_2
  //UlTx		UlRx	DlTx	DlRx
  //ErrorUlRx	ErrorDlRx	UlTxHarq	DlTxHarq

  /*Write to file*/
  //Application rate
  oFile << distance << "	";
  oFile << dataRate.substr(0,dataRate.length()-4) << "   ";
  /*sending flow*/
  //Mean transmitted
  oFile << meanTxRate_1.GetDouble()/1024 << "	";
  //Goodput
  oFile << meanRxRate_1.GetDouble()/1024 << "	";
  //TCP delay
  oFile << meanTcpDelay_1.GetDouble() << "	";
  //UL PDCP PDUs
  oFile << getUlPdcpDelay(lteHelper,1,3) << "		";
  //UL RLC PDUs
  oFile << getUlRlcDelay(lteHelper,1,3) << "		";
  //ulPdcpTxs
  oFile << getUlPdcpTxs(lteHelper,1,3)	<< "	";
  //ulRlcTxs
  oFile << getUlRlcTxs(lteHelper,1,3) << "		";
  //Lost packets
  oFile << numOfLostPackets_1 << "	";

  /*ack flow*/
  //Mean transmitted
  oFile << meanTxRate_2.GetDouble()/1024 << "	";
  //Goodput
  oFile << meanRxRate_2.GetDouble()/1024 << "	";
  //TCP delay
  oFile << meanTcpDelay_2.GetDouble() << "	";
  //DL PDCP PDUs
  oFile << getDlPdcpDelay(lteHelper,1,3) << "		";
  //DL RLC PDUs
  oFile << getDlRlcDelay(lteHelper,1,3) << "		";
  //dlPdcpTxs
  oFile << getDlPdcpTxs(lteHelper,1,3) << " 	";
  //dlRlcTxs
  oFile << getDlRlcTxs(lteHelper,1,3) << "		";
  //Lost packets
  oFile << numOfLostPackets_2 << "	";

  /*phy layer*/
  //Total Phy Ul Tx
  oFile << lteHelper->GetPhyTxStatsCalculator()->GetTotalUl() << "	";
  //Total Phy Ul Rx
  oFile << lteHelper->GetPhyRxStatsCalculator()->GetTotalUl() << "	";
  //Total Phy Dl Tx
   oFile << lteHelper->GetPhyTxStatsCalculator()->GetTotalDl() << "		";
   //Total Phy Dl Rx
   oFile << lteHelper->GetPhyRxStatsCalculator()->GetTotalDl() << "		";

   /*Error and Harq*/
   oFile << errorUlRx << "	";
   oFile << errorDlRx << "	";
   oFile << harqUl << "	";
   oFile << harqDl << "	";

   if (harqUl != 0 or harqDl != 0){
  	 NS_LOG_UNCOND("IsPhyError: True");
  	 oFile << "true";
   }

   oFile << "\n";

  oFile.close();


  Simulator::Destroy();
  return 0;
    
}

/***Calculate average of a map**/
double
CalculateAverageDelay(std::map <uint64_t, uint32_t> delayArray){
	double sum = 0;
	uint64_t counter = 0;
	for (std::map<uint64_t, uint32_t>::iterator ii = delayArray.begin(); ii != delayArray.end(); ++ii){
		sum += (*ii).second;
		counter++;
	}
	return (sum/counter);
}

/**Calculate average of an array**/
double
getAverage(std::list<double> array){
	double sum = 0;
	uint32_t ctr = 0;
	std::list<double>::iterator it;
	for (it = array.begin(); it != array.end(); it++){
		ctr++;
		sum += *it;
	}
	return sum/ctr;
}

/*
 * Get ul rlc pdus delay in ms
 */
double
getUlRlcDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->ulDelayStatsMap[p]);
}

double
getUlPdcpDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetPdcpStats()->ulDelayStatsMap[p]);
}

double
getDlRlcDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->dlDelayStatsMap[p]);
}

double
getDlPdcpDelay(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetPdcpStats()->dlDelayStatsMap[p]);
}

uint32_t
getUlRlcTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->ulTxPacketsMap[p]);
}

uint32_t
getUlRlcRxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->ulRxPacketsMap[p]);
}

uint32_t
getDlRlcTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->dlTxPacketsMap[p]);
}

uint32_t
getDlPdcpTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetPdcpStats()->dlTxPacketsMap[p]);
}

uint32_t
getDlRlcRxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetRlcStats()->dlRxPacketsMap[p]);
}



uint32_t
getUlPdcpTxs(Ptr<ns3::LteHelper> lteHelper, uint32_t imsi, uint8_t lcid){
	ImsiLcidPair_t p (imsi, lcid);
	return (lteHelper->GetPdcpStats()->ulTxPacketsMap[p]);
}

/*
static void
CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldCwnd << "\t" << newCwnd << std::endl;
}
*/

