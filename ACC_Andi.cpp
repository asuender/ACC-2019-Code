//C++

#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <future>
#include <numeric>


template<typename T>
long long min(std::vector<T> v) {
    long long min_e = (v.size() == 1) ? v.at(0) : *std::min_element(v.begin(), v.end());
    return min_e;
}


template<typename N>
long long S(N n) {
    if (n==0)
        return 290797;
    long long s = S(n-1);
    return (s*s) % 50515093;
}  

template<typename I, typename J>
long long A(I i, J j) {
    std::vector<long long> list {};
    for (J x=i; x<=j; x++) list.push_back(S(x));
    return min(list);
}

template<typename S, typename E>
std::vector<long long> M(S start, E end) {
    std::vector<long long> results {};

    for (E j=start+1; j<=end; j++) {
        for (E i=1; i<=j; i++) {
            results.push_back(A(i, j)); 
        }
    }

    return results;
}

template<typename T>
long long Thread(T num) {
    auto a1 = std::async(M<T, T>, 0, num/2);
    auto a2 = std::async(M<T, T>, num/2, num);

    std::vector<long long> r = a1.get();
    std::vector<long long> r2 = a2.get();
    r.insert(r.end(), r2.begin(), r2.end());


    long long sum = 0;
    for (auto i=0; i<r.size(); i++) sum+=r[i];
    return sum;
}

int main() {
    auto num = 200;

    time_t tstart = time(NULL);
    auto sum = Thread(num);
    time_t tend = time(NULL);

    std::cout << "M(" << num << ")= " << sum << "; " << tend-tstart << " second(s)." << std::endl;
    return 0;
}