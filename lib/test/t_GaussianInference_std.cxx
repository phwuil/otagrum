#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto GBN = OTAGRUM::GaussianBayesianNetwork();

  auto idW = GBN.addVariable("W");
  auto idX = GBN.addVariable("X");
  auto idY = GBN.addVariable("Y");
  auto idZ = GBN.addVariable("Z");

  GBN.addArc(idW,idY, 1.);
  GBN.addArc(idX,idY, 2.);

  GBN.addArc(idX,idZ, 2.);
  GBN.addArc(idW,idZ, 1.);

  std::cout << GBN << std::endl;
  std::cout << GBN.getDAG() << std::endl;

  auto GI = OTAGRUM::GaussianInference(GBN);
  std::cout << "GI : " << GI << std::endl;

}

int main(void)
{
  testConstructor();

  return 0;
}
