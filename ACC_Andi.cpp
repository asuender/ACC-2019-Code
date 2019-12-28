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

void f(std::promise<long long> P, long end, int procnum, unsigned cores) {
    try {
        long long result;
		std::string prstr="\033["+std::to_string(procnum*5)+"C";
		std::cout << prstr+"0%\r";
		long long smallest;
		long long res;
		for (int j=procnum; j<=end; j+=cores) {
		    for (int i=1; i<=j; i++) {
		    	smallest=9223372036854775807;
		        for (int x=i; x<=j; x++){
					mtx.lock();
					while(cache.size()<x+1){
						cache.push_back(0);
					}
					mtx.unlock();
					if (cache.at(x) != 0) res=cache.at(x);
					else {
						res=290797;
						for(int i=x;i>0;i--){
							res*=res;
							res%=50515093;
						}
						mtx.lock();
						cache.at(x)=res;
						mtx.unlock();
					}
					if(res<smallest) smallest=res;
				}
				result+=smallest;
		    }
		    std::cout << prstr+std::to_string(100*(j)/(end))+"%\r";
		}
		std::cout << prstr+"100%\r";
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename T>
long long Thread(T num) {
	std::vector<std::pair<std::future<long long>,std::thread>> threads;
	unsigned cores=std::thread::hardware_concurrency();
	std::cout << "Using " << cores << " Cores\n\r";
	long long result;
	std::cout << "Procs:\n\r";
	for(unsigned i=0;i<cores;i++){
		std::promise<long long> Px;
	    std::future<long long> Fx = Px.get_future();
	    std::thread Tx {f, std::move(Px), num,i+1,cores};
	    threads.push_back(std::make_pair(std::move(Fx),std::move(Tx)));
	}
	
	for(unsigned i=0;i<cores;i++){
		threads[i].second.join();
	}
    
    for(unsigned i=0;i<cores;i++){
    	result+=threads[i].first.get();
    }

    return result;
} 

int main() {
    auto num = 1000;
    time_t tstart = time(NULL);
	
    auto sum = Thread(num); 

    time_t tend = time(NULL);

    std::cout << "\nM(" << num << ")= " << sum << "; " << tend-tstart << " second(s)." << std::endl;
    return 0;
}
