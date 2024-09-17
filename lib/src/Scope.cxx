#include <algorithm>
#include <map>
#include <iostream>
#include <set>

#include "otagrum/Scope.hxx"

using namespace std;

namespace OTAGRUM {

Scope::Scope(std::initializer_list<GaussianVariable> l): _variables_(l),
                                                         _size_(l.size()) {}

Scope::Scope(vector<GaussianVariable>& variables): _variables_(variables),
                                                   _size_(variables.size()){}


bool Scope::contains(const GaussianVariable& item) const {
    if( find(_variables_.begin(), _variables_.end(), item) != _variables_.end() ){
        return true;
    }
    else{
        return false;
    }
}

void Scope::addVariable(GaussianVariable variable){
    if(!contains(variable)) {
        _variables_.push_back(variable);
        _size_++;
    }
}

void Scope::addVariables(vector<GaussianVariable> variables){
    for(const auto& variable: variables){
        addVariable(variable);
    }
}

void Scope::addVariable(GaussianVariable variable, unsigned int pos){
    if(!contains(variable)) {
        _variables_.insert(_variables_.begin() + pos, variable);
        _size_++;
    }
}

void Scope::eraseVariable(GaussianVariable variable){
    auto it = find(_variables_.begin(), _variables_.end(), variable);
    if(it != _variables_.end()){
        _variables_.erase(it);
    }
}


ostream& operator<<(ostream& os, const Scope& item){
    if(item._variables_.empty()){
        os << "[]";
    }
    else{
        os << "[";
        auto it = item._variables_.begin();
        for(; it != item._variables_.end()-1; ++it){
            os << *it << ", ";
        }
        cout << *it << "]";
    }
    
    return os;
}

Scope& Scope::operator+=(const Scope &rhs){
    for(auto x : rhs._variables_){
        addVariable(x);
    }
    return *this;
}

Scope& Scope::operator+=(const GaussianVariable &variable){
    addVariable(variable);
    return *this;
}

Scope operator+(const Scope &lhs, const Scope &rhs){
    Scope sum = lhs;
    sum += rhs;
    return sum;
}

Scope& Scope::operator-=(const Scope &rhs){
    for(auto var : rhs._variables_){
        eraseVariable(var);
    }
    return *this;
}

Scope& Scope::operator-=(const GaussianVariable &variable){
    eraseVariable(variable);
    return *this;
}

Scope operator-(const Scope &lhs, const Scope &rhs){
    Scope diff = lhs;
    diff -= rhs;
    return diff;
}

bool operator==(const Scope &lhs, const Scope &rhs){
    return lhs._variables_ == rhs._variables_;
}

bool operator!=(const Scope &lhs, const Scope &rhs){
    return !(lhs == rhs);
}

} //namespace OTAGRUM
