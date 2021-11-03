#pragma once
using namespace std;
#ifndef RB_TREE
#define RB_TREE
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <assert.h>

namespace rbtree {
    enum RBNodecolor { BLACK, RED };
    class RBNode
    {
    public:
        RBNode* parent;
        RBNode* izq;
        RBNode* der;  

        enum RBNodecolor color; 

        RBNode() {};

        RBNode(RBNode* n)
            : parent(n), izq(n), der(n) {
            color = RED;
        }

        virtual ~RBNode() {} 
        RBNode* getParent() {
            return parent;
        }

        RBNode* getizq() {
            return izq;
        }

        RBNode* getder() {
            return der;
        }

        void setParent(RBNode* n) {
            parent = n;
        }

        void setizq(RBNode* n) {
            izq = n;
        }

        void setder(RBNode* n) {
            der = n;
        }
    };

    class RBTree {
    public:

        RBNode* root;
        RBNode* nil;
        virtual ~RBTree();
        bool checkTree();
        long countBlack(RBNode* n) {
            if (n == nil)
                return 0;
            return ((n->color == BLACK) ? 1 : 0)
                + countBlack(n->izq) + countBlack(n->der);
        }

        void InsertFixup(RBNode* z, void (*updateNode)(RBNode* n, RBTree* T));
        void DeleteFixup(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T));
        void Delete(RBNode* z, void (*updateNode)(RBNode* n, RBTree* T));
        RBNode* findderSiblingLeaf(RBNode* n);
        RBNode* findizqSiblingLeaf(RBNode* n);
        RBNode* treeSuccessor(RBNode* x);
        RBNode* Minimum(RBNode* x);
        bool isleftChild(RBNode* n);
        bool isrightChild(RBNode* n);

        void printSubTree(RBNode* n);
        int checkSubTree(RBNode* n);
        void checkNode(RBNode* x);

        void deleteNode(RBNode* x) {
            if (x->izq != nil) deleteNode(x->izq);
            if (x->der != nil) deleteNode(x->der);
            delete x;
        }


    private:
        void leftRotate(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T));
        void rightRotate(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T));
    };

} 

namespace rbtree {

    RBTree::~RBTree() {
        if (root != this->nil)
            deleteNode(root);
        delete this->nil;
    }

    bool RBTree::checkTree() {
        return checkSubTree(root) != -1;
    }

