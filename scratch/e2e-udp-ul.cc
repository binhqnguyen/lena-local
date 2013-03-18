

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
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include <ns3/buildings-mobility-model.h>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/building.h>
#include <ns3/lte-common.h> //For ImsiLcidPair_t

//#include "iomanip.h"
//#include "ns3/gtk-config-store.h"

#include <map>

using namespace ns3;
/**
 * Topology:     			 Ues (x numberOfNodes) ---------- EnodeB ---------- |
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
NS_LOG_COMPONENT_DEFINE ("e2e-udp");

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
//       LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);
//       LogComponentEnable ("LteRlcAm", level);
    //   LogComponentEnable ("LteHelper",level);
    //   LogComponentEnable ("LteUeMac", level);
    //   LogComponentEnable ("LteEnbMac", level);
//       LogComponentEnable ("LtePdcp", level);
    //   LogComponentEnable ("LtePhy", level);
    
    
	//topology
    uint16_t numberOfUeNodes = 1;
    uint16_t numberOfEnodebs = 1;
    
    //S1uLink
    std::string s1uLinkDataRate = "1Gb/s";
    double	s1uLinkDelay = 0.01;
    uint16_t s1uLinkMtu	= 1000;
    
    //p2pLink
    std::string p2pLinkDataRate = "1Gb/s";
    double p2pLinkDelay = 0.01;
    uint16_t p2pLinkMtu = 1000;
    
    //Simulation
    uint32_t numberOfPackets = 10000000;
    uint16_t packetSize = 900;
    double simTime = 20;	//simulation time for EACH application
    double distance = 1000.0;
    double interPacketInterval = 2000;	//in micro seconds, minimum 1.
    uint16_t radioUlBandwidth = 25;	//the radio link bandwidth among UEs and EnodeB (in Resource Blocks). This is the configuration on LteEnbDevice.
    uint16_t radioDlBandwidth = 50;	//same as above, for downlink.
    std::string pcapName = "e2e-udp-ul";
    
    std::ofstream oFile;
    oFile.open("/Users/binh/Desktop/ns3_play/lena-output-ul.txt", std::ios::app);
    uint16_t isAMRLC = 0;
    uint16_t isError = 0;
    
    
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
    // cmd.AddValue("oFileName", "Output file name", oFileName);
    cmd.AddValue("isAMRLC", "Whether using AM RLC (UM RLC if false)", isAMRLC);
    cmd.AddValue("isError", "Whether having error in the radio link", isError);

//    Config::Reset();

    /**ConfigStore setting*/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("config-in.txt"));
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
    
//    if (isAMRLC==1)
//    	Config::SetDefault("ns3::LteEnbRrc::EpsBearerToRlcMapping", StringValue("RlcAmAlways"));
//
//    if (isError == 0 )
//    	Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));
//
//    Config::SetDefault("ns3::PfFfMacScheduler::HarqEnabled", BooleanValue(false));

//    Config::SetDefault ("ns3::LteEnbDevice::UlBandwidth", UintegerValue (50));
//	Config::SetDefault("ns3::LteEnbRrc::EpsBearerToRlcMapping", StringValue("RlcAmAlways"));




//    Config::SetDefault ("ns3::LteUePhy::NoiseFigure", UintegerValue(5));
//    lteHelper->SetSpectrumChannelAttribute("ns3::LteUePhy::NoiseFigure", IntegerValue(5));

