/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
#include "udp-client.h"
#include "seq-ts-header.h"
#include <map>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpClient");
NS_OBJECT_ENSURE_REGISTERED (UdpClient);

TypeId
UdpClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpClient")
    .SetParent<Application> ()
    .AddConstructor<UdpClient> ()
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets the application will send",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpClient::m_count),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval",
                   "The time to wait between packets", TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&UdpClient::m_interval),
                   MakeTimeChecker ())
    .AddAttribute (
      "RemoteAddress",
      "The destination Address of the outbound packets",
      AddressValue (),
      MakeAddressAccessor (&UdpClient::m_peerAddress),
      MakeAddressChecker ())
    .AddAttribute ("RemotePort", "The destination port of the outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpClient::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketSize",
                   "Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&UdpClient::m_size),
                   MakeUintegerChecker<uint32_t> (12,1500))
  ;
  return tid;
}

UdpClient::UdpClient ()
{
  NS_LOG_FUNCTION (this);
  m_sent = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
  m_delay = 0;
  m_received = 0;
}

uint32_t UdpClient::GetReceived(){
	return m_received;
}

uint32_t UdpClient::GetSent(){
	return m_sent;
}
std::map <uint64_t,uint32_t>
UdpClient::GetDelayArray(){
	return m_delayArray;
}

UdpClient::~UdpClient ()
{
  NS_LOG_FUNCTION (this);
}

void
UdpClient::SetRemote (Ipv4Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = Address(ip);
  m_peerPort = port;
}

void
UdpClient::SetRemote (Ipv6Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = Address(ip);
  m_peerPort = port;
}

void
UdpClient::SetRemote (Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = ip;
  m_peerPort = port;
}

void
UdpClient::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
UdpClient::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind ();
          m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind6 ();
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
    }

//  m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  m_socket->SetRecvCallback (MakeCallback (&UdpClient::HandleRead, this));

  m_sendEvent = Simulator::Schedule (Seconds (0.0), &UdpClient::Send, this);
}

void
UdpClient::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  if (m_socket != 0)
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket = 0;
    }
  Simulator::Cancel (m_sendEvent);
}

void
UdpClient::Send (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_sendEvent.IsExpired ());
  SeqTsHeader seqTs;
  seqTs.SetSeq (m_sent);
  Ptr<Packet> p = Create<Packet> (m_size-(8+4)); // 8+4 : the size of the seqTs header
  p->AddHeader (seqTs);

  sent_seq.push_back(m_sent);

  std::stringstream peerAddressStringStream;
  if (Ipv4Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
    }
  else if (Ipv6Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
    }

  if ((m_socket->Send (p)) >= 0)
    {

    if (m_sent == 0)
    	m_Txbegin = Simulator::Now().GetDouble();
    m_TxSize += p->GetSize();
    m_Txend = Simulator::Now().GetDouble();

      ++m_sent;
//      NS_LOG_INFO ("*CL* TraceDelay TX " << m_size << " bytes to "
//                                    << peerAddressStringStream.str () << " Uid: "
//                                    << p->GetUid () << " Time: "
//                                    << (Simulator::Now ()).GetSeconds ());

//				NS_LOG_INFO ("*Client sent " << m_size << " bytes to "
//																			<< peerAddressStringStream.str () << " Seq: "
//																			<< seqTs.GetSeq() << " Time: "
//																			<< (Simulator::Now ()).GetSeconds ());

    }
  else
    {
//      NS_LOG_INFO ("*CL* Error while sending " << m_size << " bytes to "
//                                          << peerAddressStringStream.str ());
  	NS_LOG_INFO ("*Client Error while sending " << m_size << " bytes to "
  	                                          << peerAddressStringStream.str ());
    }

  if (m_sent < m_count)
    {
      m_sendEvent = Simulator::Schedule (m_interval, &UdpClient::Send, this);
    }
}

void
UdpClient::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
      		m_received++;
          SeqTsHeader seqTs;
          packet->RemoveHeader (seqTs);
          uint32_t currentSequenceNumber = seqTs.GetSeq ();
          if (InetSocketAddress::IsMatchingType (from))
            {

          	if (m_received == 0)
          		m_Rxbegin = Simulator::Now().GetDouble();
          	m_Rxend = Simulator::Now().GetDouble();;
          	m_RxSize += packet->GetSize();

//            NS_LOG_INFO ("*Client received " << packet->GetSize () <<
//                         " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
//                         " Seq: " << currentSequenceNumber <<
////                         " Uid: " << packet->GetUid () <<
//                         " TXtime: " << seqTs.GetTs () <<
//                         " RXtime: " << Simulator::Now () <<
//                         " DELAY: " << (Simulator::Now () - seqTs.GetTs ()).GetDouble()/1000000 <<"ms");
          		m_delay = (Simulator::Now () - seqTs.GetTs ()).GetDouble()/1000000;
							NS_LOG_INFO (
//													 " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
													 " Seq: " << currentSequenceNumber <<
	//                         " Uid: " << packet->GetUid () <<
													 " TXtime: " << seqTs.GetTs () <<
													 " RXtime: " << Simulator::Now () <<
													 " DELAY: " << m_delay <<"ms");
							m_delayArray[currentSequenceNumber] = m_delay;
                
                
//                std::cout << "*Client: RX " << packet->GetSize () <<
//                " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
//                " Sequence Number: " << currentSequenceNumber <<
//                " Uid: " << packet->GetUid () <<
//                " TXtime: " << seqTs.GetTs () <<
//                " RXtime: " << Simulator::Now () <<
//                " Delay: " << Simulator::Now () - seqTs.GetTs () << std::endl;
                
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
        }
    }
//  Simulator::Stop();
}


double
UdpClient::GetReceivedRate(){
	return (double (m_RxSize)*8*1000000000/( (m_Rxend - m_Rxbegin)*1024*1024) );
}

double
UdpClient::GetSentRate(){
	return (double (m_TxSize)*8*1000000000/( (m_Txend - m_Txbegin)*(1024*1024)));
}

std::list<uint64_t>
UdpClient::GetSentSeq(){
	return sent_seq;
}


} // Namespace ns3
