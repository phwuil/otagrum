#include <algorithm>
#include <map>
#include <iostream>
#include <set>

#include "otagrum/Scope.hxx"

using namespace std;

Scope::Scope(std::initializer_list<GaussianVariable> l): _variables_(l),
                                                           _size_(l.size()){
    sort(this->_variables_.begin(), this->_variables_.end());
}


bool Scope::contains(const GaussianVariable& item) const {
    if( find(_variables_.begin(), _variables_.end(), item) != _variables_.end() ){
        return true;
    }
    else{
        return false;
    }
}

int Scope::addVariable(GaussianVariable variable){
    if(this->contains(variable)){
        return -1;
    } // Si la variable est déjà dans le scope on retourne -1


    auto it = _variables_.insert (
                  std::upper_bound(_variables_.begin(), _variables_.end(), variable),
                  variable
              );

    int position = it - _variables_.begin();
    
    _size_++;

    return position;
}

Scope::Scope(vector<GaussianVariable>& variables): _variables_(variables),
                                                     _size_(variables.size()){
    sort(this->_variables_.begin(), this->_variables_.end());
}

ostream& operator<<(ostream& os, const Scope& item){
    if(item._variables_.empty()){
        os << "[]" << endl;
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
        this->addVariable(x);
    }
    return *this;
}

Scope operator+(const Scope &lhs, const Scope &rhs){
    Scope sum = lhs;
    sum += rhs;
    return sum;
}

Scope& Scope::operator-=(const Scope &rhs){
    for(auto x : rhs._variables_){
        auto it = find(_variables_.begin(), _variables_.end(), x);
        if(it != _variables_.end()){
            _variables_.erase(it);
        }
    }
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
