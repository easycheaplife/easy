#include "easy_atomic_op.h"

namespace easy {
long multi_cpu_increment (volatile long *value) {
    long tmp = 1;
    //...
    return tmp + 1;
}

long
multi_cpu_decrement (volatile long *value) {
    long tmp = 1;
    //...
    return tmp + 1;
}

long multi_cpu_exchange (volatile long *value, long rhs) {
    unsigned long addr = reinterpret_cast<unsigned long> (value);
    if(addr) {}	//	warning: unused variable ‘addr’ [-Wunused-variable]
    //...
    return rhs;

}

long multi_cpu_exchange_add (volatile long *value, long rhs) {
    unsigned long addr = reinterpret_cast<unsigned long> (value);
    //...
    if(addr) {}	//	warning: unused variable ‘addr’ [-Wunused-variable]
    return rhs;
}

long (*EasyAtomicOp<EasyMutex, long>::increment_fn_) (volatile long *) = multi_cpu_increment;
long (*EasyAtomicOp<EasyMutex, long>::decrement_fn_) (volatile long *) = multi_cpu_decrement;
long (*EasyAtomicOp<EasyMutex, long>::exchange_fn_) (volatile long *, long) = multi_cpu_exchange;
long (*EasyAtomicOp<EasyMutex, long>::exchange_add_fn_) (volatile long *, long) = multi_cpu_exchange_add;
}