#include"easy_mongocxx_wrapper.h"
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
		//	init mongo
		mongo::Status __status = mongo::client::initialize();
		if ( !__status.isOK() ) 
		{
			printf ("failed to initialize the client driver: %s\n",__status.toString().c_str());
			return;
		}
		std::string __err_msg;
		if ( ! conn_.connect( __uri_string , __err_msg ) ) {
			printf("couldn't connect : %s\n",__err_msg.c_str());
			return;
		}
		printf("connect mongo server %s ok\n",__uri_string);
		if(__db){
			db_ = __db;
		}
		else
		{
			db_ = "test";
		}
	}

	MongocxxWrapper::~MongocxxWrapper()
	{
		destrory();
	}
}
