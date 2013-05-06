/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 * modified by: Marco Miozzo <mmiozzo@cttc.es>
 *        Convert MacStatsCalculator in PhyRxStatsCalculator
 */

#include "phy-rx-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PhyRxStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (PhyRxStatsCalculator);

PhyRxStatsCalculator::PhyRxStatsCalculator ()
  : m_dlRxFirstWrite (true),
    m_ulRxFirstWrite (true)
{
  NS_LOG_FUNCTION (this);
  totalUlRx = 0;
  totalDlRx = 0;
  totalErrorDlRx = 0;
  totalErrorUlRx = 0;
}

PhyRxStatsCalculator::~PhyRxStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_UNCOND ("Phy UlRx = " << totalUlRx);
  NS_LOG_UNCOND ("Phy DlRx = " << totalDlRx);
}

TypeId
PhyRxStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PhyRxStatsCalculator")
    .SetParent<LteStatsCalculator> ()
    .AddConstructor<PhyRxStatsCalculator> ()
    .AddAttribute ("DlRxOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlRxPhyStats.txt"),
                   MakeStringAccessor (&PhyRxStatsCalculator::SetDlRxOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlRxOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlRxPhyStats.txt"),
                   MakeStringAccessor (&PhyRxStatsCalculator::SetUlRxOutputFilename),
                   MakeStringChecker ())
  ;
  return tid;
}

void
PhyRxStatsCalculator::SetUlRxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetUlOutputFilename (outputFilename);
}

std::string
PhyRxStatsCalculator::GetUlRxOutputFilename (void)
{
  return LteStatsCalculator::GetUlOutputFilename ();
}

void
PhyRxStatsCalculator::SetDlRxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetDlOutputFilename (outputFilename);
}

std::string
PhyRxStatsCalculator::GetDlRxOutputFilename (void)
{
  return LteStatsCalculator::GetDlOutputFilename ();
}

void
PhyRxStatsCalculator::DlPhyReception (PhyReceptionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi << params.m_correctness);
  NS_LOG_INFO ("Write DL Rx Phy Stats in " << GetDlRxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_dlRxFirstWrite == true )
    {
      outFile.open (GetDlRxOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlRxOutputFilename ().c_str ());
          return;
        }
      m_dlRxFirstWrite = false;
      outFile << "% time\tcellId\tIMSI\tRNTI\ttxMode\tlayer\tmcs\tsize\trv\tndi\tcorrect";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetDlRxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlRxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  outFile << (uint32_t) params.m_txMode << "\t";
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << "\t";
  outFile << (uint32_t) params.m_correctness << std::endl;

    /**Hacked****/
  if ( params.m_correctness != 1 ){
    // NS_LOG_UNCOND("Rx: Error DlRx frame at " << params.m_timestamp);
    totalErrorDlRx += params.m_size;
  }
  if ( params.m_ndi != 1 ){
    // NS_LOG_UNCOND("Rx: DlRx received retransmitted HARQ frame at " << params.m_timestamp);
  }
  else
    totalDlRx += params.m_size;

  outFile.close ();
}

void
PhyRxStatsCalculator::UlPhyReception (PhyReceptionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi << params.m_correctness);
  NS_LOG_INFO ("Write UL Rx Phy Stats in " << GetUlRxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_ulRxFirstWrite == true )
    {
      outFile.open (GetUlRxOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlRxOutputFilename ().c_str ());
          return;
        }
      m_ulRxFirstWrite = false;
      outFile << "% time\tcellId\tIMSI\tRNTI\ttxMode\tlayer\tmcs\tsize\trv\tndi\tcorrect";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetUlRxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlRxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  outFile << (uint32_t) params.m_txMode << "\t";
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << "\t";
  outFile << (uint32_t) params.m_correctness << std::endl;
    /**Hacked****/
  if ( params.m_correctness != 1 ){
    // NS_LOG_UNCOND("Rx: Error UlRx frame at " << params.m_timestamp);
    totalErrorUlRx += params.m_size;
  }
  if ( params.m_ndi != 1 ){
    // NS_LOG_UNCOND("Rx: UlRx received retransmitted HARQ frame at " << params.m_timestamp);
  }
  else
    totalUlRx += params.m_size;
  outFile.close ();
}

/**Hacked****/
uint64_t 
PhyRxStatsCalculator::GetTotalUl(){
  return totalUlRx;
}

uint64_t 
PhyRxStatsCalculator::GetTotalDl(){
  return totalDlRx;
}

uint64_t
PhyRxStatsCalculator::GetTotalErrorUl(){
  return totalErrorUlRx;
}
uint64_t 
PhyRxStatsCalculator::GetTotalErrorDl(){
  return totalErrorDlRx;
}


} // namespace ns3
