#include <iostream>

int* create(int len, int a0 = 0, int step = 0){
    int* n_arr;
    if (len <= 0) n_arr = new int[0];
    else {
        n_arr = new int[len];
        n_arr[0] = a0;
        for (int i = 1; i < len; i++) n_arr[i] = n_arr[i-1] + step;
    }
    return n_arr;
}

int* sort(int len, int** arr){
    for (int i = 1; i < len; i++){
        for (int j = i; (j > 0) && ((*arr)[j-1] > (*arr)[j]); j--){
            std::swap((*arr)[j-1], (*arr)[j]);
        }
    }
    return *arr;
}

int* print(int len, int* arr){
    std::cout << "[";
    for (int i = 0; i+1 < len; i++){
        std::cout << arr[i] << ", ";
    }
    if (len > 0) std::cout << arr[len-1];
    std::cout << "]" << std::endl;
    return arr;
}

int main()
{
    int len, a0, step;
    std::cin >> len >> a0 >> step;
    int* arr = create(len, a0, step);
    sort(len, &arr);
    print(len, arr);
    delete [] arr;
    return 0;
}
