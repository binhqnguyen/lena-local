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
 *        Convert MacStatsCalculator in PhyTxStatsCalculator
 */

#include "phy-tx-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PhyTxStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (PhyTxStatsCalculator);

PhyTxStatsCalculator::PhyTxStatsCalculator ()
  : m_dlTxFirstWrite (true),
    m_ulTxFirstWrite (true)
{
  NS_LOG_FUNCTION (this);
  totalUlTx = 0;
  totalDlTx = 0;
  totalUlHarqRetransmission = 0;
  totalDlHarqRetransmission = 0;
  sampling_interval = 50; /*sampling each 50ms*/
  init_mcs_map();
}

PhyTxStatsCalculator::~PhyTxStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
  /*hacked*/
  NS_LOG_UNCOND("Phy UlTx = " << totalUlTx);
  NS_LOG_UNCOND("Phy DlTx = " << totalDlTx);
}

TypeId
PhyTxStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PhyTxStatsCalculator")
    .SetParent<LteStatsCalculator> ()
    .AddConstructor<PhyTxStatsCalculator> ()
    .AddAttribute ("DlTxOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlTxPhyStats.txt"),
                   MakeStringAccessor (&PhyTxStatsCalculator::SetDlTxOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlTxOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlTxPhyStats.txt"),
                   MakeStringAccessor (&PhyTxStatsCalculator::SetUlTxOutputFilename),
                   MakeStringChecker ())
  ;
  return tid;
}

void
PhyTxStatsCalculator::SetUlTxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetUlOutputFilename (outputFilename);
}

std::string
PhyTxStatsCalculator::GetUlTxOutputFilename (void)
{
  return LteStatsCalculator::GetUlOutputFilename ();
}

void
PhyTxStatsCalculator::SetDlTxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetDlOutputFilename (outputFilename);
}

std::string
PhyTxStatsCalculator::GetDlTxOutputFilename (void)
{
  return LteStatsCalculator::GetDlOutputFilename ();
}

void
PhyTxStatsCalculator::DlPhyTransmission (PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi);
  NS_LOG_INFO ("Write DL Tx Phy Stats in " << GetDlTxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_dlTxFirstWrite == true )
    {
      outFile.open (GetDlOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlTxOutputFilename ().c_str ());
          return;
        }
      m_dlTxFirstWrite = false;
      //outFile << "% time\tcellId\tIMSI\tRNTI\tlayer\tmcs\tsize\trv\tndi"; // txMode is not available at dl tx side
      outFile << "% time\tcellId\tIMSI\tRNTI\ttxMode\tlayer\tmcs\tsize\trv\tndi";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetDlTxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlTxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  //outFile << (uint32_t) params.m_txMode << "\t"; // txMode is not available at dl tx side
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << std::endl;
  if (params.m_ndi == 1)
    totalDlTx += params.m_size;
  else{
    totalDlHarqRetransmission++;
    NS_LOG_UNCOND("*Tx: DlTx resend frame at " << params.m_timestamp);
  }
  t_c[params.m_timestamp] = mcs_cap_100_single[(uint32_t) params.m_mcs]; /*obtain link capacity through mcs_index*/
  if (params.m_timestamp > (last_sampling_time_dl[params.m_imsi] + sampling_interval)) /*take sample only after a sampling_interval of time*/
    time_dlcap[params.m_imsi] = t_c;  /*imsi - timecapacity map*/
  last_sampling_time_dl[params.m_imsi] = params.m_timestamp;
  outFile.close ();
}

void
PhyTxStatsCalculator::UlPhyTransmission (PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi);
  NS_LOG_INFO ("Write UL Tx Phy Stats in " << GetUlTxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_ulTxFirstWrite == true )
    {
      outFile.open (GetUlTxOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlTxOutputFilename ().c_str ());
          return;
        }
      m_ulTxFirstWrite = false;
//       outFile << "% time\tcellId\tIMSI\tRNTI\ttxMode\tlayer\tmcs\tsize\trv\tndi";
      outFile << "% time\tcellId\tIMSI\tRNTI\tlayer\tmcs\tsize\trv\tndi";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetUlTxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlTxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  //outFile << (uint32_t) params.m_txMode << "\t";
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << std::endl;

  /*Hacked*/
  if (params.m_ndi == 1)
    totalUlTx += params.m_size;
  else{
    totalUlHarqRetransmission++;
    NS_LOG_UNCOND("*Tx: UlTx resend frame at " << params.m_timestamp);
  }
  t_c[params.m_timestamp] = mcs_cap_100_single[(uint32_t) params.m_mcs]; /*obtain link capacity through mcs_index*/
  if (params.m_timestamp > (last_sampling_time_ul[params.m_imsi] + sampling_interval)) /*take sample only after a sampling_interval of time*/
    time_ulcap[params.m_imsi] = t_c;  /*imsi - timecapacity map*/
  last_sampling_time_ul[params.m_imsi] = params.m_timestamp;

  outFile.close ();
}

/**Hacked****/
uint64_t 
PhyTxStatsCalculator::GetTotalUl(){
  return totalUlTx;
}

uint64_t
PhyTxStatsCalculator::GetTotalDl(){
  return totalDlTx;
}

uint64_t
PhyTxStatsCalculator::GetTotalUlHarqRetransmission(){
  return totalUlHarqRetransmission;
}

uint64_t
PhyTxStatsCalculator::GetTotalDlHarqRetransmission(){
  return totalDlHarqRetransmission;
}

void
PhyTxStatsCalculator::init_mcs_map(){
  double siso[29] = { /*as in mcs-put-table.xls*/
    2.792, 3.624, 4.584, 5.736, 7.224, 8.76, 10.296, 12.216, 14.112, 15.84,
    15.84, 17.568, 19.848, 22.92, 25.456, 28.336, 30.576, 30.576, 32.856, 36.696, 39.232,
    43.816, 46.888, 51.024, 55.056, 57.336, 61.664, 63.776, 75.376
  };
  double mimo[29] = { /*as in mcs-put-table.xls examples*/
    5.584, 7.248, 9.168, 11.472, 14.448, 17.52, 20.592, 24.432, 28.224, 31.68,
    31.68, 35.136, 39.696, 45.84, 50.912, 56.672, 61.152, 61.152, 65.712, 73.392, 78.464,
    87.632, 93.776, 102.048, 110.112, 114.672, 123.328, 127.552, 150.752
  };
  for (uint32_t i=0; i < 29; ++i){
      mcs_cap_100_single[i] = siso[i];   /*100RBs, single attenna*/
      mcs_cap_100_double[i] = mimo[i];    /*100RBs, double attenna*/
  };
}

std::map<uint32_t,Time_cap>
PhyTxStatsCalculator::GetUlCap(){
  return time_ulcap;
}

std::map<uint32_t,Time_cap>
PhyTxStatsCalculator::GetDlCap(){
  return time_dlcap;
}


} // namespace ns3

