#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto GBN = OTAGRUM::GaussianBayesianNetwork();

  auto idX = GBN.addVariable("X",  0,   1);
  auto idY = GBN.addVariable("Y",  1, 0.5);
  auto idZ = GBN.addVariable("Z", -1,   2);

  GBN.addArc(idX,idY, 1.);
  GBN.addArc(idZ,idY, 2.);

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
