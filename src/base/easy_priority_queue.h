/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   10:38
	filename: 	F:\easy\src\priority_queue\easy_priority_queue.h
	file path:	F:\easy\src\priority_queue
	file base:	easy_priority_queue
	file ext:	h
	author:		Lee

	purpose:	a priority queue
*********************************************************************/
#ifndef easy_priority_queue_h__
#define easy_priority_queue_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif // easy_base_type_h__

namespace easy {
class EasyPriorityQueue {
  public:
    EasyPriorityQueue() { }

    EasyPriorityQueue(easy_ulong32 priority) :priority_(priority) { }

    virtual ~EasyPriorityQueue() { }

    easy_ulong32 priority() {
        return priority_;
    }

    void set_priority( easy_ulong32 priority ) {
        priority_ = priority;
    }

    virtual easy_int32 call (void) = 0;

  protected:
    //	The priority of the request.
    easy_ulong32 priority_;
};
}
#endif // easy_priority_queue_h__