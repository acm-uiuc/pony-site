#include <boost/thread/recursive_mutex.hpp>

template <typename T, typename Mutex = boost::recursive_mutex, 
          typename Lock = typename Mutex::scoped_lock>
class LockingPtr {
    public:
        // Constructors/destructors
        LockingPtr(volatile T& obj, Mutex& mtx)
            : pObj_(const_cast<T*>(&obj)), l_(mtx) { }
        ~LockingPtr() { }
        // Pointer behavior
        T& operator*() {  return *pObj_;    }
        T* operator->() {   return pObj_;   }
    private:
        T* pObj_;
        Lock l_;
        //prevent copying
        LockingPtr(const LockingPtr&);
        LockingPtr& operator=(const LockingPtr&);
};

struct MyObj {
   int foo() { return 3; }
};

