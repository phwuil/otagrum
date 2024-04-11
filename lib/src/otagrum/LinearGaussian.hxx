#ifndef LINEAR_GAUSSIAN_H
#define LINEAR_GAUSSIAN_H

#include <iostream>
#include <cmath>
//#include <Eigen/Dense>
//#include <Eigen/LU>
#include <openturns/Matrix.hxx>
#include <openturns/SquareMatrix.hxx>
#include <openturns/Point.hxx>

class LinearGaussian;
#include "CanonicalForm.hxx"

class LinearGaussian {
    friend class CanonicalForm;

    friend std::ostream& operator<<(std::ostream& os, const LinearGaussian& item);

    public:
        LinearGaussian(Scope uncond_scope,
                       Scope cond_scope,
                       OT::SquareMatrix Sigma,
                       OT::Matrix B,
                       OT::Point mu): uncond_scope(uncond_scope), 
                                            cond_scope(cond_scope),
                                            B(B), Sigma(Sigma), mu(mu){
            c = std::pow( std::pow( 2*M_PI,uncond_scope.getSize() )
                          *Sigma.computeDeterminant(), -0.5);
         }
        //LinearGaussian(CanonicalForm CF);

    private:
        Scope uncond_scope;
        Scope cond_scope;
        OT::SquareMatrix Sigma;
        OT::Matrix B;
        OT::Point mu;
        double c = 0.;
};

std::ostream& operator<<(std::ostream& os, const LinearGaussian& item);

#endif // LINEAR_GAUSSIAN_H
