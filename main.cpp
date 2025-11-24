using namespace std;
#include<iostream>
#include<fstream>
#include<string>
#include <iomanip>
#include<vector>
#include<sstream>
#include<algorithm>
#include "Term.h"
#include "Node.h"
#include "BinTree.h"
#include <cctype>
#include<numeric>
#include<cmath>

//// identifies trig type and manages position, ie i
void findTrig(string& poly, size_t& i, int& inner, string& type ){
    inner = 1;
    type = "";

    if(i+3 <= poly.length() && poly.substr(i,3) == "cos"){
        type = "cos";
        i += 3;
    }else if(i+3 <= poly.length() && poly.substr(i,3) == "sin"){
        type = "sin";
        i += 3;
    }
    
    while(i < poly.length() && poly[i] == ' ')
        ++i;
    
    size_t st = i;
    if(st < poly.length() && poly[i] == '-')
        ++i;
    
    while(i < poly.length() && isdigit(poly[i]))
        ++i;
    
    if(i > st && poly[st] != 'x')
        inner = stoi(poly.substr(st, i-st));

    if(i < poly.length() && poly[i] == 'x') ++i;
}

/// finds all the trig terms & removes them so that only poly terms remain
void getAllTrig(string& poly, vector<Term>& trig){
    string res = "";
    size_t i = 0;
    while(i < poly.length()){
        /// getting inner and outer vars
        if(i+3 <= poly.length() && (poly.substr(i,3) == "sin" || poly.substr(i,3) == "cos")){
            int inner = 1;
            string type;
            int outer = 1;
            size_t Ocoe = i;
            while(Ocoe > 0 && poly[Ocoe-1] == ' ') --Ocoe;
            size_t Ostar = Ocoe;
            while(Ostar > 0 && isdigit(poly[Ostar-1])) --Ostar;
            /// finds start and end of the coef
            if(Ostar < Ocoe){
                /// filters out useless stuff
                string filter = poly.substr(Ostar, Ocoe - Ostar);
                filter.erase(remove(filter.begin(), filter.end(), ' '), filter.end());
                if(filter == "" || filter == "+") outer = 1;
                else if(filter == "-") outer = -1;
                else outer = stoi(filter);

                if(res.length() >= (Ocoe - Ostar))
                    res = res.substr(0, res.length() - (Ocoe - Ostar));
            }
            while(i < poly.length() && poly[i] == ' ')++i;
            if(i < poly.length() && poly[i] == 'x')++i;
            //// find and update term
            findTrig(poly, i,inner, type);
            if(type != ""){
                Term t(type, inner);
                t.setCoe(outer);
                trig.push_back(t);
            }
            /// manage pos, ie i
            while(i < poly.length() && (poly[i] == ' ' || poly[i] == '+' || poly[i] == '-')){
                if(poly[i] == '+' || poly[i] == '-'){
                    if(i+1 < poly.length() && (poly.substr(i+1,3) == "sin" || poly.substr(i+1,3) == "cos")){
                        i++;
                        while(i < poly.length() && poly[i] == ' ') ++i;
                    }else break;
                }else ++i;
            }
        }else{
            res += poly[i];
            i++;
        }
    }
    poly = res;
}


