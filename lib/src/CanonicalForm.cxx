#include <iostream>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <cmath>

#include "otagrum/GaussianVariable.hxx"
#include "otagrum/CanonicalForm.hxx"

using namespace std;
using namespace Eigen;

namespace OTAGRUM {

// Constructors


CanonicalForm::CanonicalForm(GaussianVariable variable){
    double mu = variable.getMu();
    double sigma2 = variable.getSigma() * variable.getSigma();

    _scope_ += variable;

    _K_.resize(1,1);
    _K_(0,0) = 1./sigma2;

    _h_.resize(1);
    _h_(0) = mu/sigma2;

    _g_ = -0.5 * ( mu*mu/sigma2 + log(2*M_PI * sigma2) );
}

// Formula can be found in "Propagation of probabilities, means and variances
// in mixed Graphical association models, Lauritzen, 1992" on page 1102 (formulas
// (3), (4) and (5))
CanonicalForm::CanonicalForm(GaussianVariable variable,
                             vector<GaussianVariable> parents,
                             std::vector<double> weights){
    double mu = variable.getMu();
    double sigma2 = variable.getSigma() * variable.getSigma();

    // Filling the scope
    _scope_.addVariable(variable);
    _scope_.addVariables(parents);

    // Converting the std::vector to a Eigen::VectorXd
    VectorXd beta = Eigen::Map<VectorXd>(weights.data(), weights.size());

    // Computing the h vector
    _h_.resize(beta.size() + 1);
    _h_(0) = 1.;
    _h_.tail(beta.size()) = -beta;
    _h_ *= mu/sigma2;

    // Computing the K matrix
    _K_.resize(beta.size() + 1, beta.size() + 1);
    _K_(0,0) = 1.;
    _K_.block(0, 1, 1, beta.size()) = -beta.transpose();
    _K_.block(1, 0, beta.size(), 1) = -beta;
    _K_.block(1, 1, beta.size(), beta.size()) = beta * beta.transpose();
    _K_ /= sigma2;

    // Computing g
    _g_ = - 0.5 * (mu*mu/sigma2 + log(2*M_PI*sigma2));
    
}


// I/O operators

ostream& operator<<(ostream& os, const CanonicalForm& item)
{
    os << "scope = " << item._scope_ << endl
       << "K = " << endl << item._K_ << endl
       << "h = " << endl << item._h_ << endl
       << "g = " << item._g_ << endl;
    return os;
}


// Relational operators

bool operator==(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return lhs._K_ == rhs._K_ &&
           lhs._h_ == rhs._h_ &&
           lhs._g_ == rhs._g_;
}

bool operator!=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs == rhs);
}

bool operator<(const CanonicalForm &lhs, const CanonicalForm &rhs){
    // Mettre quelque chose qui a du sens (si ça existe).
    // Information contenue dans la gaussienne associée ?
    return lhs._g_ < rhs._g_;
}

bool operator<=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    // Mettre quelque chose qui a du sens (si ça existe).
    // Information contenue dans la gaussienne associée ?
    return lhs._g_ < rhs._g_ ||
           lhs._g_ == rhs._g_;
}

bool operator>(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs <= rhs);
}

bool operator>=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs < rhs);
}


// Arithmetic operators

//CanonicalForm& CanonicalForm::operator+=(const CanonicalForm &rhs){
    //if(!(K.size()) && !(rhs.K.size())){
        //g = log(exp(g) + exp(rhs.g));
        //lp = log(exp(lp) + exp(rhs.lp));
    //}
    //else{
        //*this = CanonicalForm(Gaussian(*this) + Gaussian(rhs));
    //}
    //return *this;
//}
//CanonicalForm operator+(const CanonicalForm &lhs, const CanonicalForm &rhs){
    //CanonicalForm sum = lhs;
    //sum += rhs;
    //return sum;
//}

//CanonicalForm& CanonicalForm::operator-=(const CanonicalForm &rhs){
    //return *this;
