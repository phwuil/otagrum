#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto factory = OTAGRUM::GaussianBayesianNetworkFactory();
  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  auto Y = OTAGRUM::GaussianVariable("Y", "Another one !", 2., .25);
  auto idX = factory.addVariable(X);
  auto idY = factory.addVariable(Y);
  auto idZ = factory.addVariable("Z", 0, 2);
  factory.addArc(idX,idY, 1.);
  factory.addArc(idZ,idY, 2.);
  std::cout << factory << std::endl;

  std::cout << "Mu 0: " << factory.getMu(0) << std::endl;
  factory.changeMu(0, .5);
  std::cout << "Mu 0: " << factory.getMu(0) << std::endl;

  std::cout << "Sigma 0: " << factory.getSigma(0) << std::endl;
  factory.changeSigma(0, 3);
  std::cout << "Sigma 0: " << factory.getSigma(0) << std::endl;
}

int main(void)
{
  testConstructor();

  return 0;
}
