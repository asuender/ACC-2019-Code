//!/bin/g++

#include <future>
#include <thread>
#include <mutex>
#include <exception>
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <numeric>
#include <utility>

std::vector<long long> cache;
std::mutex mtx;



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
		if (cache.at(x) != 0) res=cache.at(x);
        else {
			res=S(x);
			mtx.lock();
			cache.at(x)=res;
			mtx.unlock();
		}
		if(res<smallest) smallest=res;
	}
	return smallest;
}

template<typename E, typename P, typename C>
std::vector<long long> M(E end, P procnum, C cores) {
    std::vector<long long> results;
	std::string prstr="\033["+std::to_string(procnum*5)+"C";
	std::cout << prstr+"0%\r";
    for (E j=procnum; j<=end; j+=cores) {
        for (E i=1; i<=j; i++) {
            results.push_back(A(i, j));
        }
        std::cout << prstr+std::to_string(100*(j)/(end))+"%\r";
    }
	std::cout << prstr+"100%\r";
    return results;
}

void f(std::promise<std::vector<long long>> P, long end, int procnum, unsigned cores) {
    try {
        std::vector<long long> result = M(end, procnum,cores);
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename T>
long long Thread(T num) {
	std::vector<std::pair<std::future<std::vector<long long>>,std::thread>> threads;
	unsigned cores=std::thread::hardware_concurrency();
	std::cout << "Using " << cores << " Cores\n\r";
	std::vector<long long> results;
	std::cout << "Procs:\n\r";
	for(unsigned i=0;i<cores;i++){
		std::promise<std::vector<long long>> Px;
	    std::future<std::vector<long long>> Fx = Px.get_future();
	    std::thread Tx {f, std::move(Px), num,i+1,cores};
	    threads.push_back(std::make_pair(std::move(Fx),std::move(Tx)));
	}
	
	for(unsigned i=0;i<cores;i++){
		threads[i].second.join();
	}
    
    for(unsigned i=0;i<cores;i++){
    	std::vector<long long> ress=threads[i].first.get();
    	results.insert(results.end(),ress.begin(),ress.end());
    }
    
    long long sum = 0;
    for (size_t i=0; i<results.size(); i++) sum+=results[i];

    return sum;
} 

int main() {
    auto num = 1000;
    time_t tstart = time(NULL);
	
    auto sum = Thread(num); 

    time_t tend = time(NULL);

    std::cout << "\nM(" << num << ")= " << sum << "; " << tend-tstart << " second(s)." << std::endl;
    return 0;
}
