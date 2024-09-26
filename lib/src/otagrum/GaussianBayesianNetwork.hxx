//                                               -*- C++ -*-
/**
 *  @brief The GaussianBayesianNetworkclass
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

#include <openturns/Distribution.hxx>
#include <openturns/Normal.hxx>

#include "otagrum/NamedDAG.hxx"
#include "otagrum/GaussianVariable.hxx"
#include "otagrum/VariableNodeMap.hxx"
#include "otagrum/GaussianBayesianNetwork.hxx"
#include "otagrum/otagrumprivate.hxx"

namespace OTAGRUM
{

/**
 * @class GaussianBayesianNetwork
 *
 * The GaussianBayesianNetworkclass.
 */
class GaussianBayesianNetwork
{
public:

  /** Default constructor */
  GaussianBayesianNetwork() = default;

public:
  gum::Size getDimension() const;
  gum::DAG getDAG() const;
  GaussianVariable getVariable(gum::NodeId varId) const;
  gum::NodeSet getParentsId(gum::NodeId varId) const;
  std::vector < GaussianVariable > getParents(gum::NodeId varId) const;
  bool existsArc(gum::NodeId tailId, gum::NodeId headId) const;

  int addVariable(GaussianVariable variable);
  int addVariable(const std::string& name, double mu = 0, double sigma = 1);
  gum::Arc addArc(gum::NodeId varId1, gum::NodeId varId2, double weight);
  gum::Arc addArc(std::string varName1, std::string varName2, double weight);

  gum::Size size() const;
  std::string toString() const;

  std::string getName(gum::NodeId varId) const;

  void changeMu(gum::NodeId varId, double mu);
  double getMu(gum::NodeId varId) const;
  void changeSigma(gum::NodeId varId, double sigma);
  double getSigma(gum::NodeId varId) const;

  double getWeight(gum::Arc arc) const;
  double getWeight(gum::NodeId tailId, gum::NodeId headId) const;

  gum::Sequence < gum::NodeId > getTopologicalOrder() const;

  OT::Sample getSample(unsigned int size) const;

  OT::Normal buildNormalDistribution() const;

private:
  VariableNodeMap _varMap_;
  gum::DAG _dag_;
  gum::ArcProperty< double > _weights_;
}; /* class GaussianBayesianNetwork*/

std::ostream& operator<<(std::ostream& output, const GaussianBayesianNetwork& factory);

} /* namespace OTAGRUM */

#endif /* OTAGRUM_GAUSSIANBAYESIANNETWORKFACTORY_HXX */
