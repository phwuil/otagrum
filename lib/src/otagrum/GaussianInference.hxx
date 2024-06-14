//                                               -*- C++ -*-
/**
 *  @brief The GaussianBayesianNetwork distribution
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

#ifndef OTAGRUM_GAUSSIAN_INFERENCE_HXX
#define OTAGRUM_GAUSSIAN_INFERENCE_HXX

#include <vector>
#include <map>
#include <agrum/agrum.h>
#include <agrum/tools/multidim/potential.h>
#include <agrum/tools/variables/labelizedVariable.h>

#include <agrum/tools/graphs/undiGraph.h>
#include <agrum/tools/graphs/DAG.h>
#include <agrum/tools/graphs/cliqueGraph.h>

#include "otagrum/GaussianVariable.hxx"
#include "otagrum/CanonicalForm.hxx"
#include "otagrum/NamedDAG.hxx"
#include "otagrum/GaussianBayesianNetwork.hxx"
#include "otagrum/Scope.hxx"
#include "otagrum/otagrumprivate.hxx"

namespace OTAGRUM
{

/**
 * @class GaussianBayesianNetwork
 *
 * The GaussianBayesianNetwork structure.
 */
class GaussianInference {

public:

  /** Constructor */
  GaussianInference(GaussianBayesianNetwork gbn): _gbn_(gbn) {};

  
  //CanonicalForm getPosterior(
          //std::vector<GaussianVariable> &variables,
          //ContinuousEvidence &evidence);
  


private:

  /**
   * @brief Eliminate a continuous variable from a set of potentials
   *
   * @param cf_set A set of potentials
   * @param variable The continuous variable to remove
   */

    void _sum_product_eliminate_var_(
          std::vector< CanonicalForm > &cf_set,
          GaussianVariable &variable);

  /**
   * @brief Variable elimination sum-product algorithm for CLGs
   *
   * @param elim_order Order on variables that are eliminated
   * @param evidence Evidence for variables
   * @param cf_set Set of potentials on which the algorithm is applied
   *
   * @return The resulting product of potentials
   */
  gum::Potential<CanonicalForm> _sum_product_ve_(
          std::vector<GaussianVariable> &elim_order,
          ContinuousEvidence &evidence,
          std::vector< CanonicalForm > cf_set);

    //gum::JunctionTree _buildJunctionTreeFromUndiGraph_(const gum::UndiGraph& g) const;
    //gum::JunctionTree _buildJunctionTreeFromDAG_(const gum::DAG& dag) const;
    std::vector< gum::NodeId > _findEliminationOrder_(const gum::UndiGraph& g) const;
    std::vector< CanonicalForm > _cf_set_;

    GaussianBayesianNetwork _gbn_;
}; /* class GaussianBayesianNetwork */


} /* namespace OTAGRUM */

#endif /* OTAGRUM_GAUSSIAN_INFERENCE_HXX */
