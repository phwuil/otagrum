#include <iostream>
#include <Eigen/LU>
#include "otagrum/LinearGaussian.hxx"

using namespace std;

ostream& operator<<(ostream& os, const LinearGaussian& item)
{
    os << "uncond_scope = " << endl << item.uncond_scope << endl
       << "cond_scope" << endl << item.cond_scope << endl
       << "Sigma = " << endl << item.Sigma << endl
       << "B = " << endl << item.B << endl
       << "mu = " << endl << item.mu << endl
       << "c = " << endl << item.c << endl;
    return os;
}

//LinearGaussian::LinearGaussian(CanonicalForm CF)
//{
    //Sigma = CF.K.inverse();
    //mu = Sigma * CF.h;
    //c = exp( CF.g + 0.5*CF.h.transpose()*mu );
//}
