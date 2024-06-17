#include <iostream>
#include <string>

#include "otagrum/GaussianVariable.hxx"

using namespace std;

namespace OTAGRUM {

GaussianVariable* GaussianVariable::clone() const { return new GaussianVariable(*this); }

void GaussianVariable::setMu(double mu){
    _mu_ = mu;
}
void GaussianVariable::setSigma(double sigma){
    _sigma_ = sigma;
}

std::string GaussianVariable::toString() const {
    std::ostringstream stream;
    stream << name() << '(' << _mu_ << ',' << _sigma_ << ')';
    return stream.str();
}

} // namespace OTAGRUM