//    Config::SetDefault("ns3::LteEnbRrc::EpsBearerToRlcMapping", StringValue("RlcAmAlways"));
    /*************Setting Radio link MAC scheduler**************/
    
    //  lteHelper->SetSchedulerType ("ns3::FdMtFfMacScheduler");    // FD-MT scheduler
    //  lteHelper->SetSchedulerType ("ns3::TdMtFfMacScheduler");    // TD-MT scheduler
    //  lteHelper->SetSchedulerType ("ns3::TtaFfMacScheduler");     // TTA scheduler
    //  lteHelper->SetSchedulerType ("ns3::FdBetFfMacScheduler");   // FD-BET scheduler
    //  lteHelper->SetSchedulerType ("ns3::TdBetFfMacScheduler");   // TD-BET scheduler
    //  lteHelper->SetSchedulerType ("ns3::FdTbfqFfMacScheduler");  // FD-TBFQ scheduler
    //  lteHelper->SetSchedulerType ("ns3::TdTbfqFfMacScheduler");  // TD-TBFQ scheduler
    //  lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler");     //PSS scheduler
        
    
    // parse again so you can override default values from the command line
    cmd.Parse(argc, argv);
    
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
    
    //Print out the addresses of the PGW and remoteHost.
    //  std::cout << "Point to point connection between SPGW and remoteHost....\n"
    //  					<< "SPGW address, remoteHost address = "
    //  					<< internetIpIfaces.GetAddress(0)
    //  					<<", " << internetIpIfaces.GetAddress(1)<< std::endl;
    //****************************************************************************************//
    
    
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
//     lteHelper->SetEnbDeviceAttribute("UlBandwidth",UintegerValue(radioUlBandwidth));
//     lteHelper->SetEnbDeviceAttribute("DlBandwidth",UintegerValue(radioDlBandwidth));

    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);

    
    /**********************Ues position allocation*********************/
    NetDeviceContainer::Iterator enbLteDevIt = enbLteDevs.Begin ();
    Vector enbPosition = (*enbLteDevIt)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
    MobilityHelper ueMobility;
    ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",	//nodes are put randomly inside a circle with the central point is (x,y).
                                     "X", DoubleValue (enbPosition.x),
                                     "Y", DoubleValue (enbPosition.y),
                                     "rho", DoubleValue (100.0));	//radius of the circle.
    ueMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    ueMobility.Install (ueNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

    lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FixedRssLossModel"));
    
    
    //**********************Assign Ipv4 addresses for UEs. Install the IP stack on the UEs******************//
    internet.Install (ueNodes);	//internet (InternetStackHelper) again be used to install an Internet stack for a node.

    //Print out Ues' Ipv4 addresses.
    //  for (uint16_t i = 0; i < ueIpIface.GetN(); ++i)
    //  	std::cout << "UE " << i << " has been assigned Ipv4 address = " << ueIpIface.GetAddress(i) << std::endl;
    //  std::cout << "UEs default gate way = " << epcHelper->GetUeDefaultGatewayAddress() << std::endl;
    // Assign IP address to UEs, and install applications
    Ipv4InterfaceContainer ueIpIface;
    // UE's IPv4 address is assigned by EpcHelper (TYPICAL node's IpV4 address assigned by Ipv4AddressHelper).
    //EpcHelper will AUTOMATICALLLY use address range beginning at "7.0.0.0/24" for UEs???????
    //As usual, interface 0 for local host, 1 is the Ue --- EnodeB.
    ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));

    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        Ptr<Node> ueNode = ueNodes.Get (u);

        //Ptr<NetDevice> ueLteDevice = ueLteDevs.Get (u);

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


        //Active bearers for Ues.
//    lteHelper->ActivateDedicatedEpsBearer (ueLteDevs, EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), EpcTft::Default ());
//     enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
//     EpsBearer bearer (q);
//     lteHelper->ActivateDataRadioBearer (ueLteDevs, bearer);
    // //std::cout << "xxxx\n";

    
    //*****************************Install and start applications on UEs and remote host****************************//
    uint16_t ulPort = 2000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;
    
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        ++ulPort;				//each Ue will contact with the remoteHost by a different ulport (the remoteHost needs this).
        
        UdpServerHelper server(ulPort); /***SERVER CONTAINS MULTIPLE APPS, each app is receiving packet for 1 ue****/
        serverApps.Add (server.Install (remoteHost));	//install a UdpEchoServerApplication on the remoteHost node.
        
        
        UdpClientHelper ulClient (remoteHostAddr, ulPort);	/*Client app, destination is remoteHostAddr, destination port is ulPort*/
        interPacketInterval *= (u+1);
        ulClient.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
        ulClient.SetAttribute ("MaxPackets", UintegerValue(numberOfPackets));
        ulClient.SetAttribute("PacketSize", UintegerValue(packetSize));
        clientApps.Add (ulClient.Install (ueNodes.Get(u)));	//install the client to the Ue node.
    }
    
    /*******************Start client and server apps***************/
    serverApps.Start (Seconds (0.01));		//All server start at 0.01s.
    //for (uint16_t i = 0; i < clientApps.GetN(); ++i){
    //    clientApps.Get(i)->SetStartTime( Seconds (0.5) );		//all apps start at 0.5s.
        //  	std::cout << "App " << i << " starts at " << stime << std::endl;
    //}
    clientApps.Start(Seconds(0.5));
    //clientApps.Get(0)->SetStartTime( Seconds (0.5) );
    
    
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
//    monitor = flowHelper.GetMonitor();


