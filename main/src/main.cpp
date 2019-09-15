#include <iostream>
#include <cache/src/cache.h>
#include <vector>
#include <time.h>
using namespace cache;

void newThread() {

}

int main() {

  srand (time(NULL));
  Cache<int,int> ca = Cache<int,int>();

  std::vector<std::thread> threads;
  for (int i = 0; i<10; i++) {
    threads.emplace_back(std::thread([&](){
      while(true){
        ca.get(std::rand()%10);
        ca.put(std::rand()%10,std::rand());
      }
    }));
  }

  for(std::vector<std::thread>::size_type i=0; i<threads.size(); i++){
    threads[i].join();
  }

  return 0;
}