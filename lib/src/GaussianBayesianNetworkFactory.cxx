//                                               -*- C++ -*-
/**
 *  @brief The GaussianBayesianNetwork distribution
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

#include "otagrum/GaussianBayesianNetworkFactory.hxx"
#include <openturns/Normal.hxx>
#include <cmath>

namespace OTAGRUM
{

gum::DAG GaussianBayesianNetworkFactory::getDAG() const {
    return _dag_;
}

int GaussianBayesianNetworkFactory::addVariable(GaussianVariable variable){
    auto id = _dag_.nextNodeId();
    _varMap_.insert(id, variable);
    _dag_.addNodeWithId(id);
    return id;
}

int GaussianBayesianNetworkFactory::addVariable(const std::string& name, double mu, double sigma){
    auto v = GaussianVariable(name, "", mu, sigma);
    return addVariable(v);
}

gum::Arc GaussianBayesianNetworkFactory::addArc(int varId1, int varId2, double weight){
    auto arc = gum::Arc(varId1, varId2);
    if (!_weights_.exists(arc) && weight != 0.){
        _weights_.insert(arc, weight);
        _dag_.addArc(varId1, varId2);
    }
    return arc;
}

gum::Size GaussianBayesianNetworkFactory::size() const {
    return _varMap_.size();
}

std::string GaussianBayesianNetworkFactory::toString() const {
  std::stringstream s;
  s << "GBN{nodes: " << size() << ", arcs: " << _dag_.sizeArcs() << "}";
  return s.str();
}

double GaussianBayesianNetworkFactory::getMu(gum::NodeId varId) {
    return _varMap_[varId].getMu();
}
void GaussianBayesianNetworkFactory::changeMu(gum::NodeId varId, double mu){
    _varMap_.changeMu(varId, mu);
}
double GaussianBayesianNetworkFactory::getSigma(gum::NodeId varId) {
    return _varMap_[varId].getSigma();
}
void GaussianBayesianNetworkFactory::changeSigma(gum::NodeId varId, double sigma){
    _varMap_.changeSigma(varId, sigma);
}

std::ostream& operator<<(std::ostream& output, const GaussianBayesianNetworkFactory& factory) {
    output << factory.toString();
    return output;
}

//GaussianBayesianNetwork GaussianBayesianNetworkFactory::buildAsGaussianBayesianNetwork() const {

//}

GaussianBayesianNetwork GaussianBayesianNetworkFactory::buildAsGaussianBayesianNetwork() const {
    DistributionCollection distributions;
    auto names = std::vector<std::string>();
    for (const auto nodeId : _dag_.nodes()) {
        std::cout << "NodeId : " << nodeId << std::endl;
        names.push_back(_varMap_.name(nodeId));
        std::cout << "Node name : " << _varMap_.name(nodeId) << std::endl;

        auto mu = _varMap_[nodeId].getMu();
        auto sigma = _varMap_[nodeId].getSigma();

        distributions.add(OT::Normal(mu, sigma));
    }
    //std::cout << "Distributions : " << distributions << std::endl;
    NamedDAG ndag = NamedDAG(_dag_, names); 
    return GaussianBayesianNetwork(ndag, distributions);
}

} // namespace OTAGRUM