//    // keep track of all path loss values in two centralized objects
//    DownlinkLteGlobalPathlossDatabase dlPathlossDb;
//    UplinkLteGlobalPathlossDatabase ulPathlossDb;
//    // we rely on the fact that LteHelper creates the DL channel object first, then the UL channel object,
//    // hence the former will have index 0 and the latter 1
//    Config::Connect ("/ChannelList/0/PathLoss",
//                     MakeCallback (&DownlinkLteGlobalPathlossDatabase::UpdatePathloss, &dlPathlossDb));
//    Config::Connect ("/ChannelList/1/PathLoss",
//                      MakeCallback (&UplinkLteGlobalPathlossDatabase::UpdatePathloss, &ulPathlossDb));


    /****ConfigStore setting****/

    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("config-out.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Save"));
    ConfigStore outputConfig;
    outputConfig.ConfigureDefaults();
    outputConfig.ConfigureAttributes();


    /*********Start the simulation*****/
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    
    /**************Simulation stops here. Start printing out information (if needed)***********/
    
    /************Get number of sent/received packets of the server***********/
    std::vector<uint32_t>	clientSentCount;
    std::vector<uint32_t>	clientReceivedCount;
    uint32_t	serverSentCount  = serverApps.Get(0)->GetObject<ns3::UdpServer>()->sent;
    uint32_t serverReceivedCount = serverApps.Get(0)->GetObject<ns3::UdpServer>()->GetReceived();
    /*This code gets clients' sent/received status, note: this is the APPLICATION rates, does NOT mean the actually sent on the radio*/
    for ( uint32_t i = 0; i < clientApps.GetN(); ++i){
        clientSentCount.push_back(clientApps.Get(i)->GetObject<ns3::UdpClient>()->GetSent());
        clientReceivedCount.push_back(clientApps.Get(i)->GetObject<ns3::UdpClient>()->GetReceived());
    }
    


    /******Print out simulation settings, client/server sent/received status*******/
    double appRate = double(packetSize)*8*1000000/(1024*1024*interPacketInterval);
    std::cout << "*************UL experiment**************"
    << "\nNumberofUeNodes = " << numberOfUeNodes
    << "\nNumberOfEnodeBs = " << numberOfEnodebs
    << "\nDistance = " << distance
    << "\nInterval = " << interPacketInterval
    << "\nPacket size = " << packetSize
    << "\nRate = " << appRate << "Mbps"
    << "\nNumber of Packets = " << numberOfPackets
    << "\nS1uLink " << s1uLinkDataRate << " " << s1uLinkDelay
    << "\np2pLink " << p2pLinkDataRate << " " << p2pLinkDelay
    << "\nServer sent/received = " << serverSentCount
    << " / " << serverReceivedCount << std::endl;
    
    
    /*******Print out simulation results, MULTIPLE CELLs enabled********/
    int SPC = 15;   //width of a column.
    std::cout << std::left << std::setw(SPC) << "CellId"
    << std::left << std::setw(SPC) << "Imsi"
    << std::left << std::setw(SPC) << "PdcpUL(ms)"
    << std::left << std::setw(SPC) << "PdcpDL(ms)"
    << std::left << std::setw(SPC) << "RlcUl(ms)"
    << std::left << std::setw(SPC) << "RlcDl(ms)"
    << std::left << std::setw(SPC) << "Latency(ms)"
    << std::left << std::setw(SPC) << "Client Sent"
    << std::left << std::setw(SPC) << "Server Received (Total*)"
    << std::left << std::setw(SPC) << "Rnd_succ"
    << std::left << std::setw(SPC) << "ULPDCPTxs"
    << std::left << std::setw(SPC) << "ULRLCTxs" << std::endl;
    const uint32_t ONEBIL = 1000000000;
    std::list<double> latency_array;
    std::list<double> cell_aggregate_latency;
    uint16_t ulB = 0;
    uint16_t dlB = 0;
    Ptr<LteEnbNetDevice> lteEnbDev;
