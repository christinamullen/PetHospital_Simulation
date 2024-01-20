//
// Created by drcnm on 12/6/2022.
//

#ifndef FINAL_DYNAMICARRAY_H
#define FINAL_DYNAMICARRAY_H

template <typename T>
class DynamicArray{
    T* value;
    int cap;
    T dummy;

public:
    DynamicArray(int =  2); //default constructor
    DynamicArray(const DynamicArray<T>&); //copy constructor
    ~DynamicArray() {delete [] value;} //destructor
    DynamicArray<T>& operator = (const DynamicArray<T>&); //assignment operator
    int capacity() const {return cap;} //const capacity getter
    void capacity(int); //capacity setter
    T operator[] (int) const; //getter
    T& operator[] (int); //setter
};

template <typename T>  //default constructor
DynamicArray<T>::DynamicArray(int cap){
    this->cap = cap;
    this->value = new T[cap]; //dynamic array in heap memory
    for(int i = 0; i < cap; i++)
        value[i] = T();
    dummy = T();
}

template <typename T> //copy constructor
DynamicArray<T>::DynamicArray(const DynamicArray<T>& og){
    this->cap = og.cap;
    this->value = new T[cap];

    for (int i = 0; i < cap; i++)
        value[i] = og.value[i];
    this->dummy = og.dummy;
}

template <typename T>//assignment operator
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& og) {
    if(this != &og) {
        delete [] value;

        this->cap = og.cap;
        this->value = new T[cap];

        for(int i = 0; i < cap; i++)
            value[i] = og.value[i];
    }
    return *this;
}

template <typename T> //capacity setter
void DynamicArray<T>::capacity(int cap){
    T* temp = new T[cap];
    int limit = (cap < this->cap ? cap : this->cap);

    for (int i = 0; i < limit; i++)
        temp[i] = value[i];

    for (int i = limit; i < cap; i++)
        temp[i] = T();

    delete [] value;
    value = temp;
    this->cap = cap;
}

template <typename T> //operator [] getter
T DynamicArray<T>::operator[] (int index) const{
    if (index < 0 || index >= cap) return T();
    return value[index];
}

template <typename T> //operator [] setter
T& DynamicArray<T>::operator[] (int index) {
    if (index < 0) { return dummy; }
    if (index >= cap) capacity(2 * index);
    return value[index];
}
#endif //FINAL_DYNAMICARRAY_H
