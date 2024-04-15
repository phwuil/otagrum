#include <iostream>
#include <openturns/IdentityMatrix.hxx>
#include <openturns/SquareMatrix.hxx>
#include <openturns/Point.hxx>
#include "otagrum/Scope.hxx"
#include "otagrum/Gaussian.hxx"

#include "otagrum/CanonicalForm.hxx"

using namespace std;
using namespace OT;

ostream& operator<<(ostream& os, const Gaussian& item)
{
    os << "scope = " << endl << item.scope << endl
       << "Sigma = " << endl << item.Sigma << endl
       << "mu = " << endl << item.mu << endl
       << "c = " << endl << item.c << endl
       << "p = " << endl << item.p << endl;
    return os;
}

Gaussian::Gaussian(Scope scope, SquareMatrix Sigma, Point mu): 
                          scope(scope), Sigma(Sigma), mu(mu) {
    c = pow( pow(2*M_PI,scope.getSize()) * Sigma.computeDeterminant(), -0.5 );
}

Gaussian::Gaussian(CanonicalForm CF)
{
    scope = CF.scope;
    Sigma = SquareMatrix(CF.K.solveLinearSystem(IdentityMatrix(scope.getSize())).getImplementation());
    mu = Sigma * CF.h;
    c = exp( CF.g + 0.5*CF.h.dot(mu) );
    p = exp(CF.lp);
}

Gaussian& Gaussian::operator+=(const Gaussian &rhs){
    if(Sigma.isEmpty() && rhs.Sigma.isEmpty()){
        p += rhs.p;
    }
    else{
        if(scope != rhs.scope){
            cout << "Erreur : les scopes sont différents :" << endl;
        }
        Point new_mu = p*mu + rhs.p*rhs.mu;

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
