#include <iostream>

#include <openturns/OT.hxx>

#include "otagrum/ContinuousBayesianNetworkFactory.hxx"
#include "otagrum/ContinuousMIIC.hxx"
#include "otagrum/ContinuousPC.hxx"
#include "otagrum/TabuList.hxx"
#include "otagrum/NamedDAG.hxx"

std::string dirname()
{
  std::string res = __FILE__;
  const size_t last_slash_idx = res.find_last_of("\\/");
  if (std::string::npos != last_slash_idx)
  {
    res.erase(last_slash_idx + 1);
  }
  return res;
}

void testGaussian()
{
  const auto sample =
    OT::Sample::ImportFromCSVFile(dirname() + "gaussian_sample_5000.csv");
  //auto learner = OTAGRUM::ContinuousPC(sample);
  //auto learner = OTAGRUM::TabuList(sample);
  auto learner = OTAGRUM::ContinuousMIIC(sample);
  auto cmt = OTAGRUM::CorrectedMutualInformation::CModeTypes::Gaussian;
  learner.setCMode(cmt);
  learner.setVerbosity(true);
  auto dag = learner.learnDAG();
  OT::Collection< OT::DistributionFactory > factories(1);
  factories[0] = OT::NormalCopulaFactory();
  OTAGRUM::ContinuousBayesianNetworkFactory CBNFactory(factories, dag, 0, 0, true);
  auto cbn = CBNFactory.buildAsContinuousBayesianNetwork(sample);
  std::cout << "Learned CBN: " << cbn << std::endl;
}

int main(void)
{
  testGaussian();

  return 0;
}
