#include <agrum/tools/multidim/potential.h>
#include "otagrum/VariableElimination.hxx"

using namespace std;

void sum_product_eliminate_continuous_var(
        vector<gum::Potential< CanonicalForm > > &cf_set,
        ContinuousVariable &variable){

    // Looking for potentials containing the variable to eliminate
    vector<gum::Potential< CanonicalForm > > contains_var;
    vector<gum::Potential< CanonicalForm > > not_contains_var;
    for(auto it=cf_set.begin(); it!=cf_set.end(); ++it){
        gum::Instantiation I(*it);
        if(it->get(I).getScope().contains(variable)){
            contains_var.push_back(*it);
        }
        else{
            not_contains_var.push_back(*it);
        }

    }

    // Multiplying potentials containing the variable to eliminate
    gum::Potential<CanonicalForm> product;
    for(auto cf : contains_var){
        product *= cf;
    }

    // Eliminating the variable from the product
    if(!contains_var.empty()){
        gum::Instantiation I(product);
        for(int i=0; i != product.domainSize() ; ++i){
            product.set(I, product.get(I).marginal(Scope({variable})));
            I.inc();
        }
        not_contains_var.push_back(product);
    }

    // Updating (in place) the set of potentials
    cf_set = not_contains_var;
}

void sum_product_eliminate_discrete_var(
        vector<gum::Potential< CanonicalForm > > &cf_set,
        gum::LabelizedVariable &variable){
    // TO FINISH: it should be done following the procedure described in 
    // Propagation of probabilities, Means, and variances in Mixed Graphical
    // Association Models, Steffen L. Lauritzen (1992)
    
    // Looking for potentials containing the variable to eliminate
    vector<gum::Potential< CanonicalForm > > contains_var;
    vector<gum::Potential< CanonicalForm > > not_contains_var;
    for(auto cf : cf_set){
        if(cf.contains(variable)){
            contains_var.push_back(cf);
        }
        else{
            not_contains_var.push_back(cf);
        }
    }

    // Multiplying potentials containing the variable to eliminate
    gum::Potential<CanonicalForm> product;
    for(auto cf : contains_var){
        product *= cf;
    }

    // Eliminating the variable from the product
    if(!contains_var.empty()){
        gum::Potential< CanonicalForm > marginal;
        for(const auto& v : product.variablesSequence()){
            if(*v != variable){
                marginal.add(*v);
            }
        }
        gum::Instantiation I(marginal);

        for(int i=0; i<marginal.domainSize(); ++i){
            auto extracted_pot = product.extract(I);
            cout << "Extracted : " << extracted_pot << endl;
            CanonicalForm sum; // Plutot une gaussienne ?
            gum::Instantiation I_ext(extracted_pot);
            for(int j=0; j<extracted_pot.domainSize(); ++j){
                cout << "Do the job" << endl;
                I_ext.inc();
            }
            marginal.set(I, sum);
            I.inc();
        }
        not_contains_var.push_back(marginal);
    }
    cf_set = not_contains_var;
}

void reduce_discrete_var(vector< gum::Potential< CanonicalForm > > &pot_set,
                         pair<gum::LabelizedVariable*, string> anEvidence){

    vector< gum::Potential< CanonicalForm > > reduced_potential_set;

    for(auto& p : pot_set){
        if(p.contains(*(anEvidence.first))){
            gum::Instantiation I;
            I << *(anEvidence.first);
            I.chgVal((anEvidence.first)->name(), anEvidence.second);
            if(p.nbrDim() == 1){
                gum::Potential< CanonicalForm > new_pot;
                gum::Instantiation I_new(new_pot);
                new_pot.set(I_new, p.get(I));
                reduced_potential_set.push_back(new_pot);
                p = new_pot;
            }
            else{
                p = p.extract(I);
            }
        }
    }
    //cout << "Reduced : " << reduced_potential_set << endl;
}

gum::Potential<CanonicalForm> sum_product_ve(
        vector<ContinuousVariable> &continuous_elim_order,
        vector< gum::LabelizedVariable*> &discrete_elim_order,
        ContinuousEvidence &continuous_evidence,
        DiscreteEvidence &discrete_evidence,
        vector< gum::Potential< CanonicalForm> > potential_set){

    vector<ContinuousVariable> red_cont_vars;
    for(auto x : continuous_elim_order){
        red_cont_vars.push_back(x);
    }

    vector<gum::LabelizedVariable> red_disc_vars;
    for(auto x : discrete_elim_order){
        red_disc_vars.push_back(*x);
    }

    //cout << "pot avant : " << potential_set << endl;

    if(!continuous_evidence.empty()){
        for(auto it = potential_set.begin(); it!=potential_set.end(); ++it){
            gum::Instantiation I(*it);
            for(int i=0; i != it->domainSize() ; ++i){
                it->set(I, it->get(I).reduce(continuous_evidence));
                I.inc();
            }
        }
    }

    //cout << "pot après : " << potential_set << endl;

    if(!discrete_evidence.empty()){
        for(auto& de : discrete_evidence){
            reduce_discrete_var(potential_set, de);
        }
    }

    if(!continuous_elim_order.empty()){
        for(auto& v : continuous_elim_order){
            sum_product_eliminate_continuous_var(potential_set, v);
        }
    }

    if(!discrete_elim_order.empty()){
        for(auto& v : discrete_elim_order){
            sum_product_eliminate_discrete_var(potential_set, *v);
        }
    }

    gum::Potential<CanonicalForm> product;
    for(auto& p : potential_set){
        product *= p;
    }
    
    return product;
}
