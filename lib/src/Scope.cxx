#include <algorithm>
#include <map>
#include <iostream>
#include <set>

#include "otagrum/Scope.hxx"

using namespace std;

Scope::Scope(std::initializer_list<ContinuousVariable> l): variables(l),
                                                           size(l.size()){
    sort(this->variables.begin(), this->variables.end());
}


bool Scope::contains(const ContinuousVariable& item) const {
    if( find(variables.begin(), variables.end(), item) != variables.end() ){
        return true;
    }
    else{
        return false;
    }
}

int Scope::addVariable(ContinuousVariable variable){
    if(this->contains(variable)){
        return -1;
    } // Si la variable est déjà dans le scope on retourne -1


    auto it = variables.insert (
                  std::upper_bound(variables.begin(), variables.end(), variable),
                  variable
              );

    int position = it - variables.begin();
    
    size++;

    return position;
}

Scope::Scope(vector<ContinuousVariable>& variables): variables(variables),
                                                     size(variables.size()){
    sort(this->variables.begin(), this->variables.end());
}

ostream& operator<<(ostream& os, const Scope& item){
    if(item.variables.empty()){
        os << "[]" << endl;
    }
    else{
        os << "[";
        auto it = item.variables.begin();
        for(; it != item.variables.end()-1; ++it){
            os << *it << ", ";
        }
        cout << *it << "]";
    }
    
    return os;
}

Scope& Scope::operator+=(const Scope &rhs){
    for(auto x : rhs.variables){
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
    for(auto x : rhs.variables){
        auto it = find(variables.begin(), variables.end(), x);
        if(it != variables.end()){
            variables.erase(it);
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
    return lhs.variables == rhs.variables;
}

bool operator!=(const Scope &lhs, const Scope &rhs){
    return !(lhs == rhs);
}