/**
Curr problems: getting trig terms form problem
store all trig terms in vector
insert all trig terms
Integrate with vector trig terms
*/
/***
@param: BinTree<Term>& tree, String poly
Inserts term values onto the tree from the poly string
It checks if the problem is definite or not and sends the problem
to the relevant method. 
*/
void populate(BinTree<Term>& tree, string& poly, vector<Term>& trig){
    
    getAllTrig(poly, trig);
    for(Term t: trig){
        /// added checks if either nodes are trig to prevent combining
        tree.insert(t);
    }
    /// remove dx
    string exp = poly;
    size_t dx = exp.find("dx");
    if(dx == string::npos) return;
    else{
        exp = exp.substr(0, dx);
        //exp.erase(remove(exp.begin(), exp.end(), ' '), exp.end());
    }
    /// get the terms and add to tree
    /// spaces are delimiters??
    string curr = "";
    bool ini = false;

    for(size_t i = 0; i < exp.length();i++){
        /// check for pos & neg
        if((exp[i] == '-' || exp[i] == '+') && ( i == 0|| exp[i-1] != '^')){
            if(ini && (curr.length() != 0)){
                int coe = 1;
                int ex = 1;
                /// should a create a separate Func???
                /// find coe & exponent
                string term = curr;
                size_t x = term.find("x");
                if(x == string::npos){
                    ex = 0;
                    /// double checking term
                    if(term.length() > 0 && term != "+" && term != "-")
                        coe = stoi(term);
                    /// tree insertion after everything is checked
                    tree.insert(Term(coe, ex));
                    curr = "";
                    if(exp[i] == '-') curr = "-";
                    ini = true;
                    continue;
                }
                /// meaning 1x^???
                if(x == 0)
                    coe = 1;
                else if(term[0] == '-' && x==1) coe = -1;
                else if(term[0] == '+' && x==1) coe = 1;
                else{
                    /// situation where coe != 1 or we are looking at a middle term
                    string fCoe = curr.substr(0, x);
                    if(fCoe == "+" || (fCoe.length() == 0)) coe = 1;
                    else if(fCoe == "-") coe = -1;
                    else if(fCoe.length() != 0) coe = stoi(fCoe);
                }
                /// find exp
                size_t carr = curr.find("^");
                if(carr == string::npos){
                    ex = 1;
                }else{
                    if(curr.substr(carr+1).length() != 0)
                        ex = stoi(curr.substr(carr+1));
                }
                tree.insert(Term(coe, ex));
                curr = "";
            }
            if(exp[i] == '-') curr = "-";
            ini = true;
        }else if(exp[i] != ' '){
            curr += exp[i];
            ini = true;
        }
    }
    // get last term
    if(curr.length() != 0){
        int coe = 1;
        int ex = 1;
        /// should a create a separate Func???
        /// find coe & exponent
        string term = curr;
        size_t x = term.find("x");
        if(x == string::npos){
            ex = 0;
            if(term.length() > 0 && term != "+" && term != "-")
                coe = stoi(term);
            tree.insert(Term(coe, ex));
            return;
        }
        /// meaning 1x^???
        if(x == 0)
            coe = 1;
        else if(term[0] == '-' && x==1) coe = -1;
        else if(term[0] == '+' && x==1) coe = 1;
        else{
            /// situation where coe != 1 or we are looking at a middle term
            string fCoe = curr.substr(0, x);
            if(fCoe == "+" || (fCoe.length() == 0)) coe = 1;
            else if(fCoe == "-") coe = -1;
            else if (fCoe.length() > 0) coe = stoi(fCoe);
        }
        /// find exp
        size_t carr = curr.find("^");
        if(carr == string::npos){
            ex = 1;
        }else{
            ex = stoi(curr.substr(carr+1));
        }
        
        tree.insert(Term(coe, ex));
    }
}
/**
@return: (String) formats it based on fraction scenario or not
@param: int num, int den
The numerator and denomenator are used to check if num | den
if so, whole numbers are outputed
else fractional output 
*/
string Frac(int num, int den){
    /// gcd method is part of numeric lib
    int g = gcd(abs(num), abs(den));
    /// reducing terms
    num /= g;
    den /= g;

    if(den < 0){
        num = -num;
        den = -den;
    }
    /// whole number check
    if(num % den == 0) return to_string(num/den);
    return ("(" + to_string(num) + "/" + to_string(den) + ")");
}
/***
@return: (string)
@param: Term& org
Returns fully formatted Integrated term
*/
string Integrated(Term& org){
    /// getting vals
    int coe = org.getCoe();
    int nexp = org.getExp()+1;
    
    Term inter(coe, nexp);
    if(coe == 0) return "";
    string out = "";
    /// for operator << in Term
    ostringstream oss;
    string format;
    bool ini = true;
    if(nexp == 0){
        /// uses << operator
        oss << inter;
        out += oss.str();
    }else{
        
        format = Frac(coe, nexp);
        /// no need for exponent case
        if(nexp == 1){
            if(coe == nexp || coe == -nexp){
                oss << inter;
                out+= oss.str();
            }else out += (format+"x");

        }else{
            /// coe is divided out
            if(coe == nexp){
                out = "x^" + to_string(nexp);
            /// negative check
            }else if(coe == -nexp) out = "-x^" + to_string(nexp);
            else out = (format + "x^" + to_string(nexp));
        }
    }

    return out;
}

