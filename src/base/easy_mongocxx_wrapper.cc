#include"easy_mongocxx_wrapper.h"

#ifndef verify
#  define verify(x) MONGO_verify(x)
#endif

namespace easy
{
	MongocxxWrapper* MongocxxWrapper::inst_ = NULL;

	MongocxxWrapper* MongocxxWrapper::instance(const char* __uri_string /*= "192.168.22.66:27017"*/)
	{
		if(!inst_){
			inst_ = new MongocxxWrapper(__uri_string);
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

	MongocxxWrapper::MongocxxWrapper(const char* __uri_string)
	{
		//	init mongo
		mongo::Status __status = mongo::client::initialize();
		if ( !__status.isOK() ) 
		{
			printf ("failed to initialize the client driver: %s\n",__status.toString().c_str());
		}
		std::string __err_msg;
		if ( ! conn_.connect( __uri_string , __err_msg ) ) {
			printf("couldn't connect : %s\n",__err_msg.c_str());
		}
		printf("connect mongo server %s ok\n",__uri_string);
	}

	MongocxxWrapper::~MongocxxWrapper()
	{
		destrory();
	}
}
