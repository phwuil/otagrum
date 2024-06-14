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
#ifndef OTAGRUM_GAUSSIANBAYESIANNETWORK_HXX
#define OTAGRUM_GAUSSIANBAYESIANNETWORK_HXX

//#include <openturns/ContinuousDistribution.hxx>
//#include <openturns/Distribution.hxx>

#include "otagrum/NamedDAG.hxx"
#include "otagrum/GaussianVariable.hxx"
#include "otagrum/Scope.hxx"
#include "otagrum/otagrumprivate.hxx"

namespace OTAGRUM
{

/**
 * @class GaussianBayesianNetwork
 *
 * The GaussianBayesianNetwork structure.
 */
class GaussianBayesianNetwork
{
public:


  /** Default constructor */
  GaussianBayesianNetwork();

public:
  gum::DAG getDAG() const;

  int addVariable(GaussianVariable variable);
  gum::Arc addArc(int varId1, int varId2, double weight);


private:
  Scope scope_; // Maybe it should be changed into a dictionary
  gum::DAG dag_;
  gum::ArcProperty< double > arc_weights_;
}; /* class GaussianBayesianNetwork */

} /* namespace OTAGRUM */

#endif /* OTAGRUM_GAUSSIANBAYESIANNETWORK_HXX */
