#include <iostream>
#include <utility>

template <class T>
class UP {
private:
    T* _ptr;

public:
    UP() : _ptr(nullptr) {}
    explicit UP(T* ptr) : _ptr(ptr) {}
    virtual ~UP() { 
        if (!_ptr)
            delete _ptr; 
    }
   
    UP(std::nullptr_t): _ptr(nullptr) {}

    UP& operator=(std::nullptr_t) {
        reset();
        return *this;
    }

    UP(UP&& rhs) noexcept : _ptr(nullptr) { rhs.swap(*this); }

    UP& operator=(UP&& rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    template<class U>
    UP(UP<U>&& rhs) {
        UP<T> tmp(rhs.release());
        tmp.swap(*this);
    }

    template<class U>
    UP& operator=(UP<U>&& rhs) {
        UP<T> tmp(rhs.release());
        tmp.swap(*this);
        return *this;
    }

    UP& operator=(const UP&) = delete;
    UP(const UP&) = delete;

    T* operator->() const { return _ptr; }
    T& operator*() const { return *_ptr; }

    T* get() const { return _ptr; }
    explicit operator bool() const { return _ptr; }

    T* release() noexcept {
        T* res = nullptr;
        std::swap(res, _ptr);
        return res;
    }

    void swap(UP& rhs) noexcept { return std::swap(_ptr, rhs._ptr); }
    void reset() {
        T* tmp = release();
        delete tmp;
    }
};

template<class T>
void swap(UP<T> lhs, UP<T>& rhs) {
    lhs.swap(rhs);
}

struct X {};
struct Y : public X {};

int main() {
    UP<X> x(new X);
    UP<Y> y(new Y);
    x = std::move(y);
    UP<X> z(std::move(y));
        
    return 0;
}
