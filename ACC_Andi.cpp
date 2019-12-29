//!/bin/g++

#include <future>
#include <thread>
#include <mutex>
#include <exception>
#include <vector>
#include <tuple>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <numeric>
#include <utility>
#include <fstream>
#include <limits>

using std::vector; using std::string; using std::cout; using std::endl; using std::initializer_list; using std::move;
using std::fstream; using std::tuple;

std::vector<long long> cache;
std::mutex mtx;

template<typename Type>
struct Calculation {
    using calc_t = long long;
    
    string file;
    vector<Type> cache;
    vector<Type> values;
    static unsigned cores=std::thread::hardware_concurrency();

    Calculation(initializer_list<Type> initl) {
        values.insert(values.end(), initl.begin(), initl.end());
    }
    Calculation(int argc, char* argv[]) : cache{0} {
        char* mode = *argv+1;
        char* startc = *argv+2;
        char* endc = *argv+argc;
        switch(*argv[1]) {
            case *"-v":
                for(char* i=startc; i!=endc; ++i)
                    values.push_back((int)*i);
                break;
        }
    }

    void calc(std::promise<long long> P, long end, int procnum, unsigned cores);
    //template<typename NumType>
    long long calculate();

};

void Calculation::calc(std::promise<long long> P, long end, int procnum, unsigned cores) {
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

//template<typename NumType>
tuple Calculation::calculate() {
    vector<long long> tmp;
    std::cout << "Using " << Calculation::cores << " Cores\n\r";
    for (auto &num : this->values) {
        std::vector<std::pair<std::future<long long>,std::thread>> threads;
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

        tmp.push_back(result);
    }
    return std::make_tuple(tmp);
}

int main(int argc, char* argv[]) {
    initializer_list<int> l = {20, 20};
    Calculation c(l);
    c.calculate();

}
