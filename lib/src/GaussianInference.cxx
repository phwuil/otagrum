//                                               -*- C++ -*-
/**
 *  @brief The GaussianInference method
 *
 *
 *  Copyright 2010-2024 Airbus-LIP6-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cmath>
#include <algorithm>
#include <agrum/tools/multidim/potential.h>
#include <agrum/tools/graphs/algorithms/triangulations/junctionTreeStrategies/defaultJunctionTreeStrategy.h>
#include <agrum/tools/graphs/algorithms/triangulations/defaultTriangulation.h>
#include "otagrum/GaussianInference.hxx"

using namespace OT;
using namespace std;

namespace OTAGRUM
{

/* Default constructor */
GaussianInference::GaussianInference(GaussianBayesianNetwork GBN)
  : _gbn_(GBN)
  , _cf_topo_order_(GBN.getTopologicalOrder())
{
    _buildCanonicalForms_();
}

void GaussianInference::_buildCanonicalForms_() {
    for(const auto nid: _cf_topo_order_) {
        auto variable = _gbn_.getVariable(nid);
        auto parents = _gbn_.getParents(nid);

        auto mu = _gbn_.getMu(nid);
        auto sigma  = _gbn_.getSigma(nid);

        auto cf = CanonicalForm();

        std::cout << "Node Id : " << nid << std::endl;
        std::cout << "Variable : " << _gbn_.getVariable(nid) << std::endl;
        if(parents.size() == 0) {
            std::cout << "Empty ! " << std::endl;
            cf = CanonicalForm(variable);
            std::cout << "CF : " << cf << std::endl;
        }
        else if (parents.size() > 0) {
            std::cout << "Not empty ! " << std::endl;
            std::vector < double > weights;
            for(const auto pid: _gbn_.getParentsId(nid)){
                std::cout << "Parent Id : " << pid << std::endl;
                std::cout << "Parent : " << _gbn_.getVariable(pid) << std::endl;
                weights.push_back(_gbn_.getWeight(pid, nid));
            }
            std::cout << "Weights : " << weights << std::endl;
            cf = CanonicalForm(variable, parents, weights);
        }
        _cf_map_.insert(nid, cf);
    }
    std::cout << "Topological order : " << _cf_topo_order_ << std::endl;
    std::cout << "Proba map : " << _cf_map_ << std::endl;
}

CanonicalForm GaussianInference::getPosterior(GaussianEvidence &evidence){
    auto dag = _gbn_.getDAG();
    auto elimination_order = _findEliminationOrder_(dag.moralGraph());
    auto elimination_order_removed = vector<gum::NodeId>();
    auto elimination_order_kept = vector<gum::NodeId>();

    for (const auto& v:elimination_order){
        if (find(variables.begin(), variables.end(), v) == variables.end())
            elimination_order_removed.push_back(v);
        else
            elimination_order_kept.push_back(v);
    }

    auto cf_list = _SumProductVE_(elimination_order_removed, evidence, _cf_set_);

    CanonicalForm posterior();
    for (const auto& p:cf_list) {
        posterior *= p;
    }

    if (normalized) {
        auto normalization_cf = _SumProductVE_(elimination_order_kept, {}, cf_list);
        CanonicalForm normalization();
        for (const auto& cf: normalization_cf){
            normalization *= cf;
        }
        posterior /= normalization;
    }

    return posterior;
}

void GaussianInference::_SumProductEliminateVar_(
        vector< CanonicalForm > &cf_set,
        GaussianVariable &variable){

    // Looking for canonical forms containing the variable to eliminate
    vector< CanonicalForm > contains_var;
    vector< CanonicalForm > not_contains_var;
    for(auto it=cf_set.begin(); it!=cf_set.end(); ++it){
        if(it->getScope().contains(variable)){
            contains_var.push_back(*it);
        }
        else{
            not_contains_var.push_back(*it);
        }

    }

    // Multiplying potentials containing the variable to eliminate
    CanonicalForm product;
    for(auto cf : contains_var){
        product *= cf;
    }

    // Eliminating the variable from the product
    if(!contains_var.empty()){
        product = product.marginal(Scope({variable})); // CHECK IF MARGINAL IS DONE INPLACE
        not_contains_var.push_back(product);
    }

    // Updating (in place) the set of potentials
    cf_set = not_contains_var;
}

CanonicalForm GaussianInference::_SumProductVE_(
        vector<GaussianVariable> &elim_order,
        GaussianEvidence &evidence,
        vector< CanonicalForm > cf_set){

    vector<GaussianVariable> red_cont_vars;
    for(auto x : elim_order){
        red_cont_vars.push_back(x);
    }

    //cout << "pot avant : " << potential_set << endl;

    if(!evidence.empty()){
        for(auto& cf: cf_set){
                cf.reduce(evidence);
        }
    }

    //cout << "pot après : " << potential_set << endl;

    if(!elim_order.empty()){
        for(auto& v : elim_order){
            _SumProductEliminateVar_(cf_set, v);
        }
    }

    CanonicalForm product;
    for(auto& p : cf_set){
        product *= p;
    }
    
    return product;
}

gum::JunctionTree GaussianInference::_buildJunctionTreeFromUndiGraph_(const gum::UndiGraph& g) const {

    auto mods = g.nodesPropertyFromVal(static_cast< gum::Size >(2));

    gum::StaticTriangulation* triangulation;
    triangulation = new gum::DefaultTriangulation(&g, &mods);

    gum::DefaultJunctionTreeStrategy strategy;
    strategy.setTriangulation(triangulation);
    auto res = strategy.junctionTree();
    delete (triangulation);

    return res;
}


gum::JunctionTree GaussianInference::_buildJunctionTreeFromDAG_(const gum::DAG& dag) const {
    return _buildJunctionTreeFromUndiGraph_(dag.moralGraph());
}

vector< gum::NodeId > GaussianInference::_findEliminationOrder_(const gum::UndiGraph& g) const {
    auto mods = g.nodesPropertyFromVal(static_cast< gum::Size >(2));
    gum::StaticTriangulation* triangulation;
    triangulation = new gum::DefaultTriangulation(&g, &mods);
    auto order = triangulation->eliminationOrder();
    delete (triangulation);
    return order;
}

std::string GaussianInference::toString() const {
    std::stringstream s;
    s << "Used GBN : " << _gbn_.toString();
    return s.str();
}

std::ostream& operator<<(std::ostream& output, const GaussianInference& GI) {
    output << GI.toString();
    return output;
}

} // namespace OTAGRUM
