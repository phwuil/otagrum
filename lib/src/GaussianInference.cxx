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
//GaussianInference::GaussianInference(GaussianBayesianNetwork GBN)
//{
//}

/*CanonicalForm GaussianInference::getPosterior(*/
        /*vector<GaussianVariable> &variables,*/
        /*ContinuousEvidence &evidence){*/
    /*auto dag = _gbn_.getDAG();*/
    /*auto elimination_order = _findEliminationOrder_(dag.moralGraph());*/
    /*auto elimination_order_removed = vector<gum::NodeId>();*/
    /*auto elimination_order_kept = vector<gum::NodeId>();*/

    /*for (const auto& v:elimination_order){*/
        /*if (find(variables.begin(), variables.end(), v) == variables.end())*/
            /*elimination_order_removed.push_back(v);*/
        /*else*/
            /*elimination_order_kept.push_back(v);*/
    /*}*/

    /*auto cf_list = _sum_product_ve_(elimination_order_removed, evidence, _cf_set_);*/

    /*CanonicalForm posterior();*/
    /*for (const auto& p:cf_list) {*/
        /*posterior *= p;*/
    /*}*/

    /*if (normalized) {*/
        /*auto normalization_cf = _sum_product_ve_(elimination_order_kept, {}, cf_list);*/
        /*CanonicalForm normalization();*/
        /*for (const auto& cf: normalization_cf){*/
            /*normalization *= cf;*/
        /*}*/
        /*posterior /= normalization;*/
    /*}*/

    /*return posterior;*/
/*}*/

void GaussianInference::_sum_product_eliminate_var_(
        vector< CanonicalForm > &cf_set,
        GaussianVariable &variable){

    // Looking for potentials containing the variable to eliminate
    vector<gum::Potential< CanonicalForm > > contains_var;
    vector<gum::Potential< CanonicalForm > > not_contains_var;
    for(auto it=cf_set.begin(); it!=cf_set.end(); ++it){
        gum::Instantiation I(*it);
        if(it->get(I).getScope().contains(variable)){
            contains_var.push_back(*it);
        }
        else{
            not_contains_var.push_back(*it);
        }

    }

    // Multiplying potentials containing the variable to eliminate
    gum::Potential<CanonicalForm> product;
    for(auto cf : contains_var){
        product *= cf;
    }

    // Eliminating the variable from the product
    if(!contains_var.empty()){
        gum::Instantiation I(product);
        for(int i=0; i != product.domainSize() ; ++i){
            product.set(I, product.get(I).marginal(Scope({variable})));
            I.inc();
        }
        not_contains_var.push_back(product);
    }

    // Updating (in place) the set of potentials
    cf_set = not_contains_var;
}

gum::Potential<CanonicalForm> GaussianInference::sum_product_ve(
        vector<GaussianVariable> &elim_order,
        ContinuousEvidence &evidence,
        vector< CanonicalForm > cf_set){

    vector<GaussianVariable> red_cont_vars;
    for(auto x : elim_order){
        red_cont_vars.push_back(x);
    }

    //cout << "pot avant : " << potential_set << endl;

    if(!evidence.empty()){
        for(const auto& cf: cf_set){
                cf.reduce(evidence);
        }
    }

    //cout << "pot après : " << potential_set << endl;

    if(!elim_order.empty()){
        for(auto& v : elim_order){
            sum_product_eliminate_var(cf_set, v);
        }
    }

    CanonicalForm product;
    for(auto& p : cf_set){
        product *= p;
    }
    
    return product;
}

/*gum::JunctionTree GaussianInference::_buildJunctionTreeFromUndiGraph_(const gum::UndiGraph& g) const {*/

    /*auto mods = g.nodesPropertyFromVal(static_cast< gum::Size >(2));*/

    /*gum::StaticTriangulation* triangulation;*/
    /*triangulation = new gum::DefaultTriangulation(&g, &mods);*/

    /*gum::DefaultJunctionTreeStrategy strategy;*/
    /*strategy.setTriangulation(triangulation);*/
    /*auto res = strategy.junctionTree();*/
    /*delete (triangulation);*/

    /*return res;*/
/*}*/


/*gum::JunctionTree GaussianInference::_buildJunctionTreeFromDAG_(const gum::DAG& dag) const {*/
    /*return _buildJunctionTreeFromUndiGraph_(dag.moralGraph());*/
/*}*/

vector< gum::NodeId > GaussianInference::_findEliminationOrder_(const gum::UndiGraph& g) const {
    auto mods = g.nodesPropertyFromVal(static_cast< gum::Size >(2));
    gum::StaticTriangulation* triangulation;
    triangulation = new gum::DefaultTriangulation(&g, &mods);
    auto order = triangulation->eliminationOrder();
    delete (triangulation);
    return order;
}


} // namespace OTAGRUM
