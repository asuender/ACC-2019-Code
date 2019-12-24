//!/bin/g++

#include <future>
#include <thread>
#include <mutex>
#include <exception>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <numeric>

std::vector<long long> cache;
std::mutex mtx;

template<typename T>
long long min(std::vector<T> v) {
    long long min_e = (v.size() == 1) ? v.at(0) : *std::min_element(v.begin(), v.end());
    return min_e;
}


template<typename N>
long long S(N n) {
	long long result;
	result=290797;
	for(int i=n;i>0;i--){
		result*=result;
		result%=50515093;
	}
    return result;
}  

template<typename I, typename J>
long long A(I i, J j) {
	std::vector<long long> list;
	long long res;
	long long smallest=9223372036854775807;
	for (J x=i; x<=j; x++){
		mtx.lock();
		while(cache.size()<x+1){
			cache.push_back(0);
		}
		mtx.unlock();
		if (cache.at(x) != 0) {
             res=cache.at(x);
        }else{
			res=S(x);
			mtx.lock();
			cache.at(x)=res;
			mtx.unlock();
		}
		if(res<smallest){
			smallest=res;
		}
	}
	return smallest;
}

template<typename E, typename P>
std::vector<long long> M(E end, P procnum) {
    std::vector<long long> results;
	std::string prstr="\033["+std::to_string(procnum*5)+"C";
	std::cout << prstr+"0%\r";
    for (E j=procnum; j<=end; j+=4) {
        for (E i=1; i<=j; i++) {
            results.push_back(A(i, j));
        }
        std::cout << prstr+std::to_string(100*(j)/(end))+"%\r";
    }
	std::cout << prstr+"100%\r";
    return results;
}

void f(std::promise<std::vector<long long>> P, long end, int procnum) {
    try {
        std::vector<long long> result = M(end, procnum);
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename T>
long long Thread(T num) {
	std::cout << "Procs:\n\r";
    std::promise<std::vector<long long>> P1;
    std::future<std::vector<long long>> F1 = P1.get_future();
    std::thread T1 {f, std::move(P1), num,1};

    std::promise<std::vector<long long>> P2;
    std::future<std::vector<long long>> F2 = P2.get_future();
    std::thread T2 {f, std::move(P2), num,2};

    std::promise<std::vector<long long>> P3;
    std::future<std::vector<long long>> F3 = P3.get_future();
    std::thread T3 {f, std::move(P3), num,3};

    std::promise<std::vector<long long>> P4;
    std::future<std::vector<long long>> F4 = P4.get_future();
    std::thread T4 {f, std::move(P4), num,4};

    T1.join(); T2.join(); T3.join(); T4.join();
    
    std::vector<long long> r = F1.get();
    std::vector<long long> r2 = F2.get();
    std::vector<long long> r3 = F3.get();
    std::vector<long long> r4 = F4.get();

    r.insert(r.end(), r2.begin(), r2.end());
    r3.insert(r3.end(), r4.begin(), r4.end());

    r.insert(r.end(), r3.begin(), r3.end());

    long long sum = 0;
    for (size_t i=0; i<r.size(); i++) sum+=r[i];

    return sum;
} 

int main() {
    auto num = 200;
    time_t tstart = time(NULL);
	
    auto sum = Thread(num); 

    time_t tend = time(NULL);

    std::cout << "\nM(" << num << ")= " << sum << "; " << tend-tstart << " second(s)." << std::endl;
    return 0;
}