//}
//CanonicalForm operator-(const CanonicalForm &lhs, const CanonicalForm &rhs){
    //CanonicalForm diff = lhs;
    //diff -= rhs;
    //return diff;
//}

CanonicalForm& CanonicalForm::operator*=(CanonicalForm rhs){
    Scope scope_union = _scope_ + rhs._scope_;
    this->extend(scope_union - _scope_);
    rhs.extend(scope_union - rhs._scope_);

    auto permutation = _findPermutation_(_scope_, rhs._scope_);
    rhs.permute(permutation);

    _K_ += rhs._K_;
    _h_ += rhs._h_;
    _g_ += rhs._g_;

    return *this;
}

CanonicalForm operator*(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm product = lhs;
    product *= rhs;
    return product;
}

CanonicalForm& CanonicalForm::operator/=(CanonicalForm rhs){
    Scope scope_union = _scope_ + rhs._scope_;
    this->extend(scope_union - _scope_);
    rhs.extend(scope_union - rhs._scope_);

    auto permutation = _findPermutation_(_scope_, rhs._scope_);
    rhs.permute(permutation);

    _K_ -= rhs._K_;
    _h_ -= rhs._h_;
    _g_ -= rhs._g_;

    return *this;
}
CanonicalForm operator/(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm ratio = lhs;
    ratio /= rhs;
    return ratio;
}

CanonicalForm CanonicalForm::extend(const GaussianVariable variable){
    if(_scope_.contains(variable))
        return *this;

    _scope_.addVariable(variable);

    // Matrix extension
    _K_.conservativeResize(_scope_.getSize(), _scope_.getSize());
    _K_.row(_scope_.getSize() - 1).setZero();
    _K_.col(_scope_.getSize() - 1).setZero();

    // Vector extension
    _h_.conservativeResize(_scope_.getSize());
    _h_(_h_.size() - 1) = 0.;

    return *this;
}

CanonicalForm CanonicalForm::extend(const GaussianVariable variable, unsigned int pos){
    if(_scope_.contains(variable))
        return *this;

    _scope_.addVariable(variable, pos);

    // Extension de la matrice
    MatrixXd temp = MatrixXd::Zero(_scope_.getSize(), _scope_.getSize()-1);

    temp.topRows(pos) = _K_.topRows(pos);

    temp.bottomRows(_scope_.getSize() - 1 - pos) =
        _K_.bottomRows(_scope_.getSize() - 1 - pos);

    _K_ = temp;

    temp = MatrixXd::Zero(_scope_.getSize(), _scope_.getSize());
    
    temp.leftCols(pos) = _K_.leftCols(pos);

    temp.rightCols(_scope_.getSize() - 1 - pos)
        = _K_.rightCols(_scope_.getSize() - 1 - pos);

    _K_ = temp;


    // Extension du vecteur
    temp = VectorXd::Zero(_scope_.getSize());

    temp.topRows(pos) = _h_.topRows(pos);
    temp.bottomRows(_scope_.getSize() - 1 - pos) = 
        _h_.bottomRows(_scope_.getSize() - 1 - pos);
    _h_ = temp;

    return *this;
}

CanonicalForm CanonicalForm::extend(const Scope scope){
    for(auto v : scope.getVariables()){
        this->extend(v);
    }

    return *this;
}

vector<int> CanonicalForm::_findPermutation_(const Scope& scope1, const Scope& scope2){
    if (scope1.getSize() != scope2.getSize()) {
        throw runtime_error("Scopes have different sizes");
    }

    auto variables1 = scope1.getVariables();
    auto variables2 = scope2.getVariables();

    std::vector<int> permutation;
    for (const auto& v: variables2) {
        // Find the position of v in variables1
        auto it = std::find(variables1.begin(), variables1.end(), v);
        if (it != variables1.end()) {
            // Record the index in the permutation vector
            permutation.push_back(std::distance(variables1.begin(), it));
        } else {
            throw std::runtime_error("Element not found in scope");
        }
    }
    return permutation;
}

