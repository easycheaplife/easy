
#include <functional>
#include <numeric>
#include <iostream>
#include <thread>

//	help function
int f(int n,char c,double d) {
    std::cout << n << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    return 1;
}

int g(int) {
    return 2;
}

// g() is overloaded
double g(double) {
    return 3;
}

// take something you can call using ()
struct int_div {
    float operator()(int x, int y) const {
        return ((float)x)/y;
    };
};

struct X {
    int foo(int) {
        return 4;
    };
};



void bind_test() {
    //	std::placeholders,it looks like closure at javascript.
    //	The std::placeholders namespace contains the placeholder objects [_1, . . . _N] where N is an implementation defined maximum number.
    //	When used as an argument in a std::bind expression, the placeholder objects are stored in the generated function object,
    //	and when that function object is invoked with unbound arguments, each placeholder _N is replaced by the corresponding Nth unbound argument.

    // deduce return type
    auto ff = std::bind(f,std::placeholders::_1,'c',1.2);
    // f(7,'c',1.2);
    int x = ff(7);

    auto fff = std::bind(f,std::placeholders::_1,std::placeholders::_2,1.2);
    // f(8,'d',1.2);
    x = fff(8,'d');

    auto ffff = std::bind(f,9,'e',1.2);
    // f(9,'e',1.2);
    x = ffff();

    // reverse argument order
    auto frev = std::bind(f,std::placeholders::_3,std::placeholders::_2,std::placeholders::_1);
    // f(7,'c',1.2);
    x = frev(1.2,'c',7);

    // error: which g()?
    //auto g1 = std::bind(g,std::placeholders::_1);
    // ok (but ugly)
    auto g2 = std::bind((double(*)(double))g,std::placeholders::_1);

    // explicit return type
    auto f2 = std::bind<int>(f,7,'c',std::placeholders::_1);
    // f(7,'c',1.2);
    x = f2(1.2);
}

void function_test() {
    //	function is a type that can hold a value of just about anything you can invoke using the (...) syntax.
    //	In particular, the result of bind can be assigned to a function. function is very simple to use. For example:
    // make a function object
    std::function<float (int x, int y)> f;
    // assign
    f = int_div();
    // call through the function object
    std::cout << f(5, 3) << std::endl;
    // passes beautifully
    int init = 100;
    int numbers[] = {10,20,30};
    std::cout << std::accumulate(numbers,numbers + 2,init,f) << std::endl;

    if(0) {
        //	Member functions can be treated as free functions with an extra argument(this pointer)
        std::function<int (X*, int)> f1;
        // pointer to member
        //	something wrong! not support by vs2012?
        //	gcc version 4.8.2 is correct!
        //	f1 = &X::foo;
        X x;
        // call X::foo() for x with 5
        int v = f1(&x, 5);
        // first argument for f is &x
        std::function<int (int)> ff1 = std::bind(f1,&x,std::placeholders::_1);
        // call x.foo(5)
        v=ff1(5);
    }

}

void independentThread() {
    std::cout << "Starting concurrent thread.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Exiting concurrent thread.\n";
}

void threadCaller() {
    std::cout << "Starting thread caller.\n";
    std::thread t(independentThread);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Exiting thread caller.\n";
}

void thread_test() {
    threadCaller();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
int main() {
    bind_test();
    function_test();
    thread_test();
    return 0;
}

