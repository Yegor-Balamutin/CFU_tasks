#include <iostream>

struct IntArray{
    int* data;
    int size;
};

void create(IntArray* arr, int size){
    if (size < 0){
        std::cout << "incorrect size\n";
        exit(1);
    }
    (*arr).size = size;
    (*arr).data = new int[size];
}

void create(IntArray& arr, int size){
    if (size < 0){
        std::cout << "incorrect size\n";
        exit(1);
    }
    arr.size = size;
    arr.data = new int[size];
}

int get(IntArray* arr, int index){
    if ((index < 0) || (index >= (*arr).size)){
        std::cout << "incorrect index\n";
        exit(1);
    }
    else return (*arr).data[index];
}

int get(IntArray& arr, int index){
    if ((index < 0) || (index >= arr.size)){
        std::cout << "incorrect index\n";
        exit(1);
    }
    else return arr.data[index];
}

void set(IntArray* arr, int index, int value){
    if ((index < 0) || (index >= (*arr).size)){
        std::cout << "incorrect index\n";
        exit(1);
    }
    else (*arr).data[index] = value;
}

void set(IntArray& arr, int index, int value){
    if ((index < 0) || (index >= arr.size)){
        std::cout << "incorrect index\n";
        exit(1);
    }
    else arr.data[index] = value;
}

void print(IntArray* arr){
    std::cout << '[';
    for (int i = 0; i+1 < (*arr).size; i++) std::cout << (*arr).data[i] << ", ";
    if ((*arr).size > 0) std::cout << (*arr).data[(*arr).size - 1];
    std::cout << ']' << std::endl;;
}

void print(IntArray& arr){
    std::cout << '[';
    for (int i = 0; i+1 < arr.size; i++) std::cout << arr.data[i] << ", ";
    if (arr.size > 0) std::cout << arr.data[arr.size - 1];
    std::cout << ']' << std::endl;;
}

void resize(IntArray* arr, int newSize){
    if (newSize < 0){
        std::cout << "incorrect new size\n";
        exit(1);
    }
    int* buf = new int[(*arr).size];
    for (int i = 0; i < (*arr).size; i++) buf[i] = (*arr).data[i];
    delete [] (*arr).data;
    (*arr).data = new int[newSize];
    for (int i = 0; i < newSize; i++){
        if (i < (*arr).size) (*arr).data[i] = buf[i];
        else (*arr).data[i] = 0;
    }
    (*arr).size = newSize;
    delete [] buf;
}

void resize(IntArray& arr, int newSize){
    if (newSize < 0){
        std::cout << "incorrect new size\n";
        exit(1);
    }
    int* buf = new int[arr.size];
    for (int i = 0; i < arr.size; i++) buf[i] = arr.data[i];
    delete [] arr.data;
    arr.data = new int[newSize];
    for (int i = 0; i < newSize; i++){
        if (i < arr.size) arr.data[i] = buf[i];
        else arr.data[i] = 0;
    }
    arr.size = newSize;
    delete [] buf;
}

void destroy(IntArray* arr){
    if ((*arr).data == nullptr) std::cout << "array has been deleted already\n";
    else{
        delete [] (*arr).data;
        (*arr).data = nullptr;
        (*arr).size = 0;
    }
}

void destroy(IntArray& arr){
    if (arr.data == nullptr) std::cout << "array has been deleted already\n";
    else{
        delete [] arr.data;
        arr.data = nullptr;
        arr.size = 0;
    }
}

int main()
{
    IntArray arr;
    create(arr, 30);
    for (int i = 1; i <= 30; i++){
        set(arr, i-1, i);
    }
    print(arr);
    resize(arr, 50);
    print(arr);
    resize(arr, 10);
    print(arr);
    destroy(arr);
}
