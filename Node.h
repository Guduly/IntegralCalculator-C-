/**
Will Primarily be used for movement in the Bin Tree (Generic)
*/
#ifndef NODE_H
#define NODE_H
using namespace std;
#include<iostream>
#include "Term.h"

template <typename X>
class Node{
    private: 
        Node<X>* lef;
        Node<X>* rig;
        X dat;
    public: 
        /// default contruc
        Node(){
            lef = nullptr;
            rig = nullptr;
            /// calls default contructor
            dat = X();
        }
        /// overloaded contruc
        Node(const X& d){
            lef = nullptr;
            rig = nullptr;
            dat = d;
        }
        /// sets Data v
        void setDat(const X& v){
            dat = v;
        }
        /// returns data
        X getDat() const{
            return dat;
        }
        /// returns right node ptr
        Node<X>* getRig() const{
            return rig;
        }
        /// returns left node ptr
        Node<X>* getLef() const {
            return lef;
        }
        /// sets right node ptr
        void setRig(Node<X>* r){
            rig = r;
        }
        /// sets left node ptr
        void setLef(Node<X>* l){
            lef = l;
        }

        /**
        @return: bool
        @param: const Node<X>& n
        Compares and returns true if our data is greater than parameter
        */
        bool operator>(const Node<X>& n){
            return (dat > n.dat);
        }
        /**
        @return: bool
        @param: const Node<X>& n
        Compares and returns true if our data is less than parameter
        */
        bool operator<(const Node<X>& n){
            return (dat < n.dat);
        }

        /**
            @param: std::ostream& out, const Node<X>& n
            @return: std::ostream&
            formats and returns output stream 
        */
        friend std::ostream& operator<<(std::ostream& out, const Node<X>& n){
            out << n.getDat();
            return out;
        }

};


#endif
