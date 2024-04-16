#ifndef CONTINUOUS_VARIABLE_H
#define CONTINUOUS_VARIABLE_H

#include <iostream>

class ContinuousVariable {
    friend std::ostream& operator<<(std::ostream& os, const ContinuousVariable& item);
    friend bool operator==(const ContinuousVariable &lhs,
                           const ContinuousVariable &rhs);
    friend bool operator<(const ContinuousVariable &lhs,
                          const ContinuousVariable &rhs);
    friend bool operator<=(const ContinuousVariable &lhs,
                           const ContinuousVariable &rhs);
    friend bool operator>(const ContinuousVariable &lhs,
                          const ContinuousVariable &rhs);
    friend bool operator>=(const ContinuousVariable &lhs,
                           const ContinuousVariable &rhs);

    public:
        // Constructeur
        ContinuousVariable(std::string name,
                           std::string desc=""): name(name),
                                                 desc(desc),
                                                 id(count) {count++;}

        // Constructeur de copy
        ContinuousVariable(const ContinuousVariable&) = default;
        // Operateur de copy
        ContinuousVariable& operator=(const ContinuousVariable &rhs) = default;

        // Constructeur de move
        ContinuousVariable(ContinuousVariable&&) = default;
        // Operateur de move
        ContinuousVariable& operator=(ContinuousVariable &&rhs) = default;

        // Destructeur
        ~ContinuousVariable() {}

        int getId() const {return id;}
        std::string getName() const { return name; }
        std::string getDesc() const { return desc; }

    private:
        static int count;

        std::string name;
        std::string desc;
        int id;
        
};

std::ostream& operator<<(std::ostream& os, const ContinuousVariable& item);

bool operator==(const ContinuousVariable &lhs, const ContinuousVariable &rhs);
bool operator<(const ContinuousVariable &lhs, const ContinuousVariable &rhs);
bool operator<=(const ContinuousVariable &lhs, const ContinuousVariable &rhs);
bool operator>(const ContinuousVariable &lhs, const ContinuousVariable &rhs);
bool operator>=(const ContinuousVariable &lhs, const ContinuousVariable &rhs);

#endif // CONTINUOUS_VARIABLE_H
