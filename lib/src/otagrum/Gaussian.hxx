#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <iostream>
#include <Eigen/Dense>

class Gaussian;
#include "otagrum/Scope.hxx"
#include "otagrum/CanonicalForm.hxx"

class Gaussian {
    friend class CanonicalForm;
    friend std::ostream& operator<<(std::ostream& os, const Gaussian& item);
    friend Gaussian operator+(const Gaussian &lhs, const Gaussian &rhs);

    public:
        // Constructeurs
        Gaussian() = default;
        Gaussian(Scope scope, Eigen::MatrixXd Sigma, Eigen::VectorXd mu);
        Gaussian(CanonicalForm CF);
        Gaussian(double proba): p(proba) {}

        // Constructeur de copy
        Gaussian(const Gaussian& item) = default;
        // Operateur de copy
        Gaussian& operator=(const Gaussian &rhs) = default;

        // Constructeur de move
        Gaussian(Gaussian&& item) = default;
        // Operateur de move
        Gaussian& operator=(Gaussian&& rhs) = default;

        // Destructeur
        ~Gaussian() {}

        Gaussian& operator+=(const Gaussian &rhs);

    private:
        Scope scope;
        Eigen::MatrixXd Sigma;
        Eigen::VectorXd mu;
        double c = 1.;
        double p = 1.;
};

std::ostream& operator<<(std::ostream& os, const Gaussian& item);

#endif // GAUSSIAN_H
