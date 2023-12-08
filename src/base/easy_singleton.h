/********************************************************************
	created:	2011/10/21
	created:	21:10:2011   18:14
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_singleton.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_singleton
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/

#ifndef easy_singleton_h__
#define easy_singleton_h__

#include <assert.h>
#ifndef easy_thread_mutex_h__
#include "easy_thread_mutex.h"
#endif //easy_thread_mutex_h__

namespace easy {
template <class TYPE>
class EasySingleton {
  public:
    /// Global access point to the Singleton.
    static TYPE *Instance (void) {
        EasySingleton<TYPE> *&singleton = EasySingleton<TYPE>::_instance_i ();
        if (0 == singleton) {
            singleton = new EasySingleton<TYPE>();
        }
        return &singleton->instance_;
    }

    /// Explicitly delete the Singleton instance.
    static void Close (void) {
        EasySingleton<TYPE>*& singleton = EasySingleton<TYPE>::_instance_i();
        if (singleton) {
            singleton->_cleanup();
        }
    }

  protected:
    /// Default constructor.
    EasySingleton (void) { }

    virtual ~EasySingleton() {}

    /// Get pointer to the Singleton instance.
    static EasySingleton<TYPE> *&_instance_i (void) {
        static EasySingleton<TYPE>* sigleton = 0;
        return sigleton;
    }

    /// Cleanup method, used by @c ace_cleanup_destroyer to destroy the
    /// ACE_Singleton.
    virtual void _cleanup (void *param = 0) {
        delete this;
        EasySingleton<TYPE>::_instance_i () = 0;
    }
  private:
    /// Contained instance.
    TYPE instance_;
};

template<class T>
class Singleton {
  public:
    Singleton() {
        assert(!singleton_);
        singleton_ = static_cast<T*>(this);
    }

    ~Singleton() {
        assert(singleton_);
        singleton_ = 0;
    }

    static Singleton& GetSingleton() {
        return *singleton_;
    }

    static Singleton* GetSingletonPtr() {
        return singleton_;
    }
  private:
    static T*	singleton_;

  private:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
};
//usage:	template<> Test* Singleton<Test*>::singleton_ = 0;
}
#endif // easy_singleton_h__