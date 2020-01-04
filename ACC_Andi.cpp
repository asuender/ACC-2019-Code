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

using std::vector; using std::tuple; using std::cout; using std::endl; using std::string;
using std::initializer_list;

template<typename Type>
struct Calculation {
    using calc_t = long long;

    vector<Type> values;
    static unsigned cores;

    Calculation() {};
    Calculation(initializer_list<Type> init) {
        values.insert(values.begin(), init.begin(), init.end());
    }

    void calc(std::promise<long long> P, long end, int procnum, unsigned cores);
    vector<calc_t> calculate();
};

template<typename Type>
void Calculation<Type>::calc(std::promise<long long> P, long end, int procnum, unsigned cores) {
    try {
        long long result;
        std::vector<long long> results;
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
				results.push_back(smallest);
		    }
		    std::cout << prstr+std::to_string(100*(j)/(end))+"%\r";
		}
		std::cout << prstr+"100%\r";
		for (size_t i=0; i<results.size(); i++) result+=results[i];
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename Type>
vector<long long> Calculation<Type>::calculate() {
    vector<long long> tmp;
    std::cout << "Using " << this->cores << " Cores\n\r";
    for (auto num : this->values) {
        std::vector<std::pair<std::future<long long>,std::thread>> threads;
        long long result;
        std::cout << "Procs:\n\r";
        for(unsigned i=0;i<cores;i++) {
            std::promise<long long> Px;
            std::future<long long> Fx = Px.get_future();
            std::thread Tx {&Calculation::calc, this, std::move(Px), num,i+1,cores};
            threads.push_back(std::make_pair(std::move(Fx),std::move(Tx)));
        }

        for(unsigned i=0;i<cores;i++) {
            threads[i].second.join();
        }

        for(unsigned i=0;i<cores;i++) {
            result+=threads[i].first.get();
        }

        tmp.push_back(result);
    }
    return tmp;
}

template<typename Type>
unsigned Calculation<Type>::cores = std::thread::hardware_concurrency();

int main() {
    time_t tstart = time(NULL);
    Calculation<int> c {500};
    vector<long long> v = c.calculate();
    time_t tend = time(NULL);

    std::cout << "\n" << v.at(0) << std::endl;
    std::cout << tend-tstart << " second(s)." << std::endl;
    return 0;
}
