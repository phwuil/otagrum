#include <iostream>

#include "otagrum/ContinuousVariable.hxx"

using namespace std;

int ContinuousVariable::count = 0;

ostream& operator<<(ostream& os, const ContinuousVariable& item){
    os << item.name;
    return os;
}

bool operator==(const ContinuousVariable &lhs, const ContinuousVariable &rhs){
    return lhs.id == rhs.id;
}
bool operator<(const ContinuousVariable &lhs, const ContinuousVariable &rhs){
    return lhs.id < rhs.id;
}
bool operator<=(const ContinuousVariable &lhs, const ContinuousVariable &rhs){
    return lhs.id < rhs.id ||
           lhs.id == rhs.id;
}
bool operator>(const ContinuousVariable &lhs, const ContinuousVariable &rhs){
    return !(lhs<=rhs);
}
bool operator>=(const ContinuousVariable &lhs, const ContinuousVariable &rhs){
    return !(lhs<rhs);
}
