#include <iostream>

#include <openturns/OT.hxx>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  std::cout << "      X : " << X << std::endl;
  std::cout << "      X : " << X.toString() << std::endl;
  std::cout << "       domain : " << X.domain() << std::endl;
  std::cout << "       mu : " << X.getMu() << std::endl;
  std::cout << "       sigma : " << X.getSigma() << std::endl;
}

int main(void)
{
  testConstructor();

  return 0;
}
