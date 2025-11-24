#include "Term.h"
#include<iostream>
#include<string>
using namespace std;

/**
@return: bool
@param: const Node<X>& n
Compares and returns true if our term is greater than parameter
*/
bool Term::operator>(const Term& t){
    // comparision by exponents, since they are most important factor in integration
    return exp > t.exp;
}

/**
@return: bool
@param: const Node<X>& n
Compares and returns true if our term is less than parameter
*/
bool Term::operator<(const Term& t){
    // comparision by exponents, since they are most important factor in integration
    return exp < t.exp;
}

/**
    @param: std::ostream& out, const Node<X>& n
    @return: std::ostream&
    formats and returns output stream 
*/
std::ostream& operator<<(std::ostream& out, const Term& t){
    // output order: 3x^2

    if(t.exp == 0)  out << t.coe;
    else if(t.exp >= 1){
        /// since formatting is graded. IE X is correct and not 1x
        if(t.exp == 1){
            if(t.coe == 1)
                out << "x";
            // minus check
            else if(t.coe == -1)
                out << "-x";
            else out << to_string(t.coe);
        }else{
            if(t.coe == 1)
                out << "x^" << to_string(t.exp);
            // minus check
            else if(t.coe == -1)
                out << "-x^" << to_string(t.exp);
            else out << to_string(t.coe) << "x^" << to_string(t.exp);
        }
    }
    return out;
}

