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

#ifndef PHY_TX_STATS_CALCULATOR_H_
#define PHY_TX_STATS_CALCULATOR_H_

#include "ns3/lte-stats-calculator.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include <string>
#include <fstream>
#include <ns3/lte-common.h>

namespace ns3 {



/**
 * Takes care of storing the information generated at PHY layer regarding 
 * transmission. Metrics saved are:
 *time\tframe\tsframe\tRNTI\tmcsTb1\tsizeTb1\tmcsTb2\tsizeTb2
 *   - Timestamp (in seconds)
 *   - Frame index
 *   - Subframe index
 *   - C-RNTI
 *   - MCS for transport block 1
 *   - Size of transport block 1
 *   - MCS for transport block 2 (0 if not used)
 *   - Size of transport block 2 (0 if not used)
 */
typedef std::map<double,double> Time_cap; /*<time stamp, link cap> map*/

class PhyTxStatsCalculator : public LteStatsCalculator
{
public:
  /**
   * Constructor
   */
  PhyTxStatsCalculator ();

  /**
   * Destructor
   */
  virtual ~PhyTxStatsCalculator ();

  /**
   * Inherited from ns3::Object
   */
  static TypeId GetTypeId (void);

  /**
   * Set the name of the file where the UL Tx PHY statistics will be stored.
   *
   * \param outputFilename string with the name of the file
   */
  void SetUlTxOutputFilename (std::string outputFilename);

  /**
   * Get the name of the file where the UL RX PHY statistics will be stored.
   */
  std::string GetUlTxOutputFilename (void);

  /**
   * Set the name of the file where the DL TX PHY statistics will be stored.
   *
   * @param outputFilename string with the name of the file
   */
  void SetDlTxOutputFilename (std::string outputFilename);

  /**
   * Get the name of the file where the DL TX PHY statistics will be stored.
   */
  std::string GetDlTxOutputFilename (void);

  /**
   * Notifies the stats calculator that an downlink trasmission has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param layer the layer (cw) of the transmission
   * @param txMode the transmission Mode
   * @param mcs MCS for transport block
   * @param size Size of transport block
   * @param rv the redundancy version (HARQ)
   * @param ndi new data indicator flag
   */
  void DlPhyTransmission (PhyTransmissionStatParameters params);

  /**
   * Notifies the stats calculator that an uplink trasmission has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param layer the layer (cw) of the transmission
   * @param txMode the transmission Mode
   * @param mcs MCS for transport block
   * @param size Size of transport block
   * @param rv the redundancy version (HARQ)
   * @param ndi new data indicator flag
   */
  void UlPhyTransmission (PhyTransmissionStatParameters params);

    /**Hacked***/
  uint64_t GetTotalUl();
  uint64_t GetTotalDl();
  // uint64_t GetTotalErrorUl();
  // uint64_t GetTotalErrorDl();
  uint64_t GetTotalUlHarqRetransmission();
  uint64_t GetTotalDlHarqRetransmission();
  std::map<uint32_t, Time_cap> GetUlCap();
  std::map<uint32_t, Time_cap> GetDlCap();

private:

  bool m_dlTxFirstWrite;
  bool m_ulTxFirstWrite;

  uint64_t totalUlTx;
  uint64_t totalDlTx;
  // uint64_t totalErrorUlTx;
  // uint64_t totalErrorDlTx;
  uint64_t totalUlHarqRetransmission;
  uint64_t totalDlHarqRetransmission;

  /*map: <mcs index, link capacity>*/
  std::map<uint32_t, double> mcs_cap_100_single; /*100RBs, single antenna*/
  std::map<uint32_t, double> mcs_cap_100_double; /*100RBs, double antenna*/
  void init_mcs_map();

  /*capacity map in time line*/
  std::map<uint32_t, Time_cap> time_ulcap;  /*<imsi, time-capacity> map for uplinks*/
  std::map<uint32_t, Time_cap> time_dlcap;  /*<imsi, time-capacity> map for downlinks*/
  Time_cap t_c; /*just temporary a <time, capacity> map, meaningless*/
  std::map<uint32_t,double> last_sampling_time_dl; /*<imsi, last_sampling_time_dl> map: sampling time of each users*/
  std::map<uint32_t,double> last_sampling_time_ul;
  double sampling_interval; /*link capacity sampling interval, in ms*/
};

} // namespace ns3

#endif /* PHY_TX_STATS_CALCULATOR_H_ */
