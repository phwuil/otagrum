#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  auto Y = OTAGRUM::GaussianVariable("Y", "Another one !", 2., .25);

  auto vnm = OTAGRUM::VariableNodeMap();

  std::cout << vnm << std::endl;

  vnm.insert(0, X);
  vnm.insert(1, Y);

  std::cout << vnm << std::endl;
}

int main(void)
{
  testConstructor();

  return 0;
}
