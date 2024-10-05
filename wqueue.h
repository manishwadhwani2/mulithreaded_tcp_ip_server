#include <list>
#include<iostream>
#include<mutex>
#include<condition_variable>
using namespace std;

template <typename T> class wqueue
{
    list<T>          m_queue;
    std::mutex m;
    std::condition_variable cv;
  public:
    void add(T item) {
        std::lock_guard<std::mutex>lg(m);
        m_queue.push_back(item);
        cv.notify_one();
    }
    T remove() {
        
      std::unique_lock<std::mutex> ul(m);
      if(!cv.wait_for(ul,std::chrono::seconds(60),[&]{ return m_queue.empty()?false:true;}))
      {
        return NULL;
      }
      T item = m_queue.front();
      m_queue.pop_front();
      return item;
    }
    int size() {
        std::lock_guard<std::mutex>lg(m);
        int size = m_queue.size();
        return size;
    }
};
