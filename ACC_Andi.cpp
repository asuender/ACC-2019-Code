//!/bin/g++

#include <future>
#include <thread>
#include <exception>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <numeric>


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

template<typename I, typename J, typename CS>
std::tuple<long long,std::map<long, long long>> A(I i, J j,CS cs) {
    long long res;
    long long smallest=9223372036854775807;
    for (J x=i; x<=j; x++){
    	if(cs.count(x)==1){
    		res=cs[x];
    	}else{
	    	res=S(x);
	    	cs.emplace(x,res);
	    }
    	if(res<smallest){
    		smallest=res;
    	}
    }
    return std::make_tuple(smallest,cs);
}

template<typename S, typename E>
std::vector<long long> M(S start, E end) {
    std::vector<long long> results {};
    std::map<long, long long> cs {};	//short for cheatsheet
	long long result;
    for (E j=start+1; j<=end; j++) {
        for (E i=1; i<=j; i++) {
        	tie(result,cs)=A(i, j, cs);
            results.push_back(result); 
        }
    }

    return results;
}

void f(std::promise<std::vector<long long>> P, long long start, long end) {
    try {
        std::vector<long long> result = M(start, end);
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename T>
long long Thread(T num) {
	long num1=1.5*num/2;
	long num2=3*num/4;
	long num3=7*num/8;
    std::promise<std::vector<long long>> P1;
    std::future<std::vector<long long>> F1 = P1.get_future();
    std::thread T1 {f, std::move(P1), 0, num1};

    std::promise<std::vector<long long>> P2;
    std::future<std::vector<long long>> F2 = P2.get_future();
    std::thread T2 {f, std::move(P2), num1, num2};

    std::promise<std::vector<long long>> P3;
    std::future<std::vector<long long>> F3 = P3.get_future();
    std::thread T3 {f, std::move(P3), num2, num3};

    std::promise<std::vector<long long>> P4;
    std::future<std::vector<long long>> F4 = P4.get_future();
    std::thread T4 {f, std::move(P4), num3, num};

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
    auto num = 500;

    time_t tstart = time(NULL);

    auto sum = Thread(num); 

    time_t tend = time(NULL);

    std::cout << "M(" << num << ")= " << sum << "; " << tend-tstart << " second(s)." << std::endl;
    return 0;
}
