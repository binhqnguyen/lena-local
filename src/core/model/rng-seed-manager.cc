#include "rng-seed-manager.h"
#include "global-value.h"
#include "attribute-helper.h"
#include "integer.h"
#include "config.h"

namespace ns3 {

static uint64_t g_nextStreamIndex = 0;
static ns3::GlobalValue g_rngSeed ("RngSeed", 
                                   "The global seed of all rng streams",
                                   ns3::IntegerValue(1),
                                   ns3::MakeIntegerChecker<uint32_t> ());
static ns3::GlobalValue g_rngRun ("RngRun", 
                                  "The run number used to modify the global seed",
                                  ns3::IntegerValue (1),
                                  ns3::MakeIntegerChecker<int64_t> ());


uint32_t RngSeedManager::GetSeed (void)
{
  IntegerValue seedValue;
  g_rngSeed.GetValue (seedValue);
  return seedValue.Get ();
}
void 
RngSeedManager::SetSeed (uint32_t seed)
{
  Config::SetGlobal ("RngSeed", IntegerValue(seed));
}

void RngSeedManager::SetRun (uint64_t run)
{
  Config::SetGlobal ("RngRun", IntegerValue (run));
}

uint64_t RngSeedManager::GetRun ()
{
  IntegerValue value;
  g_rngRun.GetValue (value);
  int run = value.Get();
  return run;
}

uint64_t RngSeedManager::GetNextStreamIndex (void)
{
  uint64_t next = g_nextStreamIndex;
  g_nextStreamIndex++;
  return next;
}

} // namespace ns3
