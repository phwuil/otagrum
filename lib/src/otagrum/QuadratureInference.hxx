#ifndef OTAGRUM_QUADRATUREINFERENCE_HXX
#define OTAGRUM_QUADRATUREINFERENCE_HXX

//#include <agrum/tools/graphs/undiGraph.h>
//#include <agrum/tools/graphs/mixedGraph.h>
//#include <agrum/tools/core/heap.h>
//#include <agrum/tools/core/hashTable.h>

//#include <openturns/Sample.hxx>

#include "otagrum/ContinuousBayesianNetwork.hxx"
//#include "otagrum/CorrectedMutualInformation.hxx"
//#include "otagrum/IndicesManip.hxx"

namespace OTAGRUM
{

#ifndef SWIG
class GreaterPairOn2nd
{
public:
  bool operator()(
    const std::pair <
    std::tuple< OT::UnsignedInteger,
    OT::UnsignedInteger,
    OT::UnsignedInteger,
    OT::Indices >*,
    double > & e1,
    const std::pair <
    std::tuple< OT::UnsignedInteger,
    OT::UnsignedInteger,
    OT::UnsignedInteger,
    OT::Indices >*,
    double > & e2) const;
};
#endif

class OTAGRUM_API QuadratureInference : public OT::Object
{
public:
  // Constructor
  QuadratureInference(const ContinuousBayesianNetwork,
                      const int n_nodes=25,
                      const double epsilon=1e-8,
                      const int n_iterations=1,
                      const bool verbose=false);

  std::vector< gum::Arc > getLatentVariables() const;

  void setVerbosity(bool verbose);
  bool getVerbosity() const;

private:
  void initiation();
  void iteration();
  void findBestContributor(const OT::UnsignedInteger X,
                           const OT::UnsignedInteger Y,
                           const OT::Indices &U);
  std::vector< std::tuple< std::tuple< OT::UnsignedInteger,
      OT::UnsignedInteger,
      OT::UnsignedInteger >*,
      double,
      double,
      double > >
      getUnshieldedTriples(const gum::MixedGraph& graph,
                           gum::HashTable< std::pair< gum::NodeId, gum::NodeId >, char >& marks);

  bool verbose_ = false;

  bool skeleton_done_ = false;
  bool pdag_done_ = false;
  bool dag_done_ = false;

  CorrectedMutualInformation info_;

  gum::HashTable< std::pair< gum::NodeId, gum::NodeId >, char > initial_marks_;
  std::vector< gum::Arc > latent_couples_;
  gum::ArcProperty< double > arc_probas_;
  gum::EdgeProperty< OT::Indices > sepset_;
  gum::Heap <
  std::pair< std::tuple< OT::UnsignedInteger,
      OT::UnsignedInteger,
      OT::UnsignedInteger,
      OT::Indices >*, double >,
      GreaterPairOn2nd > rank_;

  gum::UndiGraph skeleton_;
  gum::MixedGraph pdag_;
  NamedDAG dag_;
};

}

#endif // OTAGRUM_CONTINUOUSMIIC_HXX
