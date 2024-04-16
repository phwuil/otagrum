#include <iostream>
#include <Eigen/LU>
#include <Eigen/Dense>
#include "otagrum/Scope.hxx"
#include "otagrum/Gaussian.hxx"

//#include "canonical_form.h"

using namespace std;
using namespace Eigen;

ostream& operator<<(ostream& os, const Gaussian& item)
{
    os << "scope = " << endl << item.scope << endl
       << "Sigma = " << endl << item.Sigma << endl
       << "mu = " << endl << item.mu << endl
       << "c = " << endl << item.c << endl
       << "p = " << endl << item.p << endl;
    return os;
}

Gaussian::Gaussian(Scope scope, MatrixXd Sigma, VectorXd mu): 
                          scope(scope), Sigma(Sigma), mu(mu) {
    c = pow( pow(2*M_PI,scope.getSize()) * Sigma.determinant(), -0.5 );
}

Gaussian::Gaussian(CanonicalForm CF)
{
    scope = CF.scope;
    Sigma = CF.K.inverse();
    mu = Sigma * CF.h;
    c = exp( CF.g + 0.5*CF.h.transpose()*mu );
    p = exp(CF.lp);
}

Gaussian& Gaussian::operator+=(const Gaussian &rhs){
    if(!(Sigma.size()) && !(rhs.Sigma.size())){
        p += rhs.p;
    }
    else{
        if(scope != rhs.scope){
            cout << "Erreur : les scopes sont différents :" << endl;
        }
        VectorXd new_mu = p*mu + rhs.p*rhs.mu;

        Sigma =  p*Sigma + rhs.p*rhs.Sigma
               + p*(mu - new_mu)*(mu - new_mu).transpose()
               + rhs.p*(rhs.mu - new_mu)*((rhs.mu - new_mu).transpose());
        mu = new_mu;
        //p += rhs.p;
        p = 1.;

    }
    return *this;
}

Gaussian operator+(const Gaussian &lhs, const Gaussian &rhs){
    Gaussian sum = lhs;
    sum += rhs;
    return sum;
}
