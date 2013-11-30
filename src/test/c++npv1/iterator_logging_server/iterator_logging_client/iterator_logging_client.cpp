// iterator_logging_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ace/Log_Record.h"
#include "ace/SOCK_Stream.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"
#include "ace/os_include/sys/os_uio.h"
#include "ace/SOCK_Connector.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Truncate.h"
#include <iostream>
#include <string>


class logging_client
{
public:
	int send(const ACE_Log_Record& __log_record)
	{
		//FUZZ: enable check_for_lack_ACE_OS

		// Serialize the log record using a CDR stream, allocate
		// enough space for the complete <ACE_Log_Record>.
		const size_t max_payload_size =
			4 // type()
			+ 8 // timestamp
			+ 4 // process id
			+ 4 // data length
			+ ACE_Log_Record::MAXLOGMSGLEN // data
			+ ACE_CDR::MAX_ALIGNMENT; // padding;

		// Insert contents of <log_record> into payload stream.
		ACE_OutputCDR payload (max_payload_size);
		payload << __log_record;

		// Get the number of bytes used by the CDR stream.
		ACE_CDR::ULong length = ACE_Utils::truncate_cast<ACE_CDR::ULong> (payload.total_length ());

		// Send a header so the receiver can determine the byte
		// order and size of the incoming CDR stream.
		ACE_OutputCDR header (ACE_CDR::MAX_ALIGNMENT + 8);
		header << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);

		// Store the size of the payload that follows
		header << ACE_CDR::ULong (length);
		// Use an iovec to send both buffer and payload simultaneously.
		iovec iov[2];
		iov[0].iov_base = header.begin ()->rd_ptr ();
		iov[0].iov_len  = 8;
		iov[1].iov_base = payload.begin ()->rd_ptr ();
		iov[1].iov_len  = length;

		// Send header and payload efficiently using "gather-write".
		return logging_peer_.sendv_n (iov, 2);
	}

	~logging_client() { logging_peer_.close(); }

	ACE_SOCK_Stream& peer() { return logging_peer_;}
private:
	ACE_SOCK_Stream logging_peer_;
};
int _tmain(int argc, _TCHAR* argv[])
{
	ACE_INET_Addr __server_addr;
	int __result = __server_addr.set(9876,ACE_DEFAULT_SERVER_HOST);
	ACE_SOCK_Connector	__connector;
	logging_client __logging_client;
	__connector.connect(__logging_client.peer(),__server_addr);
	std::cin.width(ACE_Log_Record::MAXLOGMSGLEN);
	for (;;)
	{
		std::string __user_input;
		std::getline(std::cin,__user_input,'\n');
		if (!std::cin || std::cin.eof())
		{
			break;
		}
		ACE_Time_Value __now(ACE_OS::gettimeofday());
		ACE_Log_Record __log_record(LM_INFO,__now,ACE_OS::getpid());
		__log_record.msg_data(__user_input.c_str());
		int __send_bytes = __logging_client.send(__log_record);
		if (__send_bytes == -1)
		{
			DWORD __err_code = ::GetLastError();
			return -1;
		}
	}
	return 0;
}

