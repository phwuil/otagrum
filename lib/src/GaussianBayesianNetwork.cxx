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
#include <Eigen/Dense>
#include <openturns/Normal.hxx>
#include <openturns/Sample.hxx>
#include <cmath>

using namespace Eigen;

namespace OTAGRUM
{

gum::Size GaussianBayesianNetwork::getDimension() const {
    return _dag_.size();
}

gum::DAG GaussianBayesianNetwork::getDAG() const {
    return _dag_;
}

GaussianVariable GaussianBayesianNetwork::getVariable(gum::NodeId varId) const {
    return _varMap_[varId];
}
gum::NodeSet GaussianBayesianNetwork::getParentsId(gum::NodeId varId) const {
    return _dag_.parents(varId);
}

bool GaussianBayesianNetwork::existsArc(gum::NodeId tailId, gum::NodeId headId) const {
    return _dag_.existsArc(tailId, headId);
}

std::vector < GaussianVariable > GaussianBayesianNetwork::getParents(gum::NodeId varId) const {
    std::vector < GaussianVariable > parents;
    for(const auto pid: getParentsId(varId)){
        parents.push_back(getVariable(pid));
    }
    return parents;
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

std::string GaussianBayesianNetwork::getName(gum::NodeId varId) const{
    return getVariable(varId).name();
}

double GaussianBayesianNetwork::getMu(gum::NodeId varId) const {
    return _varMap_[varId].getMu();
}
void GaussianBayesianNetwork::changeMu(gum::NodeId varId, double mu){
    _varMap_.changeMu(varId, mu);
}
double GaussianBayesianNetwork::getSigma(gum::NodeId varId) const {
    return _varMap_[varId].getSigma();
}
void GaussianBayesianNetwork::changeSigma(gum::NodeId varId, double sigma){
    _varMap_.changeSigma(varId, sigma);
}

double GaussianBayesianNetwork::getWeight(gum::Arc arc) const{
    if(!_weights_.exists(arc)) 
        throw gum::NotFound("Arc not found in the GBN");

    return _weights_[arc];
}

double GaussianBayesianNetwork::getWeight(gum::NodeId tailId, gum::NodeId headId) const {
    return getWeight(gum::Arc(tailId, headId));
}

gum::Sequence < gum::NodeId > GaussianBayesianNetwork::getTopologicalOrder() const {
    return _dag_.topologicalOrder();
}


std::ostream& operator<<(std::ostream& output, const GaussianBayesianNetwork& factory) {
    output << factory.toString();
    return output;
}

// Forward sampling for CLG
OT::Sample GaussianBayesianNetwork::getSample(unsigned int size) const {
    const auto topo_order = getTopologicalOrder();

    OT::Sample sample(size, getDimension());

    // Building the description of the sample
    OT::Description description(topo_order.size());
    for (int i=0; i<topo_order.size(); ++i) {
        description[i] = getName(topo_order[i]);
    }
    sample.setDescription(description);

    // Filling the sample
    for (int m=0; m<size; ++m) {
        // Filling the realization following a topological order
        gum::NodeProperty<double> realization;
        for (int i=0; i<topo_order.size(); ++i) {

            // The nodeId of the i-th variable in the topological order
            auto nid = topo_order[i];

            double mu = getMu(nid);
            double sigma = getSigma(nid);
            auto parents_id = getParentsId(nid);

            // If the node has parents, mu is a linear regression of parents
            if (parents_id.size() > 0) {
                for (const auto pid: parents_id) {
                    //std::cout << "Parent: " << topo_order[pid] << std::endl;
                    //std::cout << "Weight: " << getWeight( << std::endl;
                    mu += getWeight(pid,nid) * realization[pid];
                }
            }

            realization.insert(nid, OT::Normal(mu, sigma).getRealization()[0]);

            if (m == 0) {
                std::cout << "Mu(" << getName(topo_order[i]) << ")= " << mu << std::endl;
                std::cout << "Sigma(" << getName(topo_order[i]) << ")= " << sigma << std::endl;
                std::cout << "Realization: " << realization << std::endl;
            }
        }

        // Converting the NodeProperty into an OT::Point
        OT::Point point(realization.size());
        for (int i=0; i<topo_order.size(); ++i) {
            point[i] = realization[topo_order[i]];
        }
        sample[m] = point;
    }

    return sample;
}

// Uses the algorithm on p.427 of "Learning Bayesian Networks, Neapolitan, 2004"
OT::Normal GaussianBayesianNetwork::buildNormalDistribution() const{
    auto t_order = getTopologicalOrder();

    // The description containing the variable names the in topological order
    OT::Description description(t_order.size());
    description[0] = getName(t_order[0]);

    // Constructing the mean vector and covariance matrix
    Eigen::VectorXd mean_vector(t_order.size());
    mean_vector(0) = getMu(t_order[0]);

    auto sigma = getSigma(t_order[0]);
    MatrixXd T =  MatrixXd::Constant(1, 1, 1./(sigma*sigma));

    for(int i=1; i<t_order.size(); ++i){
        description[i] = getName(t_order[i]);

        MatrixXd T_prev = T;

        // Computing the current t
        sigma = getSigma(t_order[i]);
        double t = 1./( sigma*sigma );

        // Creating the current vector of weight b
        auto parents_id = getParentsId(t_order[i]);
        VectorXd b(i);
        for(int j=0; j<i; ++j){
            if( existsArc(t_order[j], t_order[i] ) ){
                    b(j) = getWeight(t_order[j], t_order[i]);
            }
            else {
                b(j) = 0.;
            }
        }

        // Filling the mean vector
        mean_vector[i] = getMu(t_order[i]) + b.transpose()*mean_vector.head(i);

        // Filling the inverse covariance matrix
        T.resize(i+1, i+1);
        T.topLeftCorner(i, i) = T_prev + t*b*b.transpose();
        T.topRightCorner(i, 1) = -t*b;
        T.bottomLeftCorner(1, i) = -t*b.transpose();
        T(i,i) = t;
    }

    // Inverting the Eigen::MatrixXd to obtain the covariance matrix
    T = T.inverse();

    // Copying the Eigen::MatrixXd into an OT::CovarianceMatrix
    OT::CovarianceMatrix Sigma(T.rows());
    for( Index i=0; i<T.rows(); ++i ) {
        for( Index j=0; j<=i; ++j ) {
            Sigma(i, j) = T(i, j);
        }
    }

    // Copying the Eigen::VectorXd into an OT::Point
    OT::Point mu(mean_vector.size());
    for( int i=0; i<mean_vector.size(); ++i) {
        mu[i] = mean_vector[i];
    }

    // Creating the normal distribution
    OT::Normal normal_distribution(mu, Sigma);
    normal_distribution.setDescription(description);

    return normal_distribution;
}

} // namespace OTAGRUM
