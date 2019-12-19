//C++

#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>

template<typename T>
T min(std::vector<T> v) {
    T min = (v.size() == 1) ? v.at(0) : *std::min_element(v.begin(), v.end());
    return min;
}

int S(int n=0) {
    return 290797;
}

template<typename N>
N S(N n) {
    return (S(n-1)*S(n-1)) % 50515093;
}

template<typename I, typename J>
unsigned long A(I i, J j) {
    std::vector<unsigned long> list = {};
    for (auto x=i; x<=j; x++) list.push_back(S(x));
    return min(list);
}

template<typename N>
N M(N n) {
    std::vector<N> results = {};
    for (auto j=1; j<=n; j++) {
        for (auto i=1; i<=j; i++) {
            results.push_back(A(i, j));
        }
    }
    unsigned long sum = 0;
    for (auto it = results.begin(); it != results.end(); ++it) {
        sum += *it;
    }
    return sum;
}

int main() {
    time_t tstart = time(NULL);
    std::cout << M(500) << std::endl;
    time_t tend = time(NULL);
    std::cout << tend-tstart << " second(s)." << std::endl;
    return 0;
}