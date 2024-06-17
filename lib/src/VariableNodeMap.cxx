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

#  include <iostream>
#  include <sstream>

#  include <otagrum/VariableNodeMap.hxx>

namespace OTAGRUM {

  // Copy constructor.
  VariableNodeMap::VariableNodeMap(const VariableNodeMap& source) {
    _copy_(source);
  }

  // Destructor
  VariableNodeMap::~VariableNodeMap() {
    clear();
  }

  // Copy operator.
  VariableNodeMap& VariableNodeMap::operator=(const VariableNodeMap& source) {
    clear();
    _copy_(source);

    return *this;
  }

  void VariableNodeMap::clear() {
    for (auto iter = _nodes2vars_.begin(); iter != _nodes2vars_.end(); ++iter)
      delete iter.second();

    _nodes2vars_.clear();
    _names2nodes_.clear();
  }

  // friendly displays the content of the VariableNodeMap
  std::string VariableNodeMap::toString() const {
    std::stringstream stream;

    stream << "list of associations:" << std::endl;
    stream << _nodes2vars_ << std::endl << std::endl;
    stream << "list of variable names:" << std::endl;
    stream << _names2nodes_ << std::endl;

    return stream.str();
  }

  // do the copy
  void VariableNodeMap::_copy_(const VariableNodeMap& source) {
    for (auto iter = source._nodes2vars_.begin(); iter != source._nodes2vars_.end(); ++iter)
      _nodes2vars_.insert(iter.first(), iter.second()->clone());

    // copy factory is used inside insert

    _names2nodes_ = source._names2nodes_;
  }

  // for friendly displaying the content of clique graphs
  std::ostream& operator<<(std::ostream& stream, const VariableNodeMap& v) {
    stream << v.toString();
    return stream;
  }

  // Returns a discrete variable given it's node id.
  // @throws NotFound Raised if no nodes matches id.
  const GaussianVariable& VariableNodeMap::get(gum::NodeId id) const {
    return *(_nodes2vars_.second(id));
  }

  // Returns a node id given it's variable.
  // @throws NotFound Raised if no nodes matches var.
  gum::NodeId VariableNodeMap::get(const GaussianVariable& var) const {
    return _nodes2vars_.first(&var);
  }

  // Return true if id matches a node
  bool VariableNodeMap::exists(gum::NodeId id) const { return _nodes2vars_.existsFirst(id); }

  // Return true if var matches a node
  bool VariableNodeMap::exists(const GaussianVariable& var) const {
    return _nodes2vars_.existsSecond(&var);
  }

  // Return the size of the map
  gum::Size VariableNodeMap::size() const { return _nodes2vars_.size(); }

  // Returns a node id given it's variable.
  // @throws NotFound Raised if no nodes matches var.
  const GaussianVariable& VariableNodeMap::operator[](gum::NodeId varId) const { return get(varId); }

  // Returns a node id given it's variable.
  // @throws NotFound Raised if no nodes matches var.
  gum::NodeId VariableNodeMap::operator[](const GaussianVariable& var) const { return get(var); }

  // Maps id with var. Var is added by copy.
  // @throw DuplicateLabel if the name already exists in the mapping
  // @throw DuplicateElement if the id already exists in the mapping
  gum::NodeId VariableNodeMap::insert(gum::NodeId id, const GaussianVariable& var) {
    if (_names2nodes_.existsFirst(var.name())) {
      GUM_ERROR(gum::DuplicateLabel, "Unable to insert var with the name '" << var.name() << "'.")
    }

    if (exists(id)) {
      GUM_ERROR(gum::DuplicateElement, "Unable to insert a new variable with id " << id << ".")
    }

    _nodes2vars_.insert(id, var.clone());
    _names2nodes_.insert(var.name(), id);

    return id;
  }

  // Removes a var and it's id of this mapping. The pointer is deleted.
  void VariableNodeMap::erase(gum::NodeId id) {
    const GaussianVariable* var = _nodes2vars_.second(id);
    _names2nodes_.eraseFirst(var->name());
    delete var;
    _nodes2vars_.eraseFirst(id);
  }

  // Removes a var and it's id of this mapping. The pointer is deleted.
  void VariableNodeMap::erase(const GaussianVariable& var) {
    gum::NodeId id = _nodes2vars_.first(&var);
    erase(id);
  }

  gum::NodeId VariableNodeMap::idFromName(const std::string& name) const {
    return _names2nodes_.second(name);
  }

  const GaussianVariable& VariableNodeMap::variableFromName(const std::string& name) const {
    return *_nodes2vars_.second(idFromName(name));
  }

  // we allow the user to change the name of a variable
  // @throws DuplicateLabel if this name already exists
  // @throws NotFound Raised if no nodes matches id.
  void VariableNodeMap::changeName(gum::NodeId id, const std::string& new_name) {
    if (_names2nodes_.existsFirst(new_name)) {
      GUM_ERROR(gum::DuplicateLabel, "Unable to insert var with the name '" << new_name << "'.")
    }

    auto var = const_cast< GaussianVariable* >(_nodes2vars_.second(id));

    _names2nodes_.eraseFirst(var->name());
    var->setName(new_name);
    _names2nodes_.insert(new_name, id);
  }

  const std::string& VariableNodeMap::name(gum::NodeId id) const {
    return _names2nodes_.first(id);
  }

  const std::string& VariableNodeMap::name(const GaussianVariable& var) const {
    return _names2nodes_.first(_nodes2vars_.first(&var));
  }


} /* namespace OTAGRUM */
