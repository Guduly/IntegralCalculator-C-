#ifndef BINTREE_H
#define BINTREE_H
using namespace std;
#include<iostream>
#include "Term.h"
#include "Node.h"
#include<vector>

template <typename B>
class BinTree{
    private: 
        Node<B>* root;
        void recurInsert(Node<B>*& n, const B& val);
        Node<B>* recurSearch(Node<B>*& n, const B& val);
        void recurInorder(Node<B>* n, vector<B>& vec);
    
    public:
        BinTree(){
            root = nullptr;
        }
        /// inserts a node ---> recursive call to recurInsert
        /// since we need to keep track of nodes to recurse
        void insert(const B& val);
        /// searches for the data ---> recursive call to recurSearch
        /// since we need to keep track of nodes to recurse
        Node<B>* search(const B& val);
        // InOrder maintains order, which makes it easier for outputing
        vector<B> inorder();
        bool isEmpty(){
            return !root;
        }
};



template <typename B>
void BinTree<B>::recurInorder(Node<B>* n, vector<B>& vec){
    if(n == nullptr) return;
    recurInorder(n->getLef(), vec);
    vec.push_back(n->getDat());
    recurInorder(n->getRig(), vec);
}

template <typename B>
vector<B> BinTree<B>::inorder(){
    vector<B> res;
    recurInorder(root, res);
    return res;
}

template <typename B>
Node<B>* BinTree<B>::recurSearch(Node<B>*& n, const B& val){
    /// edge case check
    if(!n) return nullptr;
     /// move to left side, if curr side too big
    if(n->getDat() > val) return recurSearch(n->getLef(), val);
    /// move to right side, if curr side too small
    else if(n->getDat() < val) return recurSearch(n->getRig(), val);
    else return n;
}

template <typename B>
void BinTree<B>::recurInsert(Node<B>*& n, const B& val){
    /// if null, add the val there
    if(n == nullptr){
        n = new Node<B>(val);
        return;
    }
    bool check = (n->getDat().isTrig() || val.isTrig());
    /// combine data if same degree
    if(n->getDat().getExp() == val.getExp() && !check){
        B data = n->getDat();
        data.setCoe(data.getCoe() + val.getCoe());
        /// merging all info into one Term
        n->setDat(data);
        return;
    }
    
    /// move to left side, if curr side too big
    if(n->getDat() > val){
        Node<B>* leftC = n->getLef();
        /// stopping point
        if((leftC == nullptr)){
            n->setLef(new Node<B>(val));
        }else{
            recurInsert(leftC, val);
        }
    /// move to right side, if curr side too small    
    }else if(n->getDat() < val){
        Node<B>* rightC = n->getRig();
        /// stopping point
        if((rightC == nullptr)){
            n->setRig(new Node<B>(val));
        }else{
            recurInsert(rightC, val);
        }
    }
}
template <typename B>
/// sends reusive call to recurInsert
void BinTree<B>::insert(const B& val){
    if(root == nullptr)
        root = new Node<B>(val);
    else{
        // recursion insert with two parameters
        recurInsert(root, val);
    }
}

template <typename B>
/// sends func call to recurSearch
Node<B>* BinTree<B>::search(const B& val){
    return recurSearch(root, val);
}

#endif
