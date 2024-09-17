#ifndef CANONICAL_FORM_H
#define CANONICAL_FORM_H

#include <iostream>
#include <map>
#include <cmath>

#include <Eigen/Dense>
#include <agrum/tools/core/hashTable.h>
#include <agrum/tools/core/sequence.h>

#include "otagrum/GaussianVariable.hxx"
#include "otagrum/Scope.hxx"


namespace OTAGRUM {

class CanonicalForm {
    using ContinuousEvidence = std::vector<std::pair<GaussianVariable, double> >;
    //using Scope = gum::Sequence< GaussianVariable >;

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
        CanonicalForm(Scope scope,
                      Eigen::MatrixXd K,
                      Eigen::VectorXd h,
                      double g): _scope_(scope), _K_(K), _h_(h), _g_(g) {}

        // Constructor from parameters of a 1D Gaussian
        explicit CanonicalForm(GaussianVariable variable);

        // Constructor from parameters of a 1D conditional linear Gaussian
        CanonicalForm(GaussianVariable variable,
                      std::vector<GaussianVariable> parents,
                      std::vector<double> weights);

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

        //CanonicalForm& operator+=(const CanonicalForm &rhs);
        //CanonicalForm& operator-=(const CanonicalForm &rhs);
        CanonicalForm& operator*=(CanonicalForm rhs);
        CanonicalForm& operator/=(CanonicalForm rhs);

        // Accesseurs
        const unsigned int getSize() const {return _scope_.getSize();}
        Scope getScope() const {return _scope_;}
        Eigen::MatrixXd getK() const {return _K_;}
        Eigen::VectorXd getH() const {return _h_;}
        double getG() const {return _g_;}

        CanonicalForm extend(GaussianVariable variable, unsigned int pos);
        CanonicalForm extend(GaussianVariable variable);
        CanonicalForm extend(const Scope scope);
        void permute(const std::vector<int>& permutation);
        CanonicalForm marginal(const Scope summed_scope) const;
        CanonicalForm reduce(ContinuousEvidence evidence);
        
    private:
        std::vector<int> _findPermutation_(const Scope& scope1, const Scope& scope2);
        void _permuteK_(const std::vector<int>& permutation);
        void _permuteH_(const std::vector<int>& permutation);

        Scope _scope_;
        Eigen::MatrixXd _K_;
        Eigen::VectorXd _h_;
        double _g_ = 0.;

};

std::ostream& operator<<(std::ostream& os, const CanonicalForm& item);

//CanonicalForm operator+(const CanonicalForm &lhs, const CanonicalForm &rhs);
//CanonicalForm operator-(const CanonicalForm &lhs, const CanonicalForm &rhs);
CanonicalForm operator*(const CanonicalForm &lhs, const CanonicalForm &rhs);
CanonicalForm operator/(const CanonicalForm &lhs, const CanonicalForm &rhs);

bool operator==(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator!=(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator<(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator<=(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator>(const CanonicalForm &lhs, const CanonicalForm &rhs);
bool operator>=(const CanonicalForm &lhs, const CanonicalForm &rhs);

} // namespace OTAGRUM

#endif // CANONICAL_FORM_H
