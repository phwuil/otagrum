#ifndef CANONICAL_FORM_H
#define CANONICAL_FORM_H

#include <iostream>
#include <map>
#include <cmath>
#include <openturns/SquareMatrix.hxx>
#include <openturns/Point.hxx>
#include <agrum/tools/core/hashTable.h>
#include <agrum/tools/variables/labelizedVariable.h>

class CanonicalForm;
#include "Scope.hxx"
#include "Gaussian.hxx"
#include "LinearGaussian.hxx"

typedef std::vector<std::pair<ContinuousVariable, double> > ContinuousEvidence;
typedef std::vector<std::pair<gum::LabelizedVariable*, std::string> > DiscreteEvidence;

class CanonicalForm {
    friend class Gaussian;

    friend std::ostream& operator<<(std::ostream& os, const CanonicalForm& item);

    friend CanonicalForm operator+(const CanonicalForm &lhs,
                                   const CanonicalForm &rhs);
    friend CanonicalForm operator-(const CanonicalForm &lhs,
                                   const CanonicalForm &rhs);
    friend CanonicalForm operator*(const CanonicalForm &lhs,
                                   const CanonicalForm &rhs);
    friend CanonicalForm operator/(const CanonicalForm &lhs,
                                   const CanonicalForm &rhs);

    friend bool operator==(const CanonicalForm &lhs, const CanonicalForm &rhs);
    friend bool operator!=(const CanonicalForm &lhs, const CanonicalForm &rhs);
    friend bool operator<(const CanonicalForm &lhs, const CanonicalForm &rhs);
    friend bool operator<=(const CanonicalForm &lhs, const CanonicalForm &rhs);
    friend bool operator>(const CanonicalForm &lhs, const CanonicalForm &rhs);
    friend bool operator>=(const CanonicalForm &lhs, const CanonicalForm &rhs);

    public:
        // Constructeurs
        CanonicalForm() = default;
        CanonicalForm(Gaussian G);
        CanonicalForm(LinearGaussian LG);
        CanonicalForm(double p): lp(std::log(p)) {}
        CanonicalForm(Scope scope,
                      OT::SquareMatrix K,
                      OT::Point h,
                      double g): scope(scope), K(K), h(h), g(g) {}

        // Constructeur de move
        CanonicalForm(CanonicalForm &&item) = default;
        // Operateur de move
        CanonicalForm& operator=(CanonicalForm &&rhs) = default;

        // Constructeur de copy
        CanonicalForm(const CanonicalForm &item) = default;
        // Operateur de copy
        CanonicalForm& operator=(const CanonicalForm &rhs) = default;

        // Destructeur
        ~CanonicalForm() {};

        CanonicalForm& operator+=(const CanonicalForm &rhs);
        CanonicalForm& operator-=(const CanonicalForm &rhs);
        CanonicalForm& operator*=(CanonicalForm rhs);
        CanonicalForm& operator/=(CanonicalForm rhs);

        // Accesseurs
        Scope getScope() const {return scope;}
        OT::SquareMatrix getK() const {return K;}
        OT::Point getH() const {return h;}
        double getG() const {return g;}
        double getLp() const {return lp;}

        CanonicalForm addVariable(ContinuousVariable variable);
        CanonicalForm addVariables(const Scope scope);
        CanonicalForm marginal(const Scope summed_scope) const;
        CanonicalForm reduce(ContinuousEvidence evidence);
        
    private:
        Scope scope;
        OT::SquareMatrix K;
        OT::Point h;
        double g = 0.;
        double lp = 0.;

        void permute_K(OT::Point permutation);
        void permute_h(OT::Point permutation);
};

std::ostream& operator<<(std::ostream& os, const CanonicalForm& item);

CanonicalForm operator+(const CanonicalForm &lhs, const CanonicalForm &rhs);
CanonicalForm operator-(const CanonicalForm &lhs, const CanonicalForm &rhs);
CanonicalForm operator*(const CanonicalForm &lhs, const CanonicalForm &rhs);
CanonicalForm operator/(const CanonicalForm &lhs, const CanonicalForm &rhs);

bool operator==(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator!=(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator<(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator<=(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator>(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator>=(const CanonicalForm &lhs, const CanonicalForm &rhs);

namespace gum {
    template <> class HashFunc<CanonicalForm> : public
    HashFuncBase<CanonicalForm> {
        public:
            Size operator()(const CanonicalForm &key) const {
                auto variables = key.getScope().getVariables();
                auto matrix = key.getK();
                auto vector = key.getH();
                auto constant = key.getG();
                auto proba = key.getLp();

                if(variables.empty()){
                    //return (Size(constant)*HashFuncConst::gold) & this->_hash_mask;
                    return (Size(proba)*HashFuncConst::gold) & this->hash_mask_;
                }
                else {
                    double result = 0.;
                    for(auto it = variables.begin(); it != variables.end(); ++it){
                        result += std::stod(it->getName());
                        result *= HashFuncConst::gold;
                    }
                    //for(auto x : matrix.reshaped()){
                        //result += x;
                        //result *= HashFuncConst::gold;
                    //}
                    for(int i=0; i<matrix.getNbRows(); i++){
                        for(int j=0; j<matrix.getNbColumns(); j++){
                            result += matrix(i,j);
                            result *= HashFuncConst::gold;
                        }
                    }
                    for(auto x : vector){
                        result += x;
                        result *= HashFuncConst::gold;
                    }
                    result += constant;
                    result *= HashFuncConst::gold;
                    result += proba;
                    result *= HashFuncConst::gold;

                    return Size(result) & this->hash_mask_;
                }
            }
    };
}

#endif // CANONICAL_FORM_H
