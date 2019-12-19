//C++

#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <thread>
#include <future>

//some global variables
unsigned long j;
unsigned long i;

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

template<typename N, typename A>
std::vector<unsigned long> M(N n, A area) {
    n = n-area;
    if (n == 1000) {
        ::j=501;
        ::i=501;
    }
    else {
        ::j=1;
        ::i=1;
    }

    std::vector<N> results = {};
    for (::j; ::j<=n; ::j++) {
        for (::i; ::i<=::j; ::i++) {
            results.push_back(A(i, j));
        }
    }
    return results;
    /*unsigned long sum = 0;
    for (auto it = results.begin(); it != results.end(); ++it) {
        sum += *it;
    }
    return sum;*/
}

template<typename T>
void Thread(T num) {
    auto th1 = std::async(M, num, 1000);
    //unsigned long th1v = th1.get();
    auto th2 = std::async(M, num, 500);
    //nsigned long th2v = th2.get();
}

int main() {
    time_t tstart = time(NULL);
    Thread(1000);
    time_t tend = time(NULL);
    std::cout << tend-tstart << " second(s)." << std::endl;
    return 0;
}