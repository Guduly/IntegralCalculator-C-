/**
    The Term Class represents the coefcients and exponents of the integral
*/
#ifndef TERM_H
#define TERM_H
#include<iostream>
#include<climits>
using namespace std;

class Term{
    private:
    /// exponent and coefcients respectively
        int exp;
        int coe;
        bool trig;
        int Tcoe;
        string Tterm;
    public: 
        /// default contruc
        Term(){
            coe = 1; 
            exp = 0;
            trig = false;
            Tcoe = 1;
            Tterm = "";
        }
        /// overloaded contruc
        Term(int c, int e){
            exp = e;
            coe = c;
            trig = false;
            Tcoe = 1;
            Tterm = "";
        }
        Term(string val, int incoef){
            Tterm = val;
            Tcoe = incoef;
            trig = true;
            coe = 1; 
            exp = INT_MIN;
        }

        bool isTrig() const{
            return trig;
        }

        int getTcoe() const{
            return Tcoe;
        }

        string getTrig() const{
            return Tterm;
        }

        /// returns coe
        int getCoe() const{
            return coe;
        }

        /// returns exp
        int getExp() const{
            return exp;
        }
        /// sets coe
        void setCoe(int c){
            coe = c;
        }
        // sets exp
        void setExp(int e){
            exp = e;
        }

        /// overloaded operators
        friend std::ostream& operator<<(std::ostream& out, const Term& t);
        bool operator>(const Term& t);
        bool operator<(const Term& t);        
};

#endif
