//                                               -*- C++ -*-
/**
 *  @brief variableNodeMap class
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


#ifndef VARIABLE_NODE_MAP_H
#define VARIABLE_NODE_MAP_H

#include <iostream>
#include <string>

#include <agrum/agrum.h>

#include <agrum/tools/core/bijection.h>
#include <agrum/tools/graphs/graphElements.h>

#include "GaussianVariable.hxx"

namespace OTAGRUM {

  class VariableNodeMap {
    public:
    // Default constructor
    VariableNodeMap();

    // Copy constructor
    // Proceed a deep copy: all variables are copied but keep the same node id.
    VariableNodeMap(const VariableNodeMap& source);

    // Destructor
    ~VariableNodeMap();

    /// Copy operator.
    VariableNodeMap& operator=(const VariableNodeMap& source);

    // ===========================================================================
    // Getters and setters.
    // ===========================================================================

    // Returns a gaussian variable given its node id.
    /* @throws NotFound Raised if no nodes matches id. */
    const GaussianVariable& get(gum::NodeId id) const;

    // Returns a node id given it's variable.
    /* @throws NotFound Raised if no nodes matches var. */
    gum::NodeId get(const GaussianVariable& var) const;

    // Return true if id matches a node.
    bool exists(gum::NodeId id) const;

    // Return true if var matches a node.
    bool exists(const GaussianVariable& var) const;

    // Maps id with var.
    /*
     * @warning Var is added by copy.
     * @throws DuplicateLabel if this name already exists
     * @throws DuplicateElement if this id already exists
     * @return Returns id (useful in some case); */
    gum::NodeId insert(gum::NodeId id, const GaussianVariable& var);

    // Removes a var and its id of this mapping. The pointer is deleted.
    /* @throws NotFound Raised if no nodes matches id. */
    void erase(gum::NodeId id);

    // Removes a var and its id of this mapping. The pointer is deleted.
    /* @throws NotFound Raised if no nodes matches id. */
    void erase(const GaussianVariable& var);

    // we allow the user to change the name of a variable
    /* @throws DuplicateLabel if this name already exists
     * @throws NotFound Raised if no nodes matches id. */
    void changeName(gum::NodeId id, const std::string& new_name);

    // removes all the associations
    void clear();

    // give the size
    gum::Size size() const;

    // friendly displays the content of the VariableNodeMap
    std::string toString() const;

    /*
     * Returns the name of a variable given its id.
     * @param id The variable's id.
     * @return id's name.
     * @throw NotFound Raised if no variable matches id.
     */
    const std::string& name(gum::NodeId id) const;

    /*
     * Returns the name of a variable.
     * @param var The variable.
     * @return var's name.
     * @throw NotFound Raised if var is not in this VariableNodeMap.
     */
    const std::string& name(const GaussianVariable& var) const;

    // ===========================================================================
    // Operators.
    // ===========================================================================

    // Returns a Gaussian variable given its node id.
    /* @throws NotFound Raised if no nodes matches id. */
    const GaussianVariable& operator[](gum::NodeId id) const;

    // Returns a node id given it's variable.
    /* @throws NotFound Raised if no nodes matches var. */
    gum::NodeId operator[](const GaussianVariable& var) const;

    // ===========================================================================
    // Accessor by name
    // ===========================================================================

    // @throw NotFound if no such name exists in the graph.
    gum::NodeId                  idFromName(const std::string& name) const;
    const GaussianVariable& variableFromName(const std::string& name) const;

    private:
    // effectively do the copy (for copy constructor or operator=)
    void _copy_(const VariableNodeMap& source);

    // Bijection between the node's NodeIds and the variables.
    gum::Bijection< gum::NodeId, const GaussianVariable* > _nodes2vars_;

    /// HashTable for easely find an id from a name
    gum::Bijection< std::string, gum::NodeId > _names2nodes_;
  };

  // for friendly displaying the content of clique graphs

  std::ostream& operator<<(std::ostream&, const VariableNodeMap&);

} /* namespace OTAGRUM */

#endif /* VARIABLE_NODE_MAP_H*/
