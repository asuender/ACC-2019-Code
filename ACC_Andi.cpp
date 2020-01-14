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

std::vector<unsigned long long> cache;
std::vector<std::string> statusprint(5);
std::mutex cachemtx;	//mutex for the cache vector
std::mutex prntmtx;		//mutex for printing stuff
std::mutex vmutex; 		//mutex for printing vector

using std::vector; using std::tuple; using std::cout; using std::endl; using std::string;
using std::initializer_list; using std::future; using std::promise; using std::thread;

template<typename Type>
struct Calculation {
    using calc_t = unsigned long long;

    vector<Type> values;
    static unsigned cores;
	int valuec;     

    Calculation() {};
    Calculation(initializer_list<Type> init) {
        values.insert(values.begin(), init.begin(), init.end());
		valuec = init.size();
    }

    void calc(std::promise<unsigned long long> P, long end, unsigned int procnum, unsigned cores);
    vector<calc_t> calculate();
};


void print(int cores) {
	prntmtx.lock();
	vmutex.lock();
	for (size_t t=1; t<statusprint.size(); t++) {
		cout << "\r\033["+std::to_string(t*5)+"C" << statusprint[t-1] << "%\r" << std::flush;
	}
	cout << "\033["+std::to_string(cores*5)+"C\t\t" << statusprint[4] << " sec.\r" << std::flush;
	prntmtx.unlock();
	vmutex.unlock();
}

template<typename Type>
void Calculation<Type>::calc(std::promise<unsigned long long> P, long end, unsigned int procnum, unsigned cores) {
    try {
		time_t dstart = time(NULL);

        std::vector<unsigned long long> results;
		std::string prstr="\033["+std::to_string(procnum*5)+"C";
		prntmtx.lock();
		std::cout << prstr << "0%\r";
		prntmtx.unlock();
		unsigned long long smallest;
		unsigned long long res;
		uint8_t percentage=0;
		for (unsigned int j=procnum; j<=end; j+=cores) {
		    for (unsigned int i=1; i<=j; i++) {
		    	smallest=-1;
		        for (unsigned int x=i; x<=j; x++){
					cachemtx.lock();
					while(cache.size()<x+1){
						cache.push_back(0);
					}
					cachemtx.unlock();
					if (cache.at(x) != 0) res=cache.at(x);
					else {
						res=290797;
						for(unsigned int i=x;i>0;i--){
							res*=res;
							res%=50515093;
						}
						cachemtx.lock();
						cache.at(x)=res;
						cachemtx.unlock();
					}
					if(res<smallest) smallest=res;

					
				}
				results.push_back(smallest);
		    }
			/*if (percentage < 33) cout << "\033[31m";
			if (percentage < 63 && percentage > 33) cout << "\033[91m";
			if (percentage < 99 && percentage > 63) cout << "\033[33m";*/

			/*prntmtx.lock();
			cout << "\033["+std::to_string(cores*5)+"C\t   " << dend-dstart << " sec.\r";
			prntmtx.unlock();*/

			time_t dend = time(NULL);
			vmutex.lock();
			statusprint[4] = std::to_string(dend-dstart);
			vmutex.unlock();

		    if(100*(j)/(end)>percentage){
		    	percentage++;
		    	/*prntmtx.lock();
			    cout << prstr << std::to_string(percentage) << "%\r" << std::flush;
			    prntmtx.unlock();*/
				vmutex.lock();
				statusprint[procnum-1] = std::to_string(percentage);
				vmutex.unlock();
				print(cores);
			}
		}
		vmutex.lock();
		statusprint[procnum-1] = "100";
		vmutex.unlock();
		print(cores);
		unsigned long long result=0;
		for (size_t i=0; i<results.size(); i++) {
			result+=results[i];
		}
        P.set_value(result);
    }
    catch (...) {
        P.set_exception(std::current_exception());
    }
}

template<typename Type>
vector<unsigned long long> Calculation<Type>::calculate() {
    vector<unsigned long long> tmp;
    unsigned long long result;
	std::cout << "Calculating " << this->valuec << " number(s): \033[3m";
	for (auto num : this->values) cout << num << " ";
    std::cout << "\033[m\nUsing \033[33m" << this->cores << "\033[0m Cores\n\r";
    for (auto &num : this->values) {
    	vector<std::pair<future<unsigned long long>,thread>> threads;
        result=0;
        for(unsigned i=0;i<cores;i++) {
            std::promise<unsigned long long> Px;
            std::future<unsigned long long> Fx = Px.get_future();
            std::thread Tx {&Calculation::calc, this, std::move(Px), num,i+1,cores};
            threads.push_back(std::make_pair(std::move(Fx),std::move(Tx)));
        }
        
        for(unsigned i=0;i<cores;i++) {
            threads[i].second.join();
            result+=threads[i].first.get();
        }
        
        tmp.push_back(result);
        std::cout << std::endl;
    }
    return tmp;
}

template<typename Type>
unsigned Calculation<Type>::cores = (std::thread::hardware_concurrency()==0) ? 1 : std::thread::hardware_concurrency();

int main() {
    time_t tstart = time(NULL);
    Calculation<int> c {200,500,10};
    vector<unsigned long long> v = c.calculate();
    time_t tend = time(NULL);
	for(size_t i=0; i<v.size(); i++){
		std::cout << "M(" << c.values.at(i) << ") = " << v.at(i) << std::endl;
	}
    //std::cout << "\033[1;3m" << tend-tstart << "\033[0m second(s)." << std::endl;
    return 0;
}
