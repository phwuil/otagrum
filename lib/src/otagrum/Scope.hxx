#ifndef SCOPE_H
#define SCOPE_H

#include <iostream>
#include <vector>
#include <initializer_list>
#include "ContinuousVariable.hxx"

class Scope {
    friend std::ostream& operator<<(std::ostream& os, const Scope& item);
    friend Scope operator+(const Scope &lhs, const Scope &rhs);
    friend Scope operator-(const Scope &lhs, const Scope &rhs);

    friend bool operator==(const Scope &lhs, const Scope &rhs);
    friend bool operator!=(const Scope &lhs, const Scope &rhs);

    public:
        // Constructeur
        Scope() = default;
        Scope(std::vector<ContinuousVariable>& variables);
        Scope(std::initializer_list<ContinuousVariable> l);

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
        const std::vector<ContinuousVariable>& getVariables() const {
            return variables;
        }
        const unsigned int& getSize() const {return size;}

        bool contains(const ContinuousVariable& item) const;
        bool isEmpty() { return variables.empty(); }

        // Ajoute une variable dans le scope en retournant la position
        // a laquelle elle a été ajoutée dans le vecteur trié.
        int addVariable(ContinuousVariable variable);
        int addVariables(Scope scope);

        Scope& operator+=(const Scope &rhs);
        Scope& operator-=(const Scope &rhs);

        
    private:
        std::vector<ContinuousVariable> variables;
        unsigned int size = 0;
};
        
std::ostream& operator<<(std::ostream& os, const Scope& item);
Scope operator+(const Scope &lhs, const Scope &rhs);
Scope operator-(const Scope &lhs, const Scope &rhs);
bool operator==(const Scope &lhs, const Scope &rhs);
bool operator!=(const Scope &lhs, const Scope &rhs);

#endif // SCOPE_H
