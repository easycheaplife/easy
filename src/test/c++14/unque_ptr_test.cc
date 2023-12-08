#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <fstream>
#include <functional>

struct Vec3 {
  public:
    int x_;
    int y_;
    int z_;
    Vec3() {
        x_ = 0;
        y_ = 0;
        z_ = 0;
    }
    Vec3(int x, int y, int z) {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    friend std::ostream& operator<<(std::ostream& os, Vec3& v) {
        return os << '{' << "x:" << v.x_ << " y:" << v.y_ << " z:" << v.z_  << '}' << std::endl;
    }
};

struct B {
    virtual void bar() {
        std::cout << "B::bar\n";
    }
    virtual ~B() = default;
};
struct D : B {
    D() {
        std::cout << "D::D\n";
    }
    ~D() {
        std::cout << "D::~D\n";
    }
    void bar() override {
        std::cout << "D::bar\n";
    }
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
    p->bar();
    return p;
}

void close_file(std::FILE* fp) {
    std::fclose(fp);
}

// g++ -g -std=c++14 -o unque_ptr_test unque_ptr_test.cc
int main() {
    uint8_t s1[1024] = {};
    std::cout << typeid(s1).name() << std::endl;
    std::vector<uint8_t> s2;
    s2.push_back('a');
    s2.push_back('b');
    std::cout << typeid(s2.data()).name() << std::endl;
    std::cout << s2.data() << std::endl;

    // http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/memory/unique_ptr/make_unique.html
    // default contruct function
    std::unique_ptr<Vec3> v1 = std::make_unique<Vec3>();
    // contruct function
    std::unique_ptr<Vec3> v2 = std::make_unique<Vec3>(1,3,4);
    auto v4 = std::make_unique<Vec3>(1,3,4);
    // array
    std::unique_ptr<Vec3[]> v3 = std::make_unique<Vec3[]>(5);
    std::cout << *v1;
    std::cout << *v2;
    std::cout << *v4;
    for (int i = 0; i < 5; ++i) {
        std::cout << "     " << v3[i] << '\n';
    }

    // output by ->
    std::cout << "v2->x_" << v2->x_ << "v2->y_" << v2->y_ << "v2->z_" << v2->z_ << std::endl;

    // other test
    auto c1 = std::make_unique<char[]>(10);
    std::cout << typeid(c1).name() << std::endl;

    /*
     * If T is a derived class of some base B, then std::unique_ptr<T> is implicitly convertible to std::unique_ptr<B>.
     * The default deleter of the resulting std::unique_ptr<B> will use operator delete for B,
     * leading to undefined behavior unless the destructor of B is virtual.
     * Note that std::shared_ptr behaves differently: std::shared_ptr<B> will use the operator delete for the type T and the owned object will be deleted correctly
     * even if the destructor of B is not virtual.
     * more detail: https://en.cppreference.com/w/cpp/memory/unique_ptr
     */
    std::cout << "unique ownership semantics demo\n";
    {
        auto p = std::make_unique<D>(); // p is a unique_ptr that owns a D
        auto q = pass_through(std::move(p));
        assert(!p); // now p owns nothing and holds a null pointer
        q->bar();   // and q owns the D object
    }// ~D called here

    std::cout << "Runtime polymorphism demo\n";
    {
        std::unique_ptr<B> p = std::make_unique<D>(); // p is a unique_ptr that owns a D
        // as a pointer to base
        p->bar(); // virtual dispatch
        std::vector<std::unique_ptr<B>> v;  // unique_ptr can be stored in a container
        v.push_back(std::make_unique<D>());
        v.push_back(std::move(p));
        v.emplace_back(new D);
        for(auto& p: v) p->bar(); // virtual dispatch
    } // ~D called 3 times

    std::cout << "Custom deleter demo\n";
    std::ofstream("demo.txt") << 'x'; // prepare the file to read
    {
        std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
                &close_file);
        if(fp) // fopen could have failed; in which case fp holds a null pointer
            std::cout << (char)std::fgetc(fp.get()) << '\n';

    } // fclose() called here, but only if FILE* is not a null pointer
    // (that is, if fopen succeeded)

    std::cout << "Custom lambda-expression deleter demo\n";
    {
        std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr) {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;

        });	// p owns D
        p->bar();
    } // the lambda above is called and D is destroyed

    std::cout << "Array form of unique_ptr demo\n";
    {
        std::unique_ptr<D[]> p{new D[3]};
    } // calls ~D 3 times
    return 0;
}

