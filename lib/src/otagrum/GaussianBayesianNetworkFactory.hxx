//                                               -*- C++ -*-
/**
 *  @brief The GaussianBayesianNetworkFactory class
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
#ifndef OTAGRUM_GAUSSIANBAYESIANNETWORKFACTORY_HXX
#define OTAGRUM_GAUSSIANBAYESIANNETWORKFACTORY_HXX

//#include <openturns/ContinuousDistribution.hxx>
//#include <openturns/Distribution.hxx>

#include "otagrum/NamedDAG.hxx"
#include "otagrum/GaussianVariable.hxx"
#include "otagrum/VariableNodeMap.hxx"
#include "otagrum/otagrumprivate.hxx"

namespace OTAGRUM
{

/**
 * @class GaussianBayesianNetworkFactory
 *
 * The GaussianBayesianNetworkFactory class.
 */
class GaussianBayesianNetworkFactory
{
public:


  /** Default constructor */
  GaussianBayesianNetworkFactory() = default;

public:
  gum::DAG getDAG() const;

  int addVariable(GaussianVariable variable);
  int addVariable(const std::string& name, double mu = 0, double sigma = 1);
  gum::Arc addArc(int varId1, int varId2, double weight);

  gum::Size size() const;
  std::string toString() const;

  void changeMu(gum::NodeId varId, double mu);
  double getMu(gum::NodeId varId);
  void changeSigma(gum::NodeId varId, double sigma);
  double getSigma(gum::NodeId varId);

  //GaussianBayesianNetwork build();

private:
  VariableNodeMap _varMap_;
  gum::DAG _dag_;
  gum::ArcProperty< double > _weights_;
}; /* class GaussianBayesianNetworkFactory */

std::ostream& operator<<(std::ostream& output, const GaussianBayesianNetworkFactory& factory);

} /* namespace OTAGRUM */

#endif /* OTAGRUM_GAUSSIANBAYESIANNETWORKFACTORY_HXX */