    void RBTree::leftRotate(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T)) {
        assert(x->der != nil);
        RBNode* y = x->der;
        x->der = y->izq;
        if (y->izq != nil) y->izq->parent = x;
        y->parent = x->parent;
        if (x->parent == this->nil)
            root = y;
        else {
            if (x == x->parent->izq)
                x->parent->izq = y;
            else
                x->parent->der = y;
        }
        y->izq = x;
        x->parent = y;

        if (updateNode != 0) {
            updateNode(x, this);
            updateNode(y, this);
        }
    }

    void RBTree::rightRotate(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T)) {
        assert(x->izq != nil);
        RBNode* y = x->izq;
        x->izq = y->der;
        if (y->der != nil) y->der->parent = x;
        y->parent = x->parent;

        if (x->parent == nil)
            root = y;
        else {
            if (x == x->parent->der) {
                x->parent->der = y;
            }
            else {
                x->parent->izq = y;
            }
        }
        y->der = x;
        x->parent = y;
        if (updateNode != 0) {
            updateNode(x, this);
            updateNode(y, this);
        }
    }

    void RBTree::InsertFixup(RBNode* z, void (*updateNode)(RBNode* n, RBTree* T)) {
        RBNode* y;
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->izq) {
                y = z->parent->parent->der;

                if (y->color == RED) {
                    z->parent->color = BLACK;        
                    y->color = BLACK;               
                    z->parent->parent->color = RED;  
                    z = z->parent->parent;           
                }
                else {
                    if (z == z->parent->der) {
                        z = z->parent;    
                        leftRotate(z, updateNode);  
                    }
                    z->parent->color = BLACK;             
                    z->parent->parent->color = RED;      
                    rightRotate(z->parent->parent, updateNode);  
                }
            }
            else {
                y = z->parent->parent->izq;
                if (y->color == RED) {
                    z->parent->color = BLACK;       
                    y->color = BLACK;                
                    z->parent->parent->color = RED;  
                    z = z->parent->parent;           
                }
                else {
                    if (z == z->parent->izq) {
                        z = z->parent;     
                        rightRotate(z, updateNode);  
                    }
                    z->parent->color = BLACK;             
                    z->parent->parent->color = RED;      
                    leftRotate(z->parent->parent, updateNode);  
                }
            }
        } 

        root->color = BLACK;
        assert(checkTree());
    }

    void RBTree::DeleteFixup(RBNode* x, void (*updateNode)(RBNode* n, RBTree* T)) {
        RBNode* w;

        while ((x != root) && (x->color == BLACK)) {
            if (x == x->parent->izq) {
                w = x->parent->der;
                if (w->color == RED) {
                    w->color = BLACK;          
                    x->parent->color = RED;     
                    leftRotate(x->parent, updateNode); 
                    w = x->parent->der;      
                }
                if ((w->izq->color == BLACK) && (w->der->color == BLACK)) {
                    w->color = RED;  
                    x = x->parent;  
                }
                else {
                    if (w->der->color == BLACK) {
                        w->izq->color = BLACK; 
                        w->color = RED;          
                        rightRotate(w, updateNode);      
                        w = x->parent->der;    
                    }
                    w->color = x->parent->color; 
                    x->parent->color = BLACK;     
                    w->der->color = BLACK;      
                    leftRotate(x->parent, updateNode);    
                    x = root;                 
                }
            }
            else {
                w = x->parent->izq;
                if (w->color == RED) {
                    w->color = BLACK;           
                    x->parent->color = RED;     
                    rightRotate(x->parent, updateNode);  
                    w = x->parent->izq;       
                }
                if ((w->der->color == BLACK) && (w->izq->color == BLACK)) {
                    w->color = RED;  
                    x = x->parent;  
                }
                else {
                    if (w->izq->color == BLACK) {
                        w->der->color = BLACK;  
                        w->color = RED;         
                        leftRotate(w, updateNode);    
                        w = x->parent->izq;   
                    }

                    w->color = x->parent->color; 
                    x->parent->color = BLACK;   
                    w->izq->color = BLACK;  
                    rightRotate(x->parent, updateNode);    

                    x = root;                 
                }
            }
        } 
        x->color = BLACK;
    }

    void RBTree::Delete(RBNode* z, void (*updateNode)(RBNode* n, RBTree* T)) {
        RBNode* y, * x, * y_oldParent;
        y_oldParent = nil;
        if (z->izq == nil || z->der == nil) {
            y = z; 
        }
        else {
            y = treeSuccessor(z);
            y_oldParent = y->parent;
        }

        if (y->izq != nil) {
            x = y->izq;
        }
        else {
            x = y->der;
        }

        x->parent = y->parent;

        if (y->parent == nil) {
            root = x;
        }
        else {
            if (isleftChild(y)) { y->parent->izq = x; }
            else { y->parent->der = x; }
        }

        RBNodecolor old_y = y->color;
        if (y != z) { 
            if (root == z) {
                root = y;
            }
            else {
                if (isleftChild(z)) z->parent->izq = y;
                else z->parent->der = y;
            }

            y->parent = z->parent;
            y->izq = z->izq;
            y->der = z->der;
            y->color = z->color;  

            y->izq->parent = y;
            y->der->parent = y;

        }


        if (old_y == BLACK) {
            DeleteFixup(x, updateNode);
        }

        updateNode(y, this);
        if (y_oldParent != nil) updateNode(y_oldParent, this);

        assert(checkTree());
    }


    RBNode* RBTree::treeSuccessor(RBNode* x) {
        if (x->der != nil) return Minimum(x->der);

        RBNode* y = x->parent;
        while ((y != nil) && (x == y->der)) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    RBNode* RBTree::Minimum(RBNode* x) {
        while (x->izq != nil) x = x->izq;
        return x;
    }

    bool RBTree::isleftChild(RBNode* n) {
#ifndef NDEBUG
        if (n->parent == nil) {
            cerr << "error: es hijo izquierdo, sin padre." << endl;
            exit(EXIT_FAILURE);
        }
#endif
        return (n->parent->izq == n);
    }

    bool RBTree::isrightChild(RBNode* n) {
#ifndef NDEBUG
        if (n->parent == nil) {
            cerr << "error: es hijo derecho, sin padre" << endl;
            exit(EXIT_FAILURE);
        }
#endif
        return (n->parent->der == n);
    }


    RBNode* RBTree::findderSiblingLeaf(RBNode* n) {
        // go up:
        while (true) {
            if (n->parent != nil) {
                if (n == n->parent->der)
                    n = n->parent;
                else
                    break;
            }
            else
                return nil;
        }

        n = n->parent;
        n = n->der;

        while (n->izq != nil) {
            n = n->izq;
        }

        return n;
    }

    RBNode* RBTree::findizqSiblingLeaf(RBNode* n) {

        while (true) {
            if (n->parent != nil) {
                if (n == n->parent->izq)
                    n = n->parent;
                else
                    break;
            }
            else
                return nil;
        }

        n = n->parent;
        n = n->izq;
        while (n->der != nil) {
            n = n->der;
        }

        return n;
    }

    void RBTree::printSubTree(RBNode* n) {
        cout << ((n->color == BLACK) ? "BLACK" : "RED") << " ("
            << n << ") [";
        if (n->izq == nil) cout << "N";
        else printSubTree(n->izq);
        cout << ",";
        if (n->der == nil) cout << "N";
        else printSubTree(n->der);
        cout << "]";
    }


    int RBTree::checkSubTree(RBNode* n) {

        if (n == nil)
            return 0;
        if (n->izq == nil
            && n->der == nil)
            return (n->color == BLACK) ? 1 : 0;

        int nb_izq, nb_der;

        nb_izq = checkSubTree(n->izq);

        nb_der = checkSubTree(n->der);

        if (nb_izq == -1 || nb_der == -1
            || nb_izq != nb_der)
            return -1;

        return nb_izq + ((n->color == BLACK) ? 1 : 0);
    }


    void RBTree::checkNode(RBNode* n) {
        if (n->izq != nil) {
            if (n->izq->parent != n) {
                cout << "au" << endl;
                exit(1);
            }

        }

        if (n->der != nil) {
            if (n->der->parent != n) {
                cout << "au" << endl;
                exit(1);
            }
        }
    }

} 

#endif