string IntegratedTrig(Term& trig){
    string type = trig.getTrig();
    int inner = trig.getTcoe();
    int outer = trig.getCoe();
    string res = "";

    if(type == "cos"){
        string coe = Frac(outer,inner);
        if(coe == "1") res += "sin ";
        else res += ("-" + coe + "sin ");
        /*
        else if(coe == "-1") res += "-sin ";
        else res += (coe + "sin ");
        */
        (inner == 1) ? res += "x" : res += (to_string(inner) + "x");
        // no worries about exponent 
    }else if(type == "sin"){
        string coe = Frac(outer,inner);
        if(coe == "1") res+= "-cos ";
        else res += ("-" + coe + "cos ");

        (inner == 1) ? res += "x" : res += (to_string(inner) + "x");
        // no worries about exponent 
    }
    return res;
}

/// definite integral calcualtion
/**
@param: int, int, BinTree<Term>& tree
The first two parameters contains the low and high bounds of the problem
The last parameter is the Binary Tree, which contains all the ordered terms.
Utilizes function calls to update integrate and format output for a definite integral
*/
void calcDef(int low, int hig, BinTree<Term>& tree){
    /// inorder returns a vector in asending exp order
    vector<Term> terms = tree.inorder();
    /// sorting based on exponents using lambda functions since its more efficient 
    sort(terms.begin(), terms.end(), [](Term& a, const Term& b){
        return a > b;
    });
    /// double for precision
    /// represents upper and lower bound calculated vals
    double upper = 0;
    double lower = 0;
    string out = "";
    bool ini = true;
    bool z = true;

    for(Term& t: terms){
        /// getting vals
        int coe = t.getCoe();
        int exp = t.getExp();
        int nexp = exp+1;
        if(coe == 0) continue;
        /// contains integrated term
        string form = Integrated(t);
        if(form.length() == 0)continue;

        z = false;
        /// separate check for initial term
        if(ini){
            out += form;
            ini = false;
        }else{
            if(form[0] == '-'){
                out += (" - " + form.substr(1));
            }else{
                /// checking frac pos
                if(form[0] == '('){
                    /// restrucures fraction if its a middle term, since they are shown differently compared to first term
                    size_t divi = form.find('/');
                    if(divi != string::npos){
                        string beforeD = form.substr(1, divi - 1);
                        if(beforeD.length() > 0 && beforeD[0] == '-'){
                            /// num
                            string num = beforeD.substr(1); 
                            //size_t close = form.find(')');
                            string rest = form.substr(divi + 1);
                            out += " - (" + num + "/" + rest;
                        }else out += " + " + form;
                        
                    }else
                        out += " + " + form;
                    
                }else if(form[0] == '-') out += (" - " + form.substr(1));
                else out += (" + " + form);
                
            }
        }
        /// calculates values at each term
        double val = static_cast<double>(coe)/static_cast<double>(nexp);
        upper += val * pow(static_cast<double>(hig), static_cast<double>(nexp));
        lower += val * pow(static_cast<double>(low), static_cast<double>(nexp));
    }
    double fin = upper-lower;
    /// if we are given no terms, just output 0
    if(z) cout << 0 << ", " << low<<"|" << hig << " = 0\n";
    else{
        cout << out << ", " << low<<"|" << hig << " = " << fixed << setprecision(3)<< fin << endl;
    }
}

