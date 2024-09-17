#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto GBN = OTAGRUM::GaussianBayesianNetwork();

  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  auto idX = GBN.addVariable(X);

  auto Y = OTAGRUM::GaussianVariable("Y", "Another one !", 2., .25);
  auto idY = GBN.addVariable(Y);

  auto idZ = GBN.addVariable("Z", 0, 2);

  GBN.addArc(idX,idY, 1.);
  GBN.addArc(idZ,idY, 2.);

  std::cout << GBN << std::endl;
  std::cout << GBN.getDAG() << std::endl;
  std::cout << "Topological order : " << GBN.getTopologicalOrder() << std::endl;

  std::cout << "Mu 0: " << GBN.getMu(0) << std::endl;
  GBN.changeMu(0, .5);
  std::cout << "Mu 0: " << GBN.getMu(0) << std::endl;

  std::cout << "Sigma 0: " << GBN.getSigma(1) << std::endl;
  GBN.changeSigma(0, 3);
  std::cout << "Sigma 0: " << GBN.getSigma(1) << std::endl;

  std::cout << "Weight of arc X->Y : " << GBN.getWeight(idX, idY) << std::endl;

  std::cout << GBN.getVariable(idX) << std::endl;

}

int main(void)
{
  testConstructor();

  return 0;
}
