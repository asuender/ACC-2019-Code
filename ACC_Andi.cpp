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
#include <chrono>

std::vector<unsigned long long> cache;
std::vector<std::string> statusprint(5);
std::mutex cachemtx;	//mutex for the cache vector
//std::mutex prntmtx;		//mutex for printing stuff
std::mutex vmutex; 		//mutex for printing vector

using std::vector; using std::tuple; using std::cout; using std::endl; using std::string;
using std::initializer_list; using std::future; using std::promise; using std::thread;

struct TimeObject {
	time_t _times = time(NULL); //start time
	bool _end = false;
	string act_time;
};


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
	vmutex.lock();
	for (size_t t=1; t<statusprint.size(); t++) {
		cout << "\r\033["+std::to_string(t*5)+"C" << statusprint[t-1] << "%\r";
	}
	cout << "\033["+std::to_string(cores*5)+"C\t\t" << statusprint[4] << std::flush;
	vmutex.unlock();
}

void count(TimeObject &t, int cores) {

	vmutex.lock();
	statusprint[4] = "0";
	vmutex.unlock();

	while (t._end == false) {
		vmutex.lock();
		auto timediff = time(NULL) - t._times;
		if (timediff > 60) {
			statusprint[4] = std::to_string(timediff/60) + " min. "+ std::to_string(timediff%60) + " sec.";
		}
		else {
			statusprint[4] = std::to_string(time(NULL) - t._times) + " sec.";
		}
		vmutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		print(cores);
	}
}

template<typename Type>
void Calculation<Type>::calc(std::promise<unsigned long long> P, long end, unsigned int procnum, unsigned cores) {
    try {
		//time_t dstart = time(NULL);

        std::vector<unsigned long long> results;
		std::string prstr="\033["+std::to_string(procnum*5)+"C";
		statusprint[procnum-1] = "0";
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

			/*time_t dend = time(NULL);
			vmutex.lock();
			statusprint[4] = std::to_string(dend-dstart);
			vmutex.unlock();*/

		    if(100*(j)/(end)>percentage){
		    	percentage++;
				vmutex.lock();
				statusprint[procnum-1] = std::to_string(percentage);
				vmutex.unlock();
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
		TimeObject Counter;
		std::thread th {count, std::ref(Counter), cores};
		th.detach();

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
        
		Counter._end = true;
        tmp.push_back(result);
        std::cout << std::endl;
    }
    return tmp;
}

template<typename Type>
unsigned Calculation<Type>::cores = (std::thread::hardware_concurrency()==0) ? 1 : std::thread::hardware_concurrency();

int main() {
    Calculation<int> c {10, 200, 500, 1000, 10000};
    vector<unsigned long long> v = c.calculate();
	for(size_t i=0; i<v.size(); i++){
		std::cout << "M(" << c.values.at(i) << ") = " << v.at(i) << std::endl;
	}
    //std::cout << "\033[1;3m" << tend-tstart << "\033[0m second(s)." << std::endl;
    return 0;
}
