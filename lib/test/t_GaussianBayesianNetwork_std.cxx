#include <iostream>

#include "otagrum/otagrum.hxx"

void testConstructor()
{
  // Parameters of the GBN
  double mu1 = 1.;
  double mu2 = -3.5;
  double mu3 = 1.;

  double sigma1 = 4.;
  double sigma2 = 4.;
  double sigma3 = 3;

  double b21 = 0.5;
  double b32 = -1.;

  // Creating the GBN
  auto GBN = OTAGRUM::GaussianBayesianNetwork();

  auto X1 = OTAGRUM::GaussianVariable("X1", "A variable", .5, sigma1);
  auto idX1 = GBN.addVariable(X1);

  auto X2 = OTAGRUM::GaussianVariable("X2", "Another one !", mu2, 3);
  auto idX2 = GBN.addVariable(X2);

  auto idX3 = GBN.addVariable("X3", mu3, sigma3);

  GBN.addArc(idX1, idX2, b21);
  GBN.addArc(idX2, idX3, b32);

  std::cout << GBN << std::endl;
  std::cout << GBN.getDAG() << std::endl;
  std::cout << "Topological order : " << GBN.getTopologicalOrder() << std::endl;

  std::cout << "Mu X1: " << GBN.getMu(idX1) << std::endl;
  GBN.changeMu(idX1, mu1);
  std::cout << "Mu X1: " << GBN.getMu(idX1) << std::endl;

  std::cout << "Sigma X2: " << GBN.getSigma(idX2) << std::endl;
  GBN.changeSigma(idX2, sigma2);
  std::cout << "Sigma X2: " << GBN.getSigma(idX2) << std::endl;

  std::cout << "Weight of arc X1->X2 : " << GBN.getWeight(idX1, idX2) << std::endl;

  std::cout << GBN.getVariable(idX1) << std::endl;

  auto normal_distribution = GBN.buildNormalDistribution();
  std::cout << "Building Gaussian from GBN: " << normal_distribution.getDescription()
                                              << std::endl
                                              << normal_distribution
                                              << std::endl
                                              << std::endl;

  std::cout << "Covariance Matrix: " << std::endl
                                     << normal_distribution.getCovariance()
                                     << std::endl
                                     << std::endl;

  //OT::CovarianceMatrix theoretical_cov_mat(3);
  //theoretical_cov_mat(0,0) = sigma2 * sigma2;
  //theoretical_cov_mat(0,1) = 0.;
  //theoretical_cov_mat(0,2) = b32*sigma2*sigma2;
  //theoretical_cov_mat(1,1) = sigma1*sigma1;
  //theoretical_cov_mat(1,2) = b31*sigma1*sigma1;
  //theoretical_cov_mat(2,2) = b31*b31*sigma1*sigma1 + b32*b32*sigma2*sigma2 + sigma3*sigma3;

  OT::CovarianceMatrix theoretical_cov_mat(3);
  theoretical_cov_mat(0,0) = sigma1*sigma1;
  theoretical_cov_mat(0,1) = b21*sigma1*sigma1;
  theoretical_cov_mat(0,2) = b21*b32*sigma1*sigma1;
  theoretical_cov_mat(1,1) = b21*b21*sigma1*sigma1 + sigma2*sigma2;
  theoretical_cov_mat(1,2) = b32*(b21*b21*sigma1*sigma1 + sigma2*sigma2);
  theoretical_cov_mat(2,2) = b21*b21*b32*b32*sigma1*sigma1 + b32*b32*sigma2*sigma2 + sigma3*sigma3;

  std::cout << "Theoretical covariance Matrix: " << std::endl
                                                 << theoretical_cov_mat << std::endl;

  std::cout << "A sample: " << GBN.getSample(10) << std::endl;

  auto gbn_sample = GBN.getSample(100000);
  std::cout << "Order: " << gbn_sample.getDescription() << std::endl;
  std::cout << "Mean and Covariance of sample of size 100000 from GBN:"
            << std::endl
            << gbn_sample.computeMean()
            << std::endl
            << gbn_sample.computeCovariance()
            << std::endl;

  auto gaussian_sample = normal_distribution.getSample(100000);
  std::cout << "Order: " << gaussian_sample.getDescription() << std::endl;
  std::cout << "Mean and Covariance of sample of size 100000 from gaussian:"
            << std::endl
            << gaussian_sample.computeMean()
            << std::endl
            << gaussian_sample.computeCovariance()
            << std::endl;

  std::cout << "Theoretical covariance Matrix: " << std::endl
                                                 << theoretical_cov_mat << std::endl;
}

int main(void)
{
  testConstructor();

  return 0;
}
