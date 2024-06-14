#ifndef CANONICAL_FORM_H
#define CANONICAL_FORM_H

#include <iostream>
#include <map>
#include <cmath>
#include <Eigen/Dense>
#include <agrum/tools/core/hashTable.h>
#include <agrum/tools/variables/labelizedVariable.h>

class CanonicalForm;
#include "otagrum/Scope.hxx"
#include "otagrum/Gaussian.hxx"
#include "otagrum/ConditionalLinearGaussian.hxx"

typedef std::vector<std::pair<GaussianVariable, double> > ContinuousEvidence;
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
                      Eigen::MatrixXd K,
                      Eigen::VectorXd h,
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
        Eigen::MatrixXd getK() const {return K;}
        Eigen::VectorXd getH() const {return h;}
        double getG() const {return g;}
        double getLp() const {return lp;}

        CanonicalForm addVariable(GaussianVariable variable);
        CanonicalForm addVariables(const Scope scope);
        CanonicalForm marginal(const Scope summed_scope) const;
        CanonicalForm reduce(ContinuousEvidence evidence);
        
    private:
        Scope scope;
        Eigen::MatrixXd K;
        Eigen::VectorXd h;
        double g = 0.;
        double lp = 0.;

        void permute_K(Eigen::VectorXi permutation);
        void permute_h(Eigen::VectorXi permutation);
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

/*namespace gum {*/
    /*template <> class HashFunc<CanonicalForm> : public*/
    /*HashFuncBase<CanonicalForm> {*/
        /*public:*/
            /*Size operator()(const CanonicalForm &key) const {*/
                /*auto variables = key.getScope().getVariables();*/
                /*auto matrix = key.getK();*/
                /*auto vector = key.getH();*/
                /*auto constant = key.getG();*/
                /*auto proba = key.getLp();*/

                /*if(variables.empty()){*/
                    /*//return (Size(constant)*HashFuncConst::gold) & this->_hash_mask;*/
                    /*return (Size(proba)*HashFuncConst::gold) & this->hash_mask_;*/
                /*}*/
                /*else {*/
                    /*double result = 0.;*/
                    /*for(auto it = variables.begin(); it != variables.end(); ++it){*/
                        /*result += std::stod(it->getName());*/
                        /*result *= HashFuncConst::gold;*/
                    /*}*/
                    /*for(auto x : matrix.reshaped()){*/
                        /*result += x;*/
                        /*result *= HashFuncConst::gold;*/
                    /*}*/
                    /*for(auto x : vector){*/
                        /*result += x;*/
                        /*result *= HashFuncConst::gold;*/
                    /*}*/
                    /*result += constant;*/
                    /*result *= HashFuncConst::gold;*/
                    /*result += proba;*/
                    /*result *= HashFuncConst::gold;*/

                    /*return Size(result) & this->hash_mask_;*/
                /*}*/
            /*}*/
    /*};*/
/*}*/

#endif // CANONICAL_FORM_H
