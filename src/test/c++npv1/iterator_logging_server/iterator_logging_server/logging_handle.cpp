#include "logging_handle.h"
#include "ace/Message_Block.h"
#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_string.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"
#include "ace/Log_Record.h"
#include "ace/Truncate.h"

int logging_handle::recv_log_record( ACE_Message_Block*& __mblk )
{
	ACE_INET_Addr __peer_addr;
	logging_peer_.get_remote_addr(__peer_addr);
	__mblk = new ACE_Message_Block(MAXHOSTNAMELEN + 1);
	__peer_addr.get_host_name(__mblk->wr_ptr(),MAXHOSTNAMELEN);
	__mblk->wr_ptr(ACE_OS::strlen(__mblk->wr_ptr()) + 1);

	ACE_Message_Block* __payload = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
	ACE_CDR::mb_align(__payload);
	if (8 == logging_peer_.recv_n(__payload->wr_ptr(),8))
	{
		__payload->wr_ptr(8);
	}

	ACE_InputCDR __cdr(__payload);
	ACE_CDR::Boolean __byte_order;
	__cdr >> ACE_InputCDR::to_boolean(__byte_order);
	__cdr.reset_byte_order(__byte_order);

	ACE_CDR::ULong __length;
	__cdr >> __length;

	ACE_CDR::grow(__payload,ACE_CDR::MAX_ALIGNMENT + __length);
	if (logging_peer_.recv_n(__payload->wr_ptr(),__length) > 0)
	{
		__payload->wr_ptr(__length);
		__mblk->cont(__payload);
		return __length;
	}
	__payload->release();
	__payload = 0;
	__mblk->release();
	__mblk = 0;
	return -1;
}

int logging_handle::write_log_record( ACE_Message_Block* __mblk )
{
	if (-1 == logging_file_.send_n(__mblk))
	{
		return -1;
	}

	ACE_InputCDR __cdr(__mblk->cont());
	ACE_CDR::Boolean __byte_order;
	ACE_CDR::ULong __length;
	__cdr >> ACE_InputCDR::to_boolean(__byte_order);
	__cdr.reset_byte_order(__byte_order);
	__cdr >> __length;
	ACE_Log_Record __log_record;
	__cdr >> __log_record;
	__log_record.print(__mblk->rd_ptr (), 1, stderr);

	return ACE_Utils::truncate_cast<int>(__mblk->total_length());
}

int logging_handle::log_record()
{
	ACE_Message_Block *mblk = 0;
	if (recv_log_record (mblk) == -1)
		return -1;
	else 
	{
		int result = write_log_record (mblk);
		mblk->release (); // Free up the contents.
		return result == -1 ? -1 : 0;
	}
}
