#ifndef SCOPE_H
#define SCOPE_H

#include <iostream>
#include <vector>
#include <initializer_list>

#include "otagrum/GaussianVariable.hxx"

class Scope {
    friend std::ostream& operator<<(std::ostream& os, const Scope& item);
    friend Scope operator+(const Scope &lhs, const Scope &rhs);
    friend Scope operator-(const Scope &lhs, const Scope &rhs);

    friend bool operator==(const Scope &lhs, const Scope &rhs);
    friend bool operator!=(const Scope &lhs, const Scope &rhs);

    public:
        // Constructeur
        Scope() = default;
        Scope(std::vector<GaussianVariable>& variables);
        Scope(std::initializer_list<GaussianVariable> l);

        // Constucteur de copy
        Scope(const Scope& item) = default;
        // Operateur de copy
        Scope& operator=(const Scope& rhs) = default;

        // Constucteur de move
        Scope(Scope&& item) = default;
        // Operateur de move
        Scope& operator=(Scope&& rhs) = default;

        // Destructeur
        ~Scope() {}

        // Accesseurs
        const std::vector<GaussianVariable>& getVariables() const {
            return _variables_;
        }
        const unsigned int& getSize() const {return _size_;}

        bool contains(const GaussianVariable& item) const;
        bool isEmpty() { return _variables_.empty(); }

        // Ajoute une variable dans le scope en retournant la position
        // a laquelle elle a été ajoutée dans le vecteur trié.
        int addVariable(GaussianVariable variable);
        int addVariables(Scope scope);

        Scope& operator+=(const Scope &rhs);
        Scope& operator-=(const Scope &rhs);

        
    private:
        std::vector<GaussianVariable> _variables_;
        unsigned int _size_ = 0;
};
        
std::ostream& operator<<(std::ostream& os, const Scope& item);
Scope operator+(const Scope &lhs, const Scope &rhs);
Scope operator-(const Scope &lhs, const Scope &rhs);
bool operator==(const Scope &lhs, const Scope &rhs);
bool operator!=(const Scope &lhs, const Scope &rhs);

#endif // SCOPE_H
