#include "easy_util.h"

#ifdef __LINUX
#include <string.h>
#endif // __LINUX

namespace easy {
Tokens::Tokens( const std::string &src, const char sep, easy_uint32 vector_reserve /*= 0*/ ) {
    m_str = new char[src.length() + 1];
    memcpy(m_str, src.c_str(), src.length() + 1);

    if (vector_reserve)
        reserve(vector_reserve);

    char* posold = m_str;
    char* posnew = m_str;

    for (;;) {
        if (*posnew == sep) {
            push_back(posold);
            posold = posnew + 1;

            *posnew = 0x00;
        } else if (*posnew == 0x00) {
            // Hack like, but the old code accepted these kind of broken strings,
            // so changing it would break other things
            if (posold != posnew)
                push_back(posold);

            break;
        }

        ++posnew;
    }
}

}