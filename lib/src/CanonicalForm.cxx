#include <iostream>
#include <openturns/SquareMatrix.hxx>
#include <openturns/IdentityMatrix.hxx>
#include <cmath>


#include "otagrum/Scope.hxx"
#include "otagrum/CanonicalForm.hxx"

using namespace std;
using namespace OT;

// Constructors


CanonicalForm::CanonicalForm(Gaussian G){
    scope = G.scope;
    K = SquareMatrix(G.Sigma.solveLinearSystem(IdentityMatrix(scope.getSize())).getImplementation());
    h = K * G.mu;
    g = -0.5 * ( G.mu.dot(h) +
                 log(pow(2*M_PI, G.Sigma.getNbRows()) * G.Sigma.computeDeterminant()) );
    lp = log(G.p);
}

CanonicalForm::CanonicalForm(LinearGaussian LG){
    scope = LG.uncond_scope + LG.cond_scope;
    Matrix M = Matrix(scope.getSize(), LG.uncond_scope.getSize(), {0});

    int rowu = 0;
    int rowc = 0;
    int row = 0;
    for(auto v : scope.getVariables()){
        if(LG.uncond_scope.contains(v)){
            M.getRow(row) = IdentityMatrix(LG.uncond_scope.getSize()).getRow(rowu);
            rowu++;
        }
        else if(LG.cond_scope.contains(v)){
            M.getRow(row) = -LG.B.getRow(rowc);
            rowc++;
        }
        row++;
    }
    SquareMatrix Sigma_inv = SquareMatrix(LG.Sigma.solveLinearSystem(IdentityMatrix(LG.Sigma.getNbRows())).getImplementation());
    K = M * Sigma_inv * M.transpose();
    h = M * Sigma_inv * LG.mu;
    g = - 0.5 * LG.mu.transpose() * Sigma_inv * LG.mu
        - log(  pow(2*M_PI,0.5*LG.uncond_scope.getSize())
              * pow(LG.Sigma.determinant(), 0.5));
    lp = 0.;
    
}


// I/O operators

ostream& operator<<(ostream& os, const CanonicalForm& item)
{
    os << "scope = " << endl << item.scope << endl
       << "K = " << endl << item.K << endl
       << "h = " << endl << item.h << endl
       << "g = " << endl << item.g << endl
       << "lp = " << endl << item.lp << endl;
    return os;
}


// Relational operators

bool operator==(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return lhs.K == rhs.K &&
           lhs.h == rhs.h &&
           lhs.g == rhs.g &&
           lhs.lp == rhs.lp;
}

bool operator!=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs == rhs);
}

bool operator<(const CanonicalForm &lhs, const CanonicalForm &rhs){
    // Mettre quelque chose qui a du sens (si ça existe).
    // Information contenue dans la gaussienne associée ?
    return lhs.g < rhs.g;
}

bool operator<=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    // Mettre quelque chose qui a du sens (si ça existe).
    // Information contenue dans la gaussienne associée ?
    return lhs.g < rhs.g ||
           lhs.g == rhs.g;
}

bool operator>(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs <= rhs);
}

bool operator>=(const CanonicalForm &lhs, const CanonicalForm &rhs){
    return !(lhs < rhs);
}


// Arithmetic operators

CanonicalForm& CanonicalForm::operator+=(const CanonicalForm &rhs){
    if(!(K.size()) && !(rhs.K.size())){
        //g = log(exp(g) + exp(rhs.g));
        lp = log(exp(lp) + exp(rhs.lp));
    }
    else{
        *this = CanonicalForm(Gaussian(*this) + Gaussian(rhs));
    }
    return *this;
}
CanonicalForm operator+(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm sum = lhs;
    sum += rhs;
    return sum;
}

CanonicalForm& CanonicalForm::operator-=(const CanonicalForm &rhs){
    return *this;
}
CanonicalForm operator-(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm diff = lhs;
    diff -= rhs;
    return diff;
}

CanonicalForm& CanonicalForm::operator*=(CanonicalForm rhs){
    Scope scope_union = scope + rhs.scope;
    this->addVariables(scope_union - scope);
    rhs.addVariables(scope_union - rhs.scope);

    K += rhs.K;
    h += rhs.h;
    g += rhs.g;
    lp += rhs.lp;

    return *this;
}
CanonicalForm operator*(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm product = lhs;
    product *= rhs;
    return product;
}

