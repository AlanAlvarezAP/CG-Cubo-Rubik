#pragma once
#include <array>
#include <cstdint>

inline int Cnk(int n, int k) {
    if (n < k) return 0;
    if (k > n / 2) k = n - k;
    int s = 1, i = n, j = 1;
    while (i != n - k) { s *= i; s /= j; i--; j++; }
    return s;
}

inline int factorial(int n) {
    int f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}

template<typename T>
inline void rotateLeft(T* arr, int l, int r) {
    T tmp = arr[l];
    for (int i = l; i < r; i++) arr[i] = arr[i+1];
    arr[r] = tmp;
}

template<typename T>
inline void rotateRight(T* arr, int l, int r) {
    T tmp = arr[r];
    for (int i = r; i > l; i--) arr[i] = arr[i-1];
    arr[l] = tmp;
}
