#include <iostream>

#include "otagrum/otagrum.hxx"


void testConstructor()
{
  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  auto Y = OTAGRUM::GaussianVariable("Y", "Another one !", 2., .25);
  auto Z = OTAGRUM::GaussianVariable("Z", "", .5, 1.);
  auto W = OTAGRUM::GaussianVariable("W", "", .0, 1.);

  OTAGRUM::Scope scope1;
  scope1.addVariable(X);
  scope1 += Y;
  scope1 += {W, W, Z};

  std::cout << "scope1 : " << scope1 << std::endl;

  scope1.eraseVariable(W);
  std::cout << "scope1 : " << scope1 << std::endl;

  scope1.addVariable(W, 1);
  std::cout << "scope1 : " << scope1 << std::endl;
  scope1.eraseVariable(W);

  OTAGRUM::Scope scope2({X, Z, Y});

  std::cout << "Equal scope ? : " << (scope1+scope2 == scope2+scope1) << std::endl;

  std::cout << "Difference : " << (scope1 - scope2) << std::endl;
  std::cout << "Difference : " << (scope2 - scope1) << std::endl;

  std::cout << "Contains : " << scope1.contains(X) << std::endl;

  scope2 -= Y;
  scope2 -= {Z, X};
  std::cout << "Empty : " << scope2.isEmpty() << std::endl;

}

int main(void)
{
  testConstructor();

  return 0;
}
