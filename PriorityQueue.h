//
// Created by drcnm on 12/6/2022.
//

#ifndef FINAL_PRIORITYQUEUE_H
#define FINAL_PRIORITYQUEUE_H

#include <algorithm>
using namespace std;

template<typename V>
class PriorityQueue{
    V* values;
    int cap;
    int siz;
    void capacity(int);

public:
    PriorityQueue(int = 2);
    PriorityQueue(const PriorityQueue<V>&);
    ~PriorityQueue() {delete [ ] values; }
    PriorityQueue<V>& operator=(const PriorityQueue<V>&);


    void push(const V&);
    void pop();
    V top() const {return siz == 0 ? V() : values[0];}
    int size() const {return siz;}
    bool empty() const {return siz == 0 ? true : false;}
    void clear() { siz =0;}

};


template<typename V>
PriorityQueue<V>::PriorityQueue(int cap){
    this-> cap = cap;
    siz =0;
    values = new V[cap];
    /* Dont need this for stack and queue bc we are not accessing by index
    for (int i = 0; i < cap; ++i) {
        values[i] = V();  //no mystery numbers, dynamic memory needs a default value at an index to use it
    }*/
}


template <typename V>
PriorityQueue<V>::PriorityQueue(const PriorityQueue<V>& orig) {
    this->cap = orig.cap;
    this->siz = orig.siz;
    values = new V[cap];
    for (int i = 0; i < cap; ++i) {
        values[i] = orig.values[i];
    }

}



template <typename V>
PriorityQueue<V>& PriorityQueue<V>::operator=(const PriorityQueue<V>& orig){
    if (this != orig) {
        delete [ ] values;

        cap = orig.cap;
        siz = orig.siz;
        values = new V[cap];
        for (int i = 0; i < cap; ++i) {
            values[i] = orig.values[i];
        }
    }
    return *this;
}
#if 0
template <typename V>
void PriorityQueue<V>::capacity(int c) {
    V* temp =new V[c];
    int i = 0;
    for (i = 0; i < this->cap; ++i) {
        temp[i] =values[i];
    }
    delete [ ] values;
    //this->clear();
    values=temp;
    this->cap= cap;
}
#endif
template <typename V>
void PriorityQueue<V>::capacity(int cap){
    V* temp = new V[cap];
    int i = 0;
    for (i = 0; i < this->cap; ++i) {
        temp[i] = values[i];
    }
    delete [ ] values;
    values = temp;
    this->cap = cap;
}

//push
template <typename V>
void PriorityQueue<V>::push(const V& value){
    if (siz >= cap){
        capacity(cap*2);
    }
    values[siz] = value;
    // move the newly added value to the right location
// until the value of parent_index >= ,or reach the top
    int index = siz;
    int p_index=  (index +1)/2-1;
    while(index >0){
        if(values[p_index] < values[index]){ //use "<" only in the header
            swap(values[index], values[p_index]);
        }
        else{
            break;
        }
        index = p_index;
        p_index = (index+1)/2-1;
    }
    siz++;
}

template<typename V>
void PriorityQueue<V>::pop() {
    if (siz == 0) return;
    siz--;
    values[0] = values[siz];
    int index = 0;
    int lIndex = 2 * index + 1;
    int rIndex = 2 * index + 2;

    while (lIndex < siz) {
        if (rIndex >= siz)  // only L
        {
            if (values[index] < values[lIndex]) { //lIndex wins
                swap(values[lIndex], values[index]);
                index = lIndex;
            } else
                break;
        }

        // both L and R
        if (values[lIndex] < values[rIndex] && values[index] < values[rIndex]) {  //rIndex wins
            swap(values[rIndex], values[index]);
            index = rIndex;
        } else if (values[index] < values[lIndex]) {  //lIndex wins
            swap(values[lIndex], values[index]);
            index = lIndex;
        } else
            break;
//recalculate
        lIndex = 2 * index + 1;
        rIndex = 2 * index + 2;
    }
}
#endif //FINAL_PRIORITYQUEUE_H
