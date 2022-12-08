#include <iostream>
#include <utility>

using namespace std;

template <class T>
class SP {
private:
    T* _ptr;
    int* _ref_ct;

public:
    SP() : _ptr(nullptr), _ref_ct(nullptr) {}
    explicit SP(T* ptr) : _ptr(ptr), _ref_ct(new int(1)) {}
    ~SP() {
        __delete__();
        cout << "dtor called" << endl;
    }
    SP(const SP& rhs) : _ptr(rhs._ptr), _ref_ct(rhs._ref_ct) {
        cout << "copy ctor called" << endl;
        (*_ref_ct)++;
    }
    SP(SP&& rhs) : _ptr(nullptr), _ref_ct(nullptr) {
        cout << "move copy ctor called" << endl;
        rhs.swap(*this);
    }

    SP& operator=(const SP& rhs) {
        cout << "operator=(const SP&) called" << endl;
        SP tmp(rhs);
        tmp.swap(*this);
        return *this;
    }

    SP& operator=(SP&& rhs) {
        cout << "operator=(SP&&) called" << endl;
        rhs.swap(*this);
        return *this;
    }

    SP& operator=(T* ptr) {
        SP tmp(ptr);
        tmp.swap(*this);
        return *this;
    }

    void swap(SP& rhs) noexcept {
        std::swap(_ptr, rhs._ptr);
        std::swap(_ref_ct, rhs._ref_ct);
    }

    int get_count() const {
        if (_ref_ct)
            return *_ref_ct;
        else
            return 0;
    }

    T* get() const {
        return _ptr;
    }

    T* operator->() const {
        return _ptr;
    }

    T& operator*() const {
        return *_ptr;
    }

private:
    void __delete__() {
        if (_ref_ct) {
            (*_ref_ct)--;
            if (*_ref_ct == 0) {
                if (!_ptr)
                    delete _ptr;
                delete _ref_ct;
            }
        }
    }
};

class Foo 
{
public:
    int len, width, ht;
    Foo() : len(0), width(0), ht(0) {}
};

int main() {
    SP<Foo> foo;
    cout << "foo ref="<< foo.get_count() << endl;

    SP<Foo> foo1(new Foo());
    cout << "foo1 ref=" << foo1.get_count() << endl; // prints 1
    SP<Foo> foo2(foo1); // calls copy constructor
    auto foo3 = foo2;
    foo3 = foo1;
    foo3 = SP<Foo>(new Foo());
    cout << "foo1 ref="<< foo1.get_count() << endl; // prints 2
    cout << "foo2 ref=" << foo2.get_count() << endl; // also prints 2
    cout << "foo3 ref=" << foo3.get_count() << endl; // also prints 2
    SP<Foo> foo4(std::move(foo2));
    cout << "foo4 ref=" << foo4.get_count() << endl; // also prints 2
    SP<Foo> f5;
    auto f6 = std::move(f5);
    return 0;
}

