/* 
	compiler:
		$gcc -g -o example-client example-client.c -I../../../dep/mongo-c-driver/src/mongoc -I../../../dep/mongo-c-driver/src/libbson/src/bson -L../../../dep/mongo-c-driver/src/libbson/.libs -L../../../dep/mongo-c-driver/.libs  -lbson -lpthread -lmongoc-1.0 
	run:
		$export LD_LIBRARY_PATH=../../../dep/mongo-c-driver/.libs:../../../dep/mongo-c-driver/src/libbson/.libs
		$./example-client
*/
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
int
main (int   argc,
      char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   bson_error_t error;
   bson_oid_t oid;
   const bson_t *doc;
   const char *uristr = "mongodb://192.168.22.61/";
   const char *collection_name = "test";
   bson_t query;
   char *str;

   mongoc_init ();

   if (argc > 1) {
      uristr = argv [1];
   }

   if (argc > 2) {
      collection_name = argv [2];
   }

   client = mongoc_client_new (uristr);

   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }

   bson_init (&query);

#if 0
   bson_append_utf8 (&query, "hello", -1, "world", -1);
#endif

   collection = mongoc_client_get_collection (client, "test", collection_name);
   //	add mongo doc
   
   doc = bson_new ();
   bson_oid_init (&oid, NULL);
   BSON_APPEND_OID (doc, "_id", &oid);
   BSON_APPEND_UTF8 (doc, "hello", "world");
   if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
        printf ("%s\n", error.message);
    }
   bson_destroy (doc);

   //	find mongo doc
   cursor = mongoc_collection_find (collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    0,
                                    0,
                                    &query,
                                    NULL,  /* Fields, NULL for all. */
                                    NULL); /* Read Prefs, NULL for default */

   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      fprintf (stdout, "%s\n", str);
      bson_free (str);
   }

   if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "Cursor Failure: %s\n", error.message);
      return EXIT_FAILURE;
   }
   
   //	del mongo doc
   doc = bson_new ();
   BSON_APPEND_OID (doc, "_id", &oid);
   if (!mongoc_collection_delete (collection, MONGOC_DELETE_SINGLE_REMOVE, doc, NULL, &error)) {
		printf ("Delete failed: %s\n", error.message);
   }

   bson_destroy (&query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
