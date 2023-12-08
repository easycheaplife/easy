#include<iostream>

int main() {
#ifdef __WINDOWS
    std::cout << "__WINDOWS" <<std::endl;
#else __LINUX
    std::cout << "__LINUX" <<std::endl;
#endif // __WINDOWS
    return 0;
}