/***
@param (BinTree<Term>& tree)
Looks into the Tree and uses the term to build and output a integrated term
*/
void indef(BinTree<Term>& tree, vector<Term>& trig){
    vector<Term> terms = tree.inorder();
    if(terms.empty()){
        cout << "0 + C" << endl;
        return;
    }
    /// sorting based on exponents usign lambda functions since its more efficient
    sort(terms.begin(), terms.end(), [](Term& a, const Term& b){
        return a > b;
    });

    // iniital term
    bool ini = true;
    ///all zero??
    bool z = true;
    string out = "";

    for(Term& t: terms){
        /// just in case, some trig stuff come to poly string
        if(t.isTrig()) continue;

        string form = Integrated(t);
        if(form.length() == 0)continue;

        z = false;

        if(ini){
            out += form;
            ini = false;
        }else{
            if(form[0] == '-'){
                out += (" - " + form.substr(1));
            }else{
                /// same process as definite integral function for this part
                /// checking frac pos
                if(form[0] == '('){
                    /// again resturstres frac if its in the middle
                    size_t divi = form.find('/');
                    if(divi != string::npos){
                        string beforeD = form.substr(1, divi - 1);
                        if(beforeD.length() > 0 && beforeD[0] == '-'){
                            /// num
                            string num = beforeD.substr(1); 
                            //size_t close = form.find(')');
                            string rest = form.substr(divi + 1);
                            out += " - (" + num + "/" + rest;
                        }else out += " + " + form;
                        
                    }else
                        out += " + " + form;
                    
                }else if(form[0] == '-') out += (" - " + form.substr(1));
                else out += (" + " + form);
            }
        }
    }

    for(Term t: trig){
        string form = IntegratedTrig(t);
        z = false;

        if(ini){
            out += form;
            ini = false;
        }else{
            if(form[0] == '-') out += (" - " + form.substr(1));
            else{
                if(form[0] == '('){
                    /// again resturstres frac if its in the middle
                size_t divi = form.find('/');
                if(divi != string::npos){
                    string beforeD = form.substr(1, divi - 1);
                    if(beforeD.length() > 0 && beforeD[0] == '-'){
                        /// num
                        string num = beforeD.substr(1); 
                        //size_t close = form.find(')');
                        string rest = form.substr(divi + 1);
                        out += " - (" + to_string(abs(stoi(num))) + "/" + rest;
                    }else out += " + " + form;
                    
                }else
                    out += " + " + form;
                    
                }else if(form[0] == '-') out += (" - " + form.substr(1));
                else out += (" + " + form);
            }
        }
    }

    // if all terms cancel out
    if(z) cout << "0 + C" << endl;
    else cout << out << " + C" << endl;
}

/// Process line
/**
@param: const string& l
Process the given input and filters out | 
to find bounds and build the BST.
*/
void processing(const string& l){
    if(l.length() == 0) return;
    /// check if integration symbol | exists
    size_t Ipos = l.find("|");
    if(l.find("|") == string::npos)return;
    /// for definite integral
    bool def = false;
    int low = 0;
    int hig = 0;
    string poly;

    /// definite scenario, since it means there a low bound
    if(Ipos > 0){
        string store = l.substr(0, Ipos);
        /// removes all instances of spaces
        store.erase(remove(store.begin(), store.end(), ' '), store.end());
        if(!(store.length() == 0) && (store[0] == '-' || isdigit(store[0]))){
            /// once lower is found, def scenario confirmed
            def = true;
            /// stoi converts string to int
            low = stoi(store);

            /// find upper bound
            int storeM = Ipos+1;
            string rest = l.substr(storeM);
            /// generally size_t is used compared to int for string, since int causes error or warnigns
            size_t i = 0;
            while(i < rest.length() && (rest[i] == ' '))
                ++i;
            size_t st = i;
            if(i < rest.length() && rest[i] == '-')
                ++i;
            while(i < rest.length() && isdigit(rest[i])) ++i;
            string upper = rest.substr(0,i);
            upper.erase(remove(upper.begin(), upper.end(), ' '), upper.end());
            /// converts string to int
            hig = stoi(upper);

            /// rest of the terms for integratio
            poly = rest.substr(i);
        }
    }else{
        poly = l.substr(Ipos+1);
    }

    //cout << poly << endl;

    BinTree<Term> tree;
    vector<Term> trig;
    /// populate the tree
    populate(tree, poly, trig);
    /// calcIntegral based on definite or not
    if(!def) indef(tree, trig);
    else calcDef(low, hig, tree);

}

int main(){
    ifstream input;
    string file;
    cin >> file;
    input.open(file);
    if(!input.is_open()){
        cout << "CANNOT OPEN FILE\n";
    }

    /// process input lines
    string line;
    while(getline(input, line)){
        /**
        Process line
            -> definite or indefinite
            -> Create binary tree, Node, and Insert Term vals
            -> Integrate per term
            -> Based on problem, do indef or def
            -> format and return final output
        */
        processing(line);
    }

    input.close();
}