CanonicalForm& CanonicalForm::operator/=(CanonicalForm rhs){
    Scope scope_union = scope + rhs.scope;
    this->addVariables(scope_union - scope);
    rhs.addVariables(scope_union - rhs.scope);

    K -= rhs.K;
    h -= rhs.h;
    g -= rhs.g;
    lp -= rhs.lp;
    return *this;
}
CanonicalForm operator/(const CanonicalForm &lhs, const CanonicalForm &rhs){
    CanonicalForm ratio = lhs;
    ratio /= rhs;
    return ratio;
}

CanonicalForm CanonicalForm::addVariable(const ContinuousVariable variable){
    if(scope.contains(variable))
        return *this;

    int pos = scope.addVariable(variable);

    // Extension de la matrice
    MatrixXd temp = MatrixXd::Zero(scope.getSize(), scope.getSize()-1);

    temp.topRows(pos) = K.topRows(pos);

    temp.bottomRows(scope.getSize() - 1 - pos) =
        K.bottomRows(scope.getSize() - 1 - pos);

    K = temp;

    temp = MatrixXd::Zero(scope.getSize(), scope.getSize());
    
    temp.leftCols(pos) = K.leftCols(pos);

    temp.rightCols(scope.getSize() - 1 - pos)
        = K.rightCols(scope.getSize() - 1 - pos);

    K = temp;


    // Extension du vecteur
    temp = VectorXd::Zero(scope.getSize());

    temp.topRows(pos) = h.topRows(pos);
    temp.bottomRows(scope.getSize() - 1 - pos) = 
        h.bottomRows(scope.getSize() - 1 - pos);
    h = temp;

    return *this;
}

CanonicalForm CanonicalForm::addVariables(const Scope scope){
    for(auto v : scope.getVariables()){
        this->addVariable(v);
    }

    return *this;
}

void CanonicalForm::permute_K(VectorXi permutation){
    PermutationMatrix<Dynamic, Dynamic> P(permutation);
    K = P.transpose()*K*P;
}
void CanonicalForm::permute_h(VectorXi permutation){
    PermutationMatrix<Dynamic, Dynamic> P(permutation);
    K = P.transpose()*h;
}

CanonicalForm CanonicalForm::marginal(const Scope summed_vars) const {
    if(!(summed_vars.getSize())){
        return *this;
    }

    Scope unsummed_vars = scope - summed_vars;
    
    vector<int> sum_indices;
    vector<int> unsum_indices;

    int index = 0;

    for(auto v : scope.getVariables()){
        if(summed_vars.contains(v)){
            sum_indices.push_back(index);
        }
        else if(unsummed_vars.contains(v)){
            unsum_indices.push_back(index);
        }
        // else lever une exception
        ++index;
    }

    MatrixXd K_XX = K(unsum_indices, unsum_indices);
    MatrixXd K_YY = K(sum_indices, sum_indices);
    MatrixXd K_XY = K(unsum_indices, sum_indices);
    MatrixXd K_YX = K(sum_indices, unsum_indices);

    VectorXd h_X = h(unsum_indices);
    VectorXd h_Y = h(sum_indices);

    MatrixXd K_YY_inv = K_YY.inverse();
    MatrixXd new_K = K_XX - K_XY*K_YY_inv*K_YX;
    VectorXd new_h = h_X - K_XY*K_YY_inv*h_Y;
    double new_g = g + 0.5*(summed_vars.getSize()*log(2*M_PI) -
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
        if(scope.contains(it->first)){
            reduced_vars.addVariable(it->first);
        }
        values(idx) = it->second;
    }

    if(!(reduced_vars.getSize())){
        return *this;
    }

    Scope unreduced_vars = scope - reduced_vars;


    vector<int> red_indices;
    vector<int> unred_indices;
    idx = 0;
    for(auto v : scope.getVariables()){
        if(reduced_vars.contains(v)){
            red_indices.push_back(idx);
        }
        else if(unreduced_vars.contains(v)){
            unred_indices.push_back(idx);
        }
        // else lever une exception
        ++idx;
    }

    MatrixXd K_XX = K(unred_indices, unred_indices);
    MatrixXd K_YY = K(red_indices, red_indices);
    MatrixXd K_XY = K(unred_indices, red_indices);

    VectorXd h_X = h(unred_indices);
    VectorXd h_Y = h(red_indices);

    VectorXd new_h = h_X - K_XY*values;
    double new_g = g + h_Y.transpose()*values
                     - 0.5*values.transpose()*K_YY*values;

    return CanonicalForm(unreduced_vars, K_XX, new_h, new_g);
}
