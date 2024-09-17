//                                               -*- C++ -*-
/**
 *  @brief The GaussianBayesianNetwork
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

#include "otagrum/GaussianBayesianNetwork.hxx"
#include <openturns/Normal.hxx>
#include <cmath>

namespace OTAGRUM
{

gum::DAG GaussianBayesianNetwork::getDAG() const {
    return _dag_;
}

GaussianVariable GaussianBayesianNetwork::getVariable(gum::NodeId varId) const {
    return _varMap_[varId];
}
gum::NodeSet GaussianBayesianNetwork::getParents(gum::NodeId varId) const {
    return _dag_.parents(varId);
}

int GaussianBayesianNetwork::addVariable(GaussianVariable variable){
    auto id = _dag_.nextNodeId();
    _varMap_.insert(id, variable);
    _dag_.addNodeWithId(id);
    return id;
}

int GaussianBayesianNetwork::addVariable(const std::string& name, double mu, double sigma){
    auto v = GaussianVariable(name, "", mu, sigma);
    return addVariable(v);
}

gum::Arc GaussianBayesianNetwork::addArc(int varId1, int varId2, double weight){
    auto arc = gum::Arc(varId1, varId2);
    if (!_weights_.exists(arc) && weight != 0.){
        _weights_.insert(arc, weight);
        _dag_.addArc(varId1, varId2);
    }
    return arc;
}

gum::Size GaussianBayesianNetwork::size() const {
    return _varMap_.size();
}

std::string GaussianBayesianNetwork::toString() const {
  std::stringstream s;
  s << "GBN{nodes: " << size() << ", arcs: " << _dag_.sizeArcs() << "}";
  return s.str();
}

double GaussianBayesianNetwork::getMu(gum::NodeId varId) {
    return _varMap_[varId].getMu();
}
void GaussianBayesianNetwork::changeMu(gum::NodeId varId, double mu){
    _varMap_.changeMu(varId, mu);
}
double GaussianBayesianNetwork::getSigma(gum::NodeId varId) {
    return _varMap_[varId].getSigma();
}
void GaussianBayesianNetwork::changeSigma(gum::NodeId varId, double sigma){
    _varMap_.changeSigma(varId, sigma);
}

double GaussianBayesianNetwork::getWeight(gum::Arc arc){
    if(!_weights_.exists(arc)) 
        throw gum::NotFound("Arc not found in the GBN");

    return _weights_[arc];
}

double GaussianBayesianNetwork::getWeight(gum::NodeId tailId, gum::NodeId headId){
    return getWeight(gum::Arc(tailId, headId));
}

gum::Sequence < gum::NodeId > GaussianBayesianNetwork::getTopologicalOrder() {
    return _dag_.topologicalOrder();
}


std::ostream& operator<<(std::ostream& output, const GaussianBayesianNetwork& factory) {
    output << factory.toString();
    return output;
}

/*GaussianBayesianNetwork GaussianBayesianNetwork::build() const {*/
    /*DistributionCollection distributions;*/
    /*auto names = std::vector<std::string>();*/
    /*for (const auto nodeId : _dag_.nodes()) {*/
        /*std::cout << "NodeId : " << nodeId << std::endl;*/
        /*names.push_back(_varMap_.name(nodeId));*/
        /*std::cout << "Node name : " << _varMap_.name(nodeId) << std::endl;*/

        /*auto mu = _varMap_[nodeId].getMu();*/
        /*auto sigma = _varMap_[nodeId].getSigma();*/

        /*distributions.add(OT::Normal(mu, sigma));*/
    /*}*/
    /*//std::cout << "Distributions : " << distributions << std::endl;*/
    /*NamedDAG ndag = NamedDAG(_dag_, names); */
    /*return GaussianBayesianNetwork(ndag, distributions);*/
/*}*/

} // namespace OTAGRUM