void CanonicalForm::_permuteK_(const vector<int>& permutation){
    PermutationMatrix<Dynamic, Dynamic> P(permutation.size());
    for(int i=0;i<permutation.size();++i){
        P.indices()(i) = permutation[i];
    }
    _K_ = P*_K_*P.transpose();
}

void CanonicalForm::_permuteH_(const vector<int>& permutation){
    PermutationMatrix<Dynamic, Dynamic> P(permutation.size());
    for(int i=0;i<permutation.size();++i){
        P.indices()(i) = permutation[i];
    }
    _h_ = P*_h_;
}

void CanonicalForm::permute(const vector<int>& permutation){
    _permuteK_(permutation);
    _permuteH_(permutation);
}

CanonicalForm CanonicalForm::marginal(const Scope summed_vars) const {
    if(!(summed_vars.getSize())){
        return *this;
    }

    Scope unsummed_vars = _scope_ - summed_vars;
    
    vector<int> sum_indices;
    vector<int> unsum_indices;

    int index = 0;

    for(auto v : _scope_.getVariables()){
        if(summed_vars.contains(v)){
            sum_indices.push_back(index);
        }
        else if(unsummed_vars.contains(v)){
            unsum_indices.push_back(index);
        }
        // else lever une exception
        ++index;
    }

    MatrixXd K_XX = _K_(unsum_indices, unsum_indices);
    MatrixXd K_YY = _K_(sum_indices, sum_indices);
    MatrixXd K_XY = _K_(unsum_indices, sum_indices);
    MatrixXd K_YX = _K_(sum_indices, unsum_indices);

    VectorXd h_X = _h_(unsum_indices);
    VectorXd h_Y = _h_(sum_indices);

    MatrixXd K_YY_inv = K_YY.inverse();
    MatrixXd new_K = K_XX - K_XY*K_YY_inv*K_YX;
    VectorXd new_h = h_X - K_XY*K_YY_inv*h_Y;
    double new_g = _g_ + 0.5*(summed_vars.getSize()*log(2*M_PI) -
                            log(K_YY.determinant()) +
                            h_Y.transpose()*K_YY_inv*h_Y);

    //for(auto v : summed_scope.getVariables()){
        //auto it = find(scope.getVariables().begin(),
                       //scope.getVariables().end(),
                       //v);

        //if( it != scope.getVariables().end() ){
            //int index = distance(scope.getVariables().begin(), it);
            //indices.push_back(index);
        //}
    //}

    return CanonicalForm(unsummed_vars, new_K, new_h, new_g);
}

CanonicalForm CanonicalForm::reduce(ContinuousEvidence evidence){

    VectorXd values = VectorXd::Zero(evidence.size());
    Scope reduced_vars;

    int idx = 0;
    for(auto it=evidence.begin(); it!=evidence.end(); ++it, ++idx){
        if(_scope_.contains(it->first)){
            reduced_vars.addVariable(it->first);
        }
        values(idx) = it->second;
    }

    if(!(reduced_vars.getSize())){
        return *this;
    }

    Scope unreduced_vars = _scope_ - reduced_vars;


    vector<int> red_indices;
    vector<int> unred_indices;
    idx = 0;
    for(auto v : _scope_.getVariables()){
        if(reduced_vars.contains(v)){
            red_indices.push_back(idx);
        }
        else if(unreduced_vars.contains(v)){
            unred_indices.push_back(idx);
        }
        // else lever une exception
        ++idx;
    }

    MatrixXd K_XX = _K_(unred_indices, unred_indices);
    MatrixXd K_YY = _K_(red_indices, red_indices);
    MatrixXd K_XY = _K_(unred_indices, red_indices);

    VectorXd h_X = _h_(unred_indices);
    VectorXd h_Y = _h_(red_indices);

    VectorXd new_h = h_X - K_XY*values;
    double new_g = _g_ + h_Y.transpose()*values
                     - 0.5*values.transpose()*K_YY*values;

    return CanonicalForm(unreduced_vars, K_XX, new_h, new_g);
}

} // namespace OTAGRUM
