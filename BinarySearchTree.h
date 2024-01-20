//
// Created by drcnm on 12/6/2022.
//

#ifndef FINAL_BINARYSEARCHTREE_H
#define FINAL_BINARYSEARCHTREE_H

#include <queue>
using namespace std;

template <typename K, typename V>
class BinarySearchTree { //linked list version

    struct Node{
        K key;
        V value;
        Node* left;
        Node* right;
    };

    Node* rootNode;
    int siz;
    void Keys(const Node*, queue<K> &) const; //recursive functions
    void clear(Node*);
    void copyTree(Node*, const Node*);

public:
    BinarySearchTree() { rootNode = 0; siz = 0;} //dfault constrcutor
    BinarySearchTree(const BinarySearchTree&); //copy constructor
    BinarySearchTree<K,V>& operator =(const BinarySearchTree<K,V>&);
    ~BinarySearchTree(){ clear();} //destructor

    int size(){return siz;}
    V operator[] (const K&) const; //getter
    V& operator[] (const K&); //setter
    bool containsKey(const K&) const; //getter
    void deleteKey(const K&); //setter
    queue<K> Keys() const;
    void clear();
};

/************************************
 *
 * recursive functions
 *
 * **********************************/

template <typename K, typename V>
void BinarySearchTree<K,V>::Keys(const Node* p, queue<K>& q) const{
    if(!p) return; //for recursive, base case
    Keys(p->left, q);
    q.push(p->key);
    Keys(p->right, q);
}

template <typename K, typename V>
void BinarySearchTree<K,V>::clear(Node* p){
    if(!p) return;
    clear(p->left);
    clear(p->right);
    delete p;
}
template <typename K, typename V>
void BinarySearchTree<K,V>::copyTree(Node* p, const Node* copiedTree){
    //if (!p) return;
    p->key = copiedTree->key;
    p->value = copiedTree->value;
    p->left = p->right = 0;
    if(copiedTree->left) copyTree(p->left = new Node, copiedTree->left); //to end the loop
    if(copiedTree->right) copyTree(p->right = new Node, copiedTree->right); //to end the loop
}


/************************************
 *
 * Copy constructor
 *
 * **********************************/

template <typename K, typename V>
BinarySearchTree<K,V>::BinarySearchTree(const BinarySearchTree& orig){
    rootNode = 0;
    if(orig.rootNode) copyTree(rootNode = new Node, orig.rootNode);
    siz = orig.siz;
}

/************************************
 *
 * Assignment Operator
 *
 * **********************************/
template <typename K, typename V>
BinarySearchTree<K,V>& BinarySearchTree<K,V>::operator =(const BinarySearchTree<K,V>& orig){
    if (this != &orig){ // of it's not a self copy...
        // do what the destructor does
        clear();
        // do what the copy constructor does
        rootNode = 0;
        if(orig.rootNode) copyTree(rootNode = new Node, orig.rootNode);// copy the contents from the original...
        siz = orig.siz;
    }
    return *this; // return a self reference
}

/************************************
 *
 * operator [] setter
 *
 * **********************************/

//operator setter
template <typename K, typename V>
V& BinarySearchTree<K,V>::operator[](const K& key){
    Node* p = rootNode;
    Node* prev = 0;
    while(p){

        if(p-> key == key) return p->value;
        prev=p;
        if(p->key >key) p = p->left;
        else p = p->right;
    }
    //return V();
    //if we didn't find anything, then we need to
    // add a new Node
    Node* temp = new Node{key, V()}; // Node* temp = new Node{key, V(),0,0};
    siz++;

    //add as the root node
    if(prev == 0)
        rootNode = temp;
    else
    if( prev->key < key) prev->right = temp;
    else prev->left = temp;
    return temp->value;
}


/************************************
 *
 * operator [] Getter
 *
 * **********************************/

//getter
template <typename K, typename V>//
V BinarySearchTree<K,V>::operator[](const K& key) const {
    Node* p = rootNode;

    while(p){

        if(p-> key == key) return p->value;

        else if(p->key >key)
            p = p->left;
        else
            p = p->right;
    }
    return V();

}
/************************************
 *
 * containsKey
 *
 * **********************************/
//contains key
template <typename K, typename V>//
bool BinarySearchTree<K,V>::containsKey(const K& key) const {
    Node* p = rootNode;
    while(p){

        if(p->key == key) return true;
        else if(p->key > key)
            p = p->left; //smaller
        else
            p = p->right;
    }
    return false; //didnt find anything
}
/************************************
 *
 * deleteKey
 *
 * **********************************/

// deleteKey setter
template<class K, class V>
void BinarySearchTree<K, V>::deleteKey(const K& key){
    Node* p = rootNode; Node* prev = 0;

    while(p){
        if (p->key == key){
            break; // found
        }
        prev = p;
        if (p->key > key){
            p = p->left;
        }
        else{
            p = p->right;
        }
    }

    if (!p){ // key not found
        return;
    }

    if (p->left == 0){
        if (p->right == 0){

            if (prev == 0){ // only one node in tree
                rootNode = 0;
            }

            if (prev->left == p){ // p has are no children
                prev->left = 0;
            }
            else{
                prev->right = 0;
            }

            delete p;
            siz--;
            return;
        }

        Node* pSave = p; // promote to this node
        prev = p;
        p = p->right; // the right step (move down the right hand side of tree)

        while(p->left){ // find the smallest value on the right hand side
            prev = p;
            p = p->left;
        }

        // Promote new key and value to node
        pSave->key = p->key;
        pSave->value = p->value;

        if (prev->left == p){
            prev->left = p->right;
        }
        else{
            prev->right = p->right;
        }

        delete p;
        siz--;
        return;
    }
}

/************************************
 *
 * public Keys function
 *
 * **********************************/
template <typename K, typename V>
queue<K> BinarySearchTree<K,V>::Keys() const{
    queue<K> result;
    Keys(rootNode, result); //calls the private function
    return result;
}
/************************************
 *
 * public clear function
 *
 * **********************************/
template <typename K, typename V>
void BinarySearchTree<K,V>::clear(){
    clear(rootNode);
    rootNode = 0;
    siz = 0;
}


#endif //FINAL_BINARYSEARCHTREE_H
