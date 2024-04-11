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
        // Constructor
        ContinuousVariable(std::string name,
                           std::string desc=""): name(name),
                                                 desc(desc),
                                                 id(count) {count++;}

        // Copy constructor
        ContinuousVariable(const ContinuousVariable&) = default;
        // Copy operator
        ContinuousVariable& operator=(const ContinuousVariable &rhs) = default;

        // Move constructor
        ContinuousVariable(ContinuousVariable&&) = default;
        // Move operator
        ContinuousVariable& operator=(ContinuousVariable &&rhs) = default;

        // Destructor
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
