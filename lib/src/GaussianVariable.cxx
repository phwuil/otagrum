#include <iostream>

#include "otagrum/GaussianVariable.hxx"

using namespace std;

int GaussianVariable::_count_ = 0;

ostream& operator<<(ostream& os, const GaussianVariable& item){
    os << item._name_;
    return os;
}

GaussianVariable* GaussianVariable::clone() const { return new GaussianVariable(*this); }

bool operator==(const GaussianVariable &lhs, const GaussianVariable &rhs){
    return lhs._id_ == rhs._id_;
}
bool operator<(const GaussianVariable &lhs, const GaussianVariable &rhs){
    return lhs._id_ < rhs._id_;
}
bool operator<=(const GaussianVariable &lhs, const GaussianVariable &rhs){
    return lhs._id_ < rhs._id_ ||
           lhs._id_ == rhs._id_;
}
bool operator>(const GaussianVariable &lhs, const GaussianVariable &rhs){
    return !(lhs<=rhs);
}
bool operator>=(const GaussianVariable &lhs, const GaussianVariable &rhs){
    return !(lhs<rhs);
}

void GaussianVariable::setName(const std::string &new_name){
    _name_ = new_name;
}
void GaussianVariable::setMu(double mu){
    _mu_ = mu;
}
void GaussianVariable::setSigma(double sigma){
    _sigma_ = sigma;
}
