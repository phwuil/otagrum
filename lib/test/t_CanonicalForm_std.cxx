#include <iostream>
#include <Eigen/Dense>

#include "otagrum/otagrum.hxx"


void testConstructor()
{
  auto X = OTAGRUM::GaussianVariable("X", "A variable", 1., .5);
  auto Y = OTAGRUM::GaussianVariable("Y", "Another one !", 2., .25);
  auto Z = OTAGRUM::GaussianVariable("Z", "", .5, 1.);
  auto W = OTAGRUM::GaussianVariable("W", "", .0, 1.);

  // Test of constructor from 1D Gaussian
  OTAGRUM::CanonicalForm cf_from_gaussian(X);
  std::cout << "CF from Gaussian : " << cf_from_gaussian << std::endl;

  // Test of constructor from CLG
  OTAGRUM::CanonicalForm cf_from_clg(Z, {X,Y}, {1, 2});
  std::cout << "CF from CLG : " << cf_from_clg << std::endl;

  // Test of empty canonical form constructor
  OTAGRUM::CanonicalForm cf0; 
  std::cout << "cf0 : " << cf0 << std::endl;

  // Testing parameter constructor
  OTAGRUM::Scope scope1 = {X, Y};
  std::cout << "scope1 : " << scope1 << std::endl;

  Eigen::MatrixXd K1(2,2);
  K1 <<  1, -1,
        -1,  1;
  std::cout << "K1 = " << std::endl << K1 << std::endl;

  Eigen::VectorXd h1(2,1);
  h1 <<  1,
        -1;
  std::cout << "h1 = " << std::endl << h1 << std::endl;

  double g1 = -3;
  std::cout << "g1 = " << std::endl << g1 << std::endl;

  OTAGRUM::CanonicalForm cf1(scope1, K1, h1, g1);
  std::cout << "cf1 : " << cf1 << std::endl;
  std::cout << std::endl << std::endl;

  OTAGRUM::Scope scope2 = {Y, Z};
  std::cout << "scope2 : " << scope2 << std::endl;

  Eigen::MatrixXd K2(2,2);
  K2 <<  3, -2,
        -2,  4;
  std::cout << "K2 = " << std::endl << K2 << std::endl;

  Eigen::VectorXd h2(2,1);
  h2 <<  5,
        -1;
  std::cout << "h2 = " << std::endl << h2 << std::endl;

  double g2 = 1;
  std::cout << "g2 = " << std::endl << h2 << std::endl;

  OTAGRUM::CanonicalForm cf2(scope2, K2, h2, g2);
  std::cout << "cf2 : " << cf2 << std::endl;
  std::cout << std::endl << std::endl;

  // Testing multiplication with empty canonical form
  std::cout << "cf0 * cf1 = " << cf0 * cf1 << std::endl;
  std::cout << "cf1 * cf0 = " << cf1 * cf0 << std::endl;
  std::cout << std::endl << std::endl;

  // Example 14.1 p610 of Koller & Friedman
  std::cout << "cf1 * cf2 = " << cf1 * cf2 << std::endl;
  std::cout << "cf2 * cf1 = " << cf2 * cf1 << std::endl;
  std::cout << std::endl << std::endl;

  // Testing division with empty canonical form
  std::cout << "cf1 / cf0 = " << cf1 / cf0 << std::endl;
  std::cout << "cf0 / cf1 = " << cf0 / cf1 << std::endl;
  std::cout << std::endl << std::endl;

  // Testing division
  std::cout << "cf1 / cf2 = " << cf1 / cf2 << std::endl;
  std::cout << "cf2 / cf1 = " << cf2 / cf1 << std::endl;
  std::cout << std::endl << std::endl;
}

int main(void)
{
  testConstructor();

  return 0;
}
