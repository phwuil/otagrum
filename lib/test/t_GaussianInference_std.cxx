#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto GBN = OTAGRUM::GaussianBayesianNetwork();

  GBN.addVariable("E", 50, 10);
  GBN.addVariable("G", 50, 10);
  GBN.addVariable("V", -10.35534, 5);
  GBN.addVariable("N", 45, 9.949874);
  GBN.addVariable("W", 15, 7.141428);
  GBN.addVariable("C", 0, 6.25);

  GBN.addArc("E","V", 0.7711);
  GBN.addArc("G","V", 0.5);
  GBN.addArc("V","N", 0.1);
  GBN.addArc("V","W", 0.7);
  GBN.addArc("N","C", 0.3);
  GBN.addArc("W","C", 0.3);

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
