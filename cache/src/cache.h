
#ifndef STORAGE_SYSTEM_CACHE_H
#define STORAGE_SYSTEM_CACHE_H
#include <unordered_map>
#include <shared_mutex>
#include <thread>
#include <iostream>
#include <type_traits>
#include <utility>
#include <list>
#include <thread>

namespace cache {

//forward declaration
template<typename K, typename V>
class Cache;

template <typename K, typename V>
class Slot {

  V value_;
  std::shared_mutex mtx_;

  //declare Cache a friend class of Slot
  //because we will access the slots from the Cache object
  friend class Cache<K, V>;

public:
  explicit Slot(V value) : value_(value), mtx_() {}
  Slot() = default;

  Slot& operator=(const Slot&& obj){
    if(this == &obj){
      return *this;
    }
    value_=obj.value_;
    return *this;
  }

  void setValue(V value) {
    std::cout << "setValue(), unique lock mtx_" << std::endl;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout <<  this_id << std::endl;
    //use unique lock to get exclusive access to the slot
    std::unique_lock<std::shared_mutex> lck(mtx_);
    value_ = value;
  }

  V getValue(){
    std::cout << "getValue(), share lock mtx_" << std::endl;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout <<  this_id << std::endl;
    std::shared_lock<std::shared_mutex> lck(mtx_);
    return value_;
  }
};

template<class K>
class HashFunction; // leave entirely unimplemented

template<>
class HashFunction<std::string>{
public:
  size_t operator()(const std::string& inKey) const {
    std::hash<std::string> hash;
    return hash(inKey);
  }
};

template<>
class HashFunction<float>{
public:
  size_t operator()(const float& inKey) const {
    std::hash<float> hash;
    return hash(inKey);
  }
};

template<>
class HashFunction<int>{
public:
  size_t operator()(const int& inKey) const {
    std::hash<int> hash;
    return hash(inKey);
  }
};


template <typename K, typename V>
class Cache {

  std::shared_mutex gl_mtx_;
  std::unordered_map<K, Slot<K,V>, HashFunction<K>> cache_;

public:
  Cache() : gl_mtx_(), cache_() {}

  void put(K key, V value) {

    std::cout << "put() with key " << key << std::endl;
    //lock global shared mutex at the cache level
    std::shared_lock<std::shared_mutex> lck(gl_mtx_);

    //if key is already present in cache
    if(cache_.find(key) != cache_.end()){
      std::cout << "key already present, update value" << std::endl;
      //cache_[key].setValue(value);
    } else { // add element to cache
      std::cout << "key not found, add new element" << std::endl;
      //cache_[key];// = Slot<K,V>(value);
    }
    cache_[key].setValue(value);
  }

  V get(K key) {
    std::cout << "get() key " << key << std::endl;
    //lock global shared mutex at the cache level
    std::shared_lock<std::shared_mutex> lck(gl_mtx_);

    if(cache_.find(key) != cache_.end()){
      //if key was found, access the value from the Slot
      auto value = cache_[key].getValue();
      std::cout << "key found, value " << value << std::endl;
      return value;
    }

    //if key was not found, return empty object
    std::cout << "key not found" << std::endl;
    return V();
  }
};

}

#endif //STORAGE_SYSTEM_CACHE_H