//     for (uint32_t i = 0; i <10; ++i){
//     	for (uint32_t j = 0; j < 10; ++j){
//     	    std::cout << lteHelper->GetRlcStats()->GetUlDelay(i,j) << std::endl;
//     	}
//     }

//    std::cout << getUlPdcpDelay(lteHelper,1,3) << std::endl;
//    std::cout << getUlRlcDelay(lteHelper,1,3) << std::endl;


    for ( uint32_t i = 0 ; i < enbNodes.GetN(); ++i){
        latency_array.clear();
        for ( uint32_t j = 0; j < ueNodes.GetN(); ++j){
            lteEnbDev = enbLteDevs.Get (i)->GetObject<ns3::LteEnbNetDevice> ();
            latency_array.push_back(CalculateAverageDelay (clientApps.Get(j)->GetObject<ns3::UdpClient>()->GetDelayArray()));
            std::cout << std::left << std::setw(SPC) << i+1;
            std::cout << std::left << std::setw(SPC) << j+1;
            std::cout << std::left << std::setw(SPC) << getUlPdcpDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getDlPdcpDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getUlRlcDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << getDlRlcDelay(lteHelper,j+1,3);
            std::cout << std::left << std::setw(SPC) << CalculateAverageDelay (clientApps.Get(j)->GetObject<ns3::UdpClient>()->GetDelayArray());
            std::cout << std::left << std::setw(SPC) << clientSentCount.at(j);
            std::cout << std::left << std::setw(SPC) << serverReceivedCount;//clientReceivedCount.at(j);
            std::cout << std::left << std::setw(SPC) << double (clientReceivedCount.at(j))*100/clientSentCount.at(j);
            std::cout << std::left << std::setw(SPC) << lteHelper->GetPdcpStats()->GetUlTxPackets(j+1,3);
            std::cout << std::left << std::setw(SPC) << lteHelper->GetRlcStats()->GetUlTxPackets(j+1,3) << std::endl;
        }
        //cell_aggregate_latency.push_back(getAverage(latency_array));
        ulB = lteEnbDev->GetUlBandwidth();
        dlB = lteEnbDev->GetDlBandwidth();
        std::cout << "\nCell " << (i+1) << " average latency " << getAverage(latency_array) << "ms"
        << "\nCell " << (i+1) << " eNBs ul/dl bandwidth(RBs): " << int (lteEnbDev->GetUlBandwidth()) << "/" << int (lteEnbDev->GetDlBandwidth()) << "\n";
    }
    

//    std::list<double>::iterator it;
//    uint16_t cellId = 0;
//    for (it = cell_aggregate_latency.begin(); it != cell_aggregate_latency.end(); it++){
//        cellId++;
//        std::cout << "Cell " << cellId << " average latency = " << *it << "ms" << std::endl << std::endl;
//    }
    
