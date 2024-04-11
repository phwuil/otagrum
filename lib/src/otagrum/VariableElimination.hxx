#ifndef VARIABLE_ELIMINATION_H
#define VARIABLE_ELIMINATION_H

#include <vector>
#include <map>
#include <agrum/agrum.h>
#include <agrum/tools/multidim/potential.h>
#include <agrum/tools/variables/labelizedVariable.h>
#include "ContinuousVariable.hxx"
#include "CanonicalForm.hxx"


/**
* @brief Eliminate a continuous variable from a set of potentials
*
* @param cf_set A set of potentials
* @param variable The continuous variable to remove
*/
void sum_product_eliminate_continuous_var(
        std::vector<gum::Potential< CanonicalForm > > &cf_set,
        ContinuousVariable &variable);

/**
* @brief Eliminate a discrete variable from a set of potentials
*
* @param cf_set A set of potentials
* @param variable The discrete variable to remove
*/
void sum_product_eliminate_discrete_var(
        std::vector<gum::Potential< CanonicalForm > > &cf_set,
        gum::LabelizedVariable &variable);

/**
* @brief Reduce the potentials containing the observed discrete variable
*
* @param pot_set The set of potentials to reduce
* @param anEvidence An evidence for the observed discrete variable
*/
void reduce_discrete_var(
        std::vector< gum::Potential< CanonicalForm > > &pot_set,
        std::pair< gum::LabelizedVariable*, std::string > anEvidence);


/**
* @brief Variable elimination sum-product algorithm for CLGs
*
* @param continuous_elim_order Order on continuous variables that are eliminated
* @param discrete_elim_order Order on discrete variables that are eliminated
* @param continuous_evidence Evidence for continuous variables
* @param discrete_evidence Evidence for discrete variables
* @param cf_set Set of potentials on which the algorithm is applied
*
* @return The resulting product of potentials
*/
gum::Potential<CanonicalForm> sum_product_ve(
        std::vector<ContinuousVariable> &continuous_elim_order,
        std::vector<gum::LabelizedVariable*> &discrete_elim_order,
        ContinuousEvidence &continuous_evidence,
        DiscreteEvidence &discrete_evidence,
        std::vector< gum::Potential< CanonicalForm> > cf_set);

#endif // VARIABLE_ELIMINATION_H
