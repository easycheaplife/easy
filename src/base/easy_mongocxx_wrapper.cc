#include"easy_mongocxx_wrapper.h"

#ifndef verify
#  define verify(x) MONGO_verify(x)
#endif

namespace easy
{
	MongocxxWrapper* MongocxxWrapper::inst_ = NULL;

	MongocxxWrapper* MongocxxWrapper::instance()
	{
		if(!inst_){
			inst_ = new MongocxxWrapper();
		}
		return inst_;
	}

	void MongocxxWrapper::destrory()
	{
		if (inst_)
		{
			delete inst_;
			inst_ = NULL;
		}
	}

	MongocxxWrapper::MongocxxWrapper(const char* __db, const char* __uri_string)
	{
		const char* __mongo_host = "192.168.22.61:";
		const char* __mongo_port = "27017";
		//	init mongo
		mongo::Status __status = mongo::client::initialize();
		if ( !__status.isOK() ) 
		{
			printf ("failed to initialize the client driver: %s\n",__status.toString().c_str());
		}
		mongo::DBClientConnection __conn;
		std::string __err_msg;
		if ( ! __conn.connect( std::string( __mongo_host ) + __mongo_port , __err_msg ) ) {
			printf("couldn't connect : %s\n",__err_msg.c_str());
		}
		printf("connect mongo server %s %s ok\n",__mongo_host,__mongo_port);
		//	test mongo code begin
		const char * __ns = "test.test1";
		__conn.dropCollection(__ns);
		// clean up old data from any previous tests
		__conn.remove( __ns, mongo::BSONObj() );
		verify( __conn.findOne( __ns , mongo::BSONObj() ).isEmpty() );
		// test insert
		__conn.insert( __ns ,BSON( "name" << "eliot" << "num" << 1 ) );
		verify( ! __conn.findOne( __ns , mongo::BSONObj() ).isEmpty() );
		//	test mongo code end
		printf("mongo dxx test\n");
	}

	MongocxxWrapper::~MongocxxWrapper()
	{
		destrory();
	}
}