//    std::list<double> receiving_interval_list = serverApps.Get(0)->GetObject<ns3::UdpServer>()->receiving_interval;
//    receiving_interval_list.pop_front();	//delete the first unvalid value.
//    //  for (it = receiving_interval_list.begin(); it != receiving_interval_list.end(); ++it){
//    //  	std::cout << *it << " ";
//    //  }
//    std::cout << "\nServer receiving interval average = " << getAverage(receiving_interval_list) << "us\n";
    
    
    /* Print out server Apps rate and client Apps rate*/
    double sTxRate ;
	double sRxRate ;
	double clTxRate = 0;
	double clRxRate = 0;
	int crx = 0;
    int srx = 0;
    for (uint32_t i = 0; i < clientApps.GetN(); ++i){
        clTxRate = clientApps.Get(i)->GetObject<ns3::UdpClient>()->GetSentRate();
        clRxRate = clientApps.Get(i)->GetObject<ns3::UdpClient>()->GetReceivedRate();
        sTxRate = serverApps.Get(i)->GetObject<ns3::UdpServer>()->GetSentRate();
        sRxRate = serverApps.Get(i)->GetObject<ns3::UdpServer>()->GetReceivedRate();
        crx += clientApps.Get(i)->GetObject<ns3::UdpClient>()->GetReceived();
        srx += serverApps.Get(i)->GetObject<ns3::UdpServer>()->GetReceived();
        std::cout << "Client "<< i << " TxRate/RxRate = " << clTxRate << " " << clRxRate << " Mbps\n";
        std::cout << "Server " << i << " TxRate/RxRate = " << sTxRate << " " << sRxRate << " Mbps\n";
    }
    std::cout << "Client total received " << crx << " packets\n";
    std::cout << "Server total received " << srx << " packets\n";
    
    /*Write to file*/
    //Application rate
    oFile << appRate << "   ";
    //goodput (that received on server)
    oFile << sRxRate << "   ";
    //Loss rate (that the ratio of Application Client sent/Client received)
    oFile << double (serverApps.Get(0)->GetObject<ns3::UdpServer>()->GetReceived())/clientApps.Get(0)->GetObject<ns3::UdpClient>()->GetSent()*100 << "  ";
    //UL PDCP PDUs
    oFile << getUlPdcpDelay(lteHelper,1,3) << " ";
    //UL RLC PDUs
    oFile << getUlRlcDelay(lteHelper,1,3) << "\n";

    oFile.close();

  monitor->CheckForLostPackets();
  Ptr<ns3::Ipv4FlowClassifier> classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  std::map <FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    NS_LOG_UNCOND("***Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
    NS_LOG_UNCOND("Tx Packets " << iter->second.txPackets);
    NS_LOG_UNCOND("Rx Packets " << iter->second.rxPackets);
    NS_LOG_UNCOND("Lost ratio " << double (iter->second.lostPackets)/(iter->second.lostPackets+iter->second.rxPackets));
    NS_LOG_UNCOND("Number of lost packets " << iter->second.lostPackets);
    if (iter->second.rxPackets != 0){
        NS_LOG_UNCOND("Average delay received " << iter->second.delaySum/iter->second.rxPackets/1000000);
        NS_LOG_UNCOND("Mean received bitrate " << 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024*1024));
    }
    // NS_LOG_UNCOND("Mean received bitrate " << 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024*1024));
    NS_LOG_UNCOND("Mean transmitted bitrate " << 8*iter->second.txBytes/(iter->second.timeLastTxPacket-iter->second.timeFirstTxPacket)*ONEBIL/(1024*1024));
    if (iter->second.packetsDropped.size() != 0){
    	NS_LOG_UNCOND("Dropped NO ROUTE " << iter->second.packetsDropped.at(0));
    	NS_LOG_UNCOND("Dropped TTL EXPIRED " << iter->second.packetsDropped.at(1));
    	NS_LOG_UNCOND("Dropped BAD CHECKSUM " << iter->second.packetsDropped.at(2));
    }

  }

//  // print the pathloss values at the end of the simulation
//  std::cout << std::endl << "Downlink pathloss:" << std::endl;
//  dlPathlossDb.Print ();
//  std::cout << std::endl << "Uplink pathloss:" << std::endl;
//  ulPathlossDb.Print ();


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

