#ifndef GAUSSIAN_VARIABLE_H
#define GAUSSIAN_VARIABLE_H

#include <iostream>
#include <agrum/tools/variables/continuousVariable.h>

namespace OTAGRUM {

class GaussianVariable : public gum::ContinuousVariable< double > {
    public:
        // Constructeur
        GaussianVariable(std::string name,
                         std::string description="",
                         double mu=0,
                         double sigma=1): ContinuousVariable(name, description),
                                          _mu_(mu),
                                          _sigma_(sigma) {};

        // Constructeur de copy
        GaussianVariable(const GaussianVariable&) = default;
        // Operateur de copy
        GaussianVariable& operator=(const GaussianVariable &rhs) = default;

        // Constructeur de move
        GaussianVariable(GaussianVariable&&) = default;
        // Operateur de move
        GaussianVariable& operator=(GaussianVariable &&rhs) = default;

        // Destructeur
        ~GaussianVariable() {}

        // Copy factory
        // Returns a pointer on a new copy of this.
        GaussianVariable* clone() const;

        double mu() const { return _mu_; }
        double sigma() const { return _sigma_; }

        void setMu(double mu);
        void setSigma(double sigma);

        std::string toString() const;

    private:
        double _mu_;
        double _sigma_;
};

} // namespace OTAGRUM

#endif // GAUSSIAN_VARIABLE_H
