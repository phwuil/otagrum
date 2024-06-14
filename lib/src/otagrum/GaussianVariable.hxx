#ifndef GAUSSIAN_VARIABLE_H
#define GAUSSIAN_VARIABLE_H

#include <iostream>

class GaussianVariable {
    friend std::ostream& operator<<(std::ostream& os, const GaussianVariable& item);
    friend bool operator==(const GaussianVariable &lhs,
                           const GaussianVariable &rhs);
    friend bool operator<(const GaussianVariable &lhs,
                          const GaussianVariable &rhs);
    friend bool operator<=(const GaussianVariable &lhs,
                           const GaussianVariable &rhs);
    friend bool operator>(const GaussianVariable &lhs,
                          const GaussianVariable &rhs);
    friend bool operator>=(const GaussianVariable &lhs,
                           const GaussianVariable &rhs);

    public:
        // Constructeur
        GaussianVariable(std::string name,
                         double mu=0,
                         double sigma=1,
                         std::string desc=""): _name_(name),
                                               _mu_(mu),
                                               _sigma_(sigma),
                                               _desc_(desc),
                                               _id_(_count_) {_count_++;}

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

        int getId() const {return _id_;}
        double getMu() const { return _mu_; }
        double getSigma() const { return _sigma_; }
        std::string getName() const { return _name_; }
        std::string getDesc() const { return _desc_; }

        void setName(const std::string& new_name);
        void setMu(double mu);
        void setSigma(double sigma);

    private:
        static int _count_;

        std::string _name_;
        double _mu_;
        double _sigma_;
        std::string _desc_;
        int _id_;
        
};

std::ostream& operator<<(std::ostream& os, const GaussianVariable& item);

bool operator==(const GaussianVariable &lhs, const GaussianVariable &rhs);
bool operator<(const GaussianVariable &lhs, const GaussianVariable &rhs);
bool operator<=(const GaussianVariable &lhs, const GaussianVariable &rhs);
bool operator>(const GaussianVariable &lhs, const GaussianVariable &rhs);
bool operator>=(const GaussianVariable &lhs, const GaussianVariable &rhs);

#endif // GAUSSIAN_VARIABLE_H
