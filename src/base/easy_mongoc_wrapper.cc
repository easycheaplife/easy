#include"easy_mongoc_wrapper.h"
#ifdef __USE_MONGOC
namespace easy
{
	MongocWrapper* MongocWrapper::inst_ = NULL;

	MongocWrapper* MongocWrapper::instance()
	{
		if(!inst_){
			inst_ = new MongocWrapper();
		}
		return inst_;
	}

	void MongocWrapper::destrory()
	{
		if (inst_)
		{
			delete inst_;
			inst_ = NULL;
		}
	}

	MongocWrapper::MongocWrapper(const char* __db, const char* __uri_string)
	{
		//	init mongoc
		mongoc_init();
		client_ = mongoc_client_new (__uri_string);
		if (!client_) {
			fprintf (stderr, "Failed to parse URI.\n");
		}
		if(__db){
			db_ = __db;
		}
		else
		{
			db_ = "test";
		}
		
	}

	MongocWrapper::~MongocWrapper()
	{
		destrory();
	}

	mongoc_collection_t* MongocWrapper::get_collection( const char* __collection_name )
	{
		if (!__collection_name)
		{
			return NULL;
		}
		if(collections_[__collection_name]){
			return collections_[__collection_name];
		}
		mongoc_collection_t* __collection = mongoc_client_get_collection (client_, db_.c_str(), __collection_name);
		if (__collection)
		{
			collections_[__collection_name] = __collection;
		}
		return __collection;
	}

	bool MongocWrapper::collection_insert( const char* __collection_name,bson_t* __doc )
	{
		bson_error_t __error;
		bool __result = false;
		if (!(__result = mongoc_collection_insert (get_collection(__collection_name), MONGOC_INSERT_NONE, __doc, NULL, &__error))) {
			printf ("%s\n", __error.message);
		}
		bson_destroy (__doc);
		return __result;
	}

	mongoc_cursor_t* MongocWrapper::collection_find( const char* __collection_name, bson_t* __query )
	{
		//	find a doc
		mongoc_cursor_t* __cursor = mongoc_collection_find (get_collection(__collection_name),
			MONGOC_QUERY_NONE,
			0,
			0,
			0,
			__query,
			NULL,  /* Fields, NULL for all. */
			NULL); /* Read Prefs, NULL for default */
		bson_destroy (__query);
		return __cursor;
	}
}

#endif //__USE_MONGOC