/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "packet-loss-counter.h"

#include "seq-ts-header.h"
#include "udp-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpServer");
NS_OBJECT_ENSURE_REGISTERED (UdpServer);


TypeId
UdpServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpServer")
    .SetParent<Application> ()
    .AddConstructor<UdpServer> ()
    .AddAttribute ("Port",
                   "Port on which we listen for incoming packets.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketWindowSize",
                   "The size of the window used to compute the packet loss. This value should be a multiple of 8.",
                   UintegerValue (32),
                   MakeUintegerAccessor (&UdpServer::GetPacketWindowSize,
                                         &UdpServer::SetPacketWindowSize),
                   MakeUintegerChecker<uint16_t> (8,256))
  ;
  return tid;
}

UdpServer::UdpServer ()
  : m_lossCounter (0)
{
  NS_LOG_FUNCTION (this);
  m_received=0;
  sent = 0;
  last_received = 0;


  m_RxSize = 0;
  m_TxSize = 0;
  m_Txbegin = 0;
  m_Txend = 0;
  oFile.open("/Users/binh/Desktop/ns3_play/udp-server-out.txt");
  oFile << "#packetID    Delay(ms)\n";
}

UdpServer::~UdpServer ()
{
  NS_LOG_FUNCTION (this);
}

uint16_t
UdpServer::GetPacketWindowSize () const
{
  NS_LOG_FUNCTION (this);
  return m_lossCounter.GetBitMapSize ();
}

void
UdpServer::SetPacketWindowSize (uint16_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_lossCounter.SetBitMapSize (size);
}

uint32_t
UdpServer::GetLost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_lossCounter.GetLost ();
}

uint32_t
UdpServer::GetReceived (void) const
{
  NS_LOG_FUNCTION (this);
  return m_received;
}

void
UdpServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
UdpServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);


  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
                                                   m_port);
      m_socket->Bind (local);
    }

  m_socket->SetRecvCallback (MakeCallback (&UdpServer::HandleRead, this));

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (),
                                                   m_port);
      m_socket6->Bind (local);
    }

  m_socket6->SetRecvCallback (MakeCallback (&UdpServer::HandleRead, this));

}

void
UdpServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);
  oFile.close();

  if (m_socket != 0)
    {
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void
UdpServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          SeqTsHeader seqTs;
          packet->RemoveHeader (seqTs);
          uint32_t currentSequenceNumber = seqTs.GetSeq ();

          received_seq.push_back(currentSequenceNumber);

          if (InetSocketAddress::IsMatchingType (from))
            {
          	if (m_received == 0)
          		m_Rxbegin = Simulator::Now().GetDouble();
          	receiving_interval.push_back((Simulator::Now().GetDouble() - last_received)/1000) ;
          	last_received = Simulator::Now().GetDouble();
          	m_Rxend = last_received;
          	m_RxSize += packet->GetSize();
//              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
//                           " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
//                           " Sequence Number: " << currentSequenceNumber <<
//                           " Uid: " << packet->GetUid () <<
//                           " TXtime: " << seqTs.GetTs () <<
//                           " RXtime: " << Simulator::Now () <<
//                           " Delay: " << Simulator::Now () - seqTs.GetTs ());


            NS_LOG_INFO ("**Server received " << packet->GetSize () + 12 <<
                         " bytes from " << InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
                         " Seq: " << currentSequenceNumber <<
                         " Uid: " << packet->GetUid () <<
                         " TXtime: " << seqTs.GetTs () <<
                         " RXtime: " << Simulator::Now () <<
                         " DELAY: " << (Simulator::Now () - seqTs.GetTs ()).GetDouble()/1000000 << "ms");
            oFile << currentSequenceNumber << " " << (Simulator::Now () - seqTs.GetTs ()).GetDouble()/1000000 << "\n"; 
//                std::cout << "**Server received " << packet->GetSize () + 12 <<
//                " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
//                " Seq: " << currentSequenceNumber <<
//                " Uid: " << packet->GetUid () <<
//                " TXtime: " << seqTs.GetTs () <<
//                " RXtime: " << Simulator::Now () << 
//                " DELAY: " << (Simulator::Now () - seqTs.GetTs ()).GetDouble()/1000000 <<"ms" << std::endl;
            }
          else if (Inet6SocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
                           " bytes from "<< Inet6SocketAddress::ConvertFrom (from).GetIpv6 () <<
                           " Sequence Number: " << currentSequenceNumber <<
                           " Uid: " << packet->GetUid () <<
                           " TXtime: " << seqTs.GetTs () <<
                           " RXtime: " << Simulator::Now () <<
                           " Delay: " << Simulator::Now () - seqTs.GetTs ());
            }

          m_lossCounter.NotifyReceived (currentSequenceNumber);
          m_received++;
          
          /*
          ////NEEDED!!!!
  	      packet->RemoveAllPacketTags ();
  	      packet->RemoveAllByteTags ();
          packet->AddHeader(seqTs);

          socket->SendTo (packet, 0, from);
          if (sent == 0)
          	m_Txbegin = Simulator::Now().GetDouble();
          m_TxSize += packet->GetSize();
          m_Txend = Simulator::Now().GetDouble();
          sent++;
  	      if (InetSocketAddress::IsMatchingType (from))
  	        {
  	          NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << packet->GetSize () << " bytes to " <<
  	                       InetSocketAddress::ConvertFrom (from).GetIpv4 () << " port " <<
  	                       InetSocketAddress::ConvertFrom (from).GetPort () );
                
//                std::cout << "At time " << Simulator::Now ().GetSeconds () << "s server "<< m_port << " sent " << packet->GetSize () << " bytes to " <<
//                InetSocketAddress::ConvertFrom (from).GetIpv4 () << " port " <<
//                InetSocketAddress::ConvertFrom (from).GetPort () << std::endl;
  	        }
          */
            
        }
    }
}

uint32_t UdpServer::GetReceived(){
	return m_received;
}

uint32_t UdpServer::GetSent(){
	return sent;
}

double
UdpServer::GetReceivedRate(){
	return (double (m_RxSize)*8*1000000000/( (m_Rxend - m_Rxbegin)*1024*1024) );
}

double
UdpServer::GetSentRate(){
	return (double (m_TxSize)*8*1000000000/( (m_Txend - m_Txbegin)*(1024*1024)));
}

std::list<uint64_t>
UdpServer::GetReceivedSeq(){
	return received_seq;
}

} // Namespace ns3
