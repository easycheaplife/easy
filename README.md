easy,cross platform c++ class lib (makefile version) which include database,network,thread,lock.buffer,and so on. at the directory of src/test you will find some test code,you can use at windows or linux to do unit test.
winodws:
	use vistual studio 2012 and just add the test code to you project.
	
linux:
	change the src/test/cppunit/Makefile,some thing like this:
	#****************************************************************************
	# Source files
	# test file shold be the first line, you should not be a empty line
	#****************************************************************************
	OBJS = easy_main.o
	SRCS = easy_main.cc \
	../easy_test_lock.cc \
	../easy_test_allocator.cc \
	../easy_test_threading.cc \
	../stl/stl_map_test.cc \
	../easy_test_dump.cc \
	../stl/stl_advance_test.cc \
	../easy_unit_test_template.cc \
	../easy_test_log.cc \
	../easy_test_singleton.cc \
	../easy_test_type_traits.cc \
	../easy_test_template.cpp\
	../easy_test_list.cc\
	../easy_test_locked_queue.cc\
	../easy_test_byte_buffer.cc \
	../easy_test_ring_buffer.cc \
	../easy_test_stdandard_allocator.cc \
	and compile at src/build/Makefile,it will find all sub directory and compile everyone.
	
	enjoy it !
*************************************************************************************
$tree
.
├── build
│   ├── base
│   │   ├── easy_atomic_op.o
│   │   ├── easy_big_int.o
│   │   ├── easy_exception.o
│   │   ├── easy_log.o
│   │   ├── easy_threading.o
│   │   ├── easy_util.o
│   │   ├── Makefile
│   │   ├── myadd.o
│   │   └── mysub.o
│   ├── easy.sln
│   ├── easy.suo
│   ├── easy.vcxproj
│   ├── easy.vcxproj.filters
│   ├── easy.vcxproj.user
│   ├── easy_vs2012.sln
│   ├── easy_vs2012.suo
│   ├── easy_vs2012.v11.suo
│   ├── Makefile
│   └── tinyxml
│       └── Makefile
├── cygwin
│   ├── builed.bat
│   ├── debug.bat
│   └── test.bat
├── dep
│   ├── libevent
│   │   ├── aclocal.m4
│   │   ├── arc4random.c
│   │   ├── autogen.sh
│   │   ├── autom4te.cache
│   │   │   ├── output.0
│   │   │   ├── requests
│   │   │   └── traces.0
│   │   ├── buffer.c
│   │   ├── bufferevent_async.c
│   │   ├── bufferevent.c
│   │   ├── bufferevent_filter.c
│   │   ├── bufferevent_filter.lo
│   │   ├── bufferevent_filter.o
│   │   ├── bufferevent-internal.h
│   │   ├── bufferevent.lo
│   │   ├── bufferevent.o
│   │   ├── bufferevent_openssl.c
│   │   ├── bufferevent_openssl.lo
│   │   ├── bufferevent_openssl.o
│   │   ├── bufferevent_pair.c
│   │   ├── bufferevent_pair.lo
│   │   ├── bufferevent_pair.o
│   │   ├── bufferevent_ratelim.c
│   │   ├── bufferevent_ratelim.lo
│   │   ├── bufferevent_ratelim.o
│   │   ├── bufferevent_sock.c
│   │   ├── bufferevent_sock.lo
│   │   ├── bufferevent_sock.o
│   │   ├── buffer_iocp.c
│   │   ├── buffer.lo
│   │   ├── buffer.o
│   │   ├── changelist-internal.h
│   │   ├── ChangeLog
│   │   ├── compat
│   │   │   └── sys
│   │   │       └── queue.h
│   │   ├── compile
│   │   ├── config.guess
│   │   ├── config.h
│   │   ├── config.h.in
│   │   ├── config.log
│   │   ├── config.status
│   │   ├── config.sub
│   │   ├── configure
│   │   ├── configure.in
│   │   ├── defer-internal.h
│   │   ├── depcomp
│   │   ├── devpoll.c
│   │   ├── Doxyfile
│   │   ├── epoll.c
│   │   ├── epoll.lo
│   │   ├── epoll.o
│   │   ├── epoll_sub.c
│   │   ├── evbuffer-internal.h
│   │   ├── evdns.c
│   │   ├── evdns.h
│   │   ├── evdns.lo
│   │   ├── evdns.o
│   │   ├── event.c
│   │   ├── event.h
│   │   ├── event-internal.h
│   │   ├── event_iocp.c
│   │   ├── event.lo
│   │   ├── event.o
│   │   ├── event_rpcgen.py
│   │   ├── event_tagging.c
│   │   ├── event_tagging.lo
│   │   ├── event_tagging.o
│   │   ├── evhttp.h
│   │   ├── evmap.c
│   │   ├── evmap-internal.h
│   │   ├── evmap.lo
│   │   ├── evmap.o
│   │   ├── evport.c
│   │   ├── evrpc.c
│   │   ├── evrpc.h
│   │   ├── evrpc-internal.h
│   │   ├── evrpc.lo
│   │   ├── evrpc.o
│   │   ├── evsignal-internal.h
│   │   ├── evthread.c
│   │   ├── evthread-internal.h
│   │   ├── evthread.lo
│   │   ├── evthread.o
│   │   ├── evthread_pthread.c
│   │   ├── evthread_pthread.lo
│   │   ├── evthread_pthread.o
│   │   ├── evthread_win32.c
│   │   ├── evutil.c
│   │   ├── evutil.h
│   │   ├── evutil.lo
│   │   ├── evutil.o
│   │   ├── evutil_rand.c
│   │   ├── evutil_rand.lo
│   │   ├── evutil_rand.o
│   │   ├── ht-internal.h
│   │   ├── http.c
│   │   ├── http-internal.h
│   │   ├── http.lo
│   │   ├── http.o
│   │   ├── include
│   │   │   ├── event2
│   │   │   │   ├── buffer_compat.h
│   │   │   │   ├── bufferevent_compat.h
│   │   │   │   ├── bufferevent.h
│   │   │   │   ├── bufferevent_ssl.h
│   │   │   │   ├── bufferevent_struct.h
│   │   │   │   ├── buffer.h
│   │   │   │   ├── dns_compat.h
│   │   │   │   ├── dns.h
│   │   │   │   ├── dns_struct.h
│   │   │   │   ├── event_compat.h
│   │   │   │   ├── event-config.h
│   │   │   │   ├── event.h
│   │   │   │   ├── event_struct.h
│   │   │   │   ├── http_compat.h
│   │   │   │   ├── http.h
│   │   │   │   ├── http_struct.h
│   │   │   │   ├── keyvalq_struct.h
│   │   │   │   ├── listener.h
│   │   │   │   ├── rpc_compat.h
│   │   │   │   ├── rpc.h
│   │   │   │   ├── rpc_struct.h
│   │   │   │   ├── tag_compat.h
│   │   │   │   ├── tag.h
│   │   │   │   ├── thread.h
│   │   │   │   └── util.h
│   │   │   ├── Makefile
│   │   │   ├── Makefile.am
│   │   │   └── Makefile.in
│   │   ├── install-sh
│   │   ├── iocp-internal.h
│   │   ├── ipv6-internal.h
│   │   ├── kqueue.c
│   │   ├── libevent_core.la
│   │   ├── libevent_extra.la
│   │   ├── libevent.la
│   │   ├── libevent_openssl.la
│   │   ├── libevent_openssl.pc
│   │   ├── libevent_openssl.pc.in
│   │   ├── libevent.pc
│   │   ├── libevent.pc.in
│   │   ├── libevent_pthreads.la
│   │   ├── libevent_pthreads.pc
│   │   ├── libevent_pthreads.pc.in
│   │   ├── libtool
│   │   ├── LICENSE
│   │   ├── listener.c
│   │   ├── listener.lo
│   │   ├── listener.o
│   │   ├── log.c
│   │   ├── log-internal.h
│   │   ├── log.lo
│   │   ├── log.o
│   │   ├── ltmain.sh
│   │   ├── m4
│   │   │   ├── acx_pthread.m4
│   │   │   ├── libtool.m4
│   │   │   ├── lt~obsolete.m4
│   │   │   ├── ltoptions.m4
│   │   │   ├── ltsugar.m4
│   │   │   └── ltversion.m4
│   │   ├── make-event-config.sed
│   │   ├── Makefile
│   │   ├── Makefile.am
│   │   ├── Makefile.in
│   │   ├── Makefile.nmake
│   │   ├── minheap-internal.h
│   │   ├── missing
│   │   ├── mm-internal.h
│   │   ├── poll.c
│   │   ├── poll.lo
│   │   ├── poll.o
│   │   ├── ratelim-internal.h
│   │   ├── README
│   │   ├── sample
│   │   │   ├── dns-example
│   │   │   ├── dns-example.c
│   │   │   ├── dns-example.o
│   │   │   ├── event-test
│   │   │   ├── event-test.c
│   │   │   ├── event-test.o
│   │   │   ├── hello-world
│   │   │   ├── hello-world.c
│   │   │   ├── hello-world.o
│   │   │   ├── http-server
│   │   │   ├── http-server.c
│   │   │   ├── http-server.o
│   │   │   ├── le-proxy
│   │   │   ├── le-proxy.c
│   │   │   ├── le-proxy.o
│   │   │   ├── Makefile
│   │   │   ├── Makefile.am
│   │   │   ├── Makefile.in
│   │   │   ├── signal-test
│   │   │   ├── signal-test.c
│   │   │   ├── signal-test.o
│   │   │   ├── time-test
│   │   │   ├── time-test.c
│   │   │   └── time-test.o
│   │   ├── select.c
│   │   ├── select.lo
│   │   ├── select.o
│   │   ├── signal.c
│   │   ├── signal.lo
│   │   ├── signal.o
│   │   ├── stamp-h1
│   │   ├── strlcpy.c
│   │   ├── strlcpy-internal.h
│   │   ├── strlcpy.lo
│   │   ├── strlcpy.o
│   │   ├── test
│   │   │   ├── bench
│   │   │   ├── bench.c
│   │   │   ├── bench_cascade
│   │   │   ├── bench_cascade.c
│   │   │   ├── bench_cascade.o
│   │   │   ├── bench_http
│   │   │   ├── bench_http.c
│   │   │   ├── bench_httpclient
│   │   │   ├── bench_httpclient.c
│   │   │   ├── bench_httpclient.o
│   │   │   ├── bench_http.o
│   │   │   ├── bench.o
│   │   │   ├── Makefile
│   │   │   ├── Makefile.am
│   │   │   ├── Makefile.in
│   │   │   ├── Makefile.nmake
│   │   │   ├── regress
│   │   │   ├── regress_buffer.c
│   │   │   ├── regress_bufferevent.c
│   │   │   ├── regress.c
│   │   │   ├── regress_dns.c
│   │   │   ├── regress_et.c
│   │   │   ├── regress.gen.c
│   │   │   ├── regress.gen.h
│   │   │   ├── regress.h
│   │   │   ├── regress_http.c
│   │   │   ├── regress_iocp.c
│   │   │   ├── regress_listener.c
│   │   │   ├── regress_main.c
│   │   │   ├── regress_minheap.c
│   │   │   ├── regress-regress_bufferevent.o
│   │   │   ├── regress-regress_buffer.o
│   │   │   ├── regress-regress_dns.o
│   │   │   ├── regress-regress_et.o
│   │   │   ├── regress-regress.gen.o
│   │   │   ├── regress-regress_http.o
│   │   │   ├── regress-regress_listener.o
│   │   │   ├── regress-regress_main.o
│   │   │   ├── regress-regress_minheap.o
│   │   │   ├── regress-regress.o
│   │   │   ├── regress-regress_rpc.o
│   │   │   ├── regress-regress_ssl.o
│   │   │   ├── regress-regress_testutils.o
│   │   │   ├── regress-regress_thread.o
│   │   │   ├── regress-regress_util.o
│   │   │   ├── regress-regress_zlib.o
│   │   │   ├── regress.rpc
│   │   │   ├── regress_rpc.c
│   │   │   ├── regress_ssl.c
│   │   │   ├── regress_testutils.c
│   │   │   ├── regress_testutils.h
│   │   │   ├── regress_thread.c
│   │   │   ├── regress-tinytest.o
│   │   │   ├── regress_util.c
│   │   │   ├── regress_zlib.c
│   │   │   ├── rpcgen-attempted
│   │   │   ├── rpcgen_wrapper.sh
│   │   │   ├── test-changelist
│   │   │   ├── test-changelist.c
│   │   │   ├── test-changelist.o
│   │   │   ├── test-eof
│   │   │   ├── test-eof.c
│   │   │   ├── test-eof.o
│   │   │   ├── test-init
│   │   │   ├── test-init.c
│   │   │   ├── test-init.o
│   │   │   ├── test-ratelim
│   │   │   ├── test-ratelim.c
│   │   │   ├── test-ratelim.o
│   │   │   ├── test.sh
│   │   │   ├── test-time
│   │   │   ├── test-time.c
│   │   │   ├── test-time.o
│   │   │   ├── test-weof
│   │   │   ├── test-weof.c
│   │   │   ├── test-weof.o
│   │   │   ├── tinytest.c
│   │   │   ├── tinytest.h
│   │   │   ├── tinytest_local.h
│   │   │   └── tinytest_macros.h
│   │   ├── util-internal.h
│   │   ├── version.txt
│   │   ├── whatsnew-2.0.txt
│   │   ├── WIN32-Code
│   │   │   ├── event2
│   │   │   │   └── event-config.h
│   │   │   └── tree.h
│   │   └── win32select.c
│   └── tinyxml
│       ├── changes.txt
│       ├── docs
│       │   ├── annotated.html
│       │   ├── classes.html
│       │   ├── classTiXmlAttribute.html
│       │   ├── classTiXmlAttribute-members.html
│       │   ├── classTiXmlAttribute.png
│       │   ├── classTiXmlBase.html
│       │   ├── classTiXmlBase-members.html
│       │   ├── classTiXmlBase.png
│       │   ├── classTiXmlComment.html
│       │   ├── classTiXmlComment-members.html
│       │   ├── classTiXmlComment.png
│       │   ├── classTiXmlDeclaration.html
│       │   ├── classTiXmlDeclaration-members.html
│       │   ├── classTiXmlDeclaration.png
│       │   ├── classTiXmlDocument.html
│       │   ├── classTiXmlDocument-members.html
│       │   ├── classTiXmlDocument.png
│       │   ├── classTiXmlElement.html
│       │   ├── classTiXmlElement-members.html
│       │   ├── classTiXmlElement.png
│       │   ├── classTiXmlHandle.html
│       │   ├── classTiXmlHandle-members.html
│       │   ├── classTiXmlNode.html
│       │   ├── classTiXmlNode-members.html
│       │   ├── classTiXmlNode.png
│       │   ├── classTiXmlPrinter.html
│       │   ├── classTiXmlPrinter-members.html
│       │   ├── classTiXmlPrinter.png
│       │   ├── classTiXmlText.html
│       │   ├── classTiXmlText-members.html
│       │   ├── classTiXmlText.png
│       │   ├── classTiXmlUnknown.html
│       │   ├── classTiXmlUnknown-members.html
│       │   ├── classTiXmlUnknown.png
│       │   ├── classTiXmlVisitor.html
│       │   ├── classTiXmlVisitor-members.html
│       │   ├── classTiXmlVisitor.png
│       │   ├── deprecated.html
│       │   ├── doxygen.css
│       │   ├── doxygen.png
│       │   ├── files.html
│       │   ├── functions_0x63.html
│       │   ├── functions_0x64.html
│       │   ├── functions_0x65.html
│       │   ├── functions_0x66.html
│       │   ├── functions_0x67.html
│       │   ├── functions_0x69.html
│       │   ├── functions_0x6c.html
│       │   ├── functions_0x6e.html
│       │   ├── functions_0x6f.html
│       │   ├── functions_0x70.html
│       │   ├── functions_0x71.html
│       │   ├── functions_0x72.html
│       │   ├── functions_0x73.html
│       │   ├── functions_0x74.html
│       │   ├── functions_0x75.html
│       │   ├── functions_0x76.html
│       │   ├── functions_enum.html
│       │   ├── functions_func_0x63.html
│       │   ├── functions_func_0x64.html
│       │   ├── functions_func_0x65.html
│       │   ├── functions_func_0x66.html
│       │   ├── functions_func_0x67.html
│       │   ├── functions_func_0x69.html
│       │   ├── functions_func_0x6c.html
│       │   ├── functions_func_0x6e.html
│       │   ├── functions_func_0x70.html
│       │   ├── functions_func_0x71.html
│       │   ├── functions_func_0x72.html
│       │   ├── functions_func_0x73.html
│       │   ├── functions_func_0x74.html
│       │   ├── functions_func_0x75.html
│       │   ├── functions_func_0x76.html
│       │   ├── functions_func.html
│       │   ├── functions.html
│       │   ├── functions_rela.html
│       │   ├── functions_vars.html
│       │   ├── hierarchy.html
│       │   ├── index.html
│       │   ├── pages.html
│       │   ├── tab_b.gif
│       │   ├── tab_l.gif
│       │   ├── tab_r.gif
│       │   ├── tabs.css
│       │   ├── tinystr_8h_source.html
│       │   ├── tinyxml_8h_source.html
│       │   └── tutorial0.html
│       ├── Makefile
│       ├── readme.txt
│       ├── tinystr.cpp
│       ├── tinystr.h
│       ├── tinystr.o
│       ├── tinyxml.cpp
│       ├── tinyxmlerror.cpp
│       ├── tinyxmlerror.o
│       ├── tinyxml.h
│       ├── tinyxml_lib.vcxproj
│       ├── tinyxml.o
│       ├── tinyxmlparser.cpp
│       ├── tinyxmlparser.o
│       ├── tinyxml.sln
│       ├── tinyxmlSTL.vcxproj
│       ├── tinyXmlTestSTL.vcxproj
│       ├── tinyXmlTest.vcxproj
│       ├── utf8test.gif
│       ├── utf8testverify.xml
│       ├── utf8test.xml
│       └── xmltest.cpp
├── doc
│   ├── Linux\344\270\213Socket\347\274\226\347\250\213\347\232\204\347\253\257\345\217\243\351\227\256\351\242\230(Address+already+in+use).pdf
│   ├── Linux\351\235\231\346\200\201\345\272\223\344\270\216\345\212\250\346\200\201\345\272\223\347\232\204\345\210\233\345\273\272\344\270\216\344\275\277\347\224\250-\345\244\207\345\277\230
│   ├── Makefile\344\270\255\347\224\250\345\256\217\345\256\232\344\271\211\350\277\233\350\241\214\346\235\241\344\273\266\347\274\226\350\257\221.txt
│   ├── makefile\347\274\226\350\257\221\351\235\231\346\200\201\345\272\223\345\222\214\345\212\250\346\200\201\345\272\223
│   └── \345\274\202\345\270\270\346\215\225\350\216\267\350\257\264\346\230\216.txt
├── lib
│   ├── ExpLib_d.lib
│   ├── libeasy.a
│   ├── libevent_core.lib
│   ├── libevent_extras.lib
│   ├── libevent.lib
│   └── libtinyxml.a
├── README.md
├── src
│   ├── base
│   │   ├── easy_activation_queue.h
│   │   ├── easy_addr.h
│   │   ├── easy_algorithm.h
│   │   ├── easy_allocator.h
│   │   ├── easy_assert.h
│   │   ├── easy_assist.h
│   │   ├── easy_atomic_op.cc
│   │   ├── easy_atomic_op.h
│   │   ├── easy_atomic_op_t.h
│   │   ├── easy_auto_ptr.h
│   │   ├── easy_base_type.h
│   │   ├── easy_big_int.cc
│   │   ├── easy_big_int.h
│   │   ├── easy_byte_buffer.h
│   │   ├── easy_byte_convertrt.h
│   │   ├── easy_char_traits.h
│   │   ├── easy_console.h
│   │   ├── easy_constructor.h
│   │   ├── easy_copy_disabled.h
│   │   ├── easy_counted_auto_ptr.h
│   │   ├── easy_dump.h
│   │   ├── easy_exception.cc
│   │   ├── easy_exception.h
│   │   ├── easy_guard.h
│   │   ├── easy_inet_addr.h
│   │   ├── easy_iterator_base.h
│   │   ├── easy_iterator.h
│   │   ├── easy_linked_list.h
│   │   ├── easy_list.h
│   │   ├── easy_locked_queue.h
│   │   ├── easy_lock.h
│   │   ├── easy_log.cc
│   │   ├── easy_log.h
│   │   ├── easy_macro.h
│   │   ├── easy_memory.h
│   │   ├── easy_message_queue_base.cpp
│   │   ├── easy_message_queue_base.h
│   │   ├── easy_message_queue.cpp
│   │   ├── easy_message_queue.h
│   │   ├── easy_mutex.h
│   │   ├── easy_new_handle_support.h
│   │   ├── easy_obj_pool.h
│   │   ├── easy_os_socket.h
│   │   ├── easy_priority_queue.h
│   │   ├── easy_ref_count_big_int.h
│   │   ├── easy_ref_count_object.h
│   │   ├── easy_ref_count_ptr.h
│   │   ├── easy_reference.h
│   │   ├── easy_ref_manager.h
│   │   ├── easy_ring_buffer.h
│   │   ├── easy_singleton.h
│   │   ├── easy_stdandard_allocator.h
│   │   ├── easy_threading.cc
│   │   ├── easy_threading.h
│   │   ├── easy_thread_mutex.h
│   │   ├── easy_time.h
│   │   ├── easy_timer.h
│   │   ├── easy_type_traits.h
│   │   ├── easy_uninitialized.h
│   │   ├── easy_util.cc
│   │   ├── easy_util.h
│   │   ├── easy_wchar.h
│   │   ├── myadd.cc
│   │   ├── myadd.h
│   │   ├── mysub.cc
│   │   └── mysub.h
│   ├── database
│   │   ├── easy_data_base_worker_pool.h
│   │   ├── easy_field.cc
│   │   ├── easy_field.h
│   │   ├── easy_mysql_connection.cc
│   │   ├── easy_mysql_connection.h
│   │   ├── easy_prepared_statement.cc
│   │   ├── easy_prepared_statement.h
│   │   ├── easy_query_result.cc
│   │   ├── easy_query_result.h
│   │   └── easy_sql_operation.h
│   ├── main
│   │   ├── easy.cc
│   │   └── easy_main.cc
│   └── test
│       ├── ace
│       │   ├── atomic_op_test.cc
│       │   ├── echo
│       │   │   └── echo_c
│       │   │       ├── echo_c
│       │   │       │   ├── echo_c.cpp
│       │   │       │   ├── echo_c.vcxproj
│       │   │       │   ├── echo_c.vcxproj.filters
│       │   │       │   ├── ReadMe.txt
│       │   │       │   ├── stdafx.cpp
│       │   │       │   ├── stdafx.h
│       │   │       │   └── targetver.h
│       │   │       ├── echo_c.sln
│       │   │       └── echo_c.suo
│       │   ├── echo_c.cc
│       │   ├── echo_s.cc
│       │   ├── inet_addr_test.cc
│       │   ├── message_block_test.cc
│       │   ├── process_test.cc
│       │   ├── share_memory_non_of_business_position.cc
│       │   ├── share_memory_persist_test.cc
│       │   └── task_test.cc
│       ├── c++11
│       │   └── c++11_test.cpp
│       ├── c++npv1
│       │   ├── ch3.cc
│       │   ├── iterator_logging_server
│       │   │   ├── iterator_logging_client
│       │   │   │   ├── iterator_logging_client.cpp
│       │   │   │   ├── iterator_logging_client.vcxproj
│       │   │   │   ├── iterator_logging_client.vcxproj.filters
│       │   │   │   ├── ReadMe.txt
│       │   │   │   ├── stdafx.cpp
│       │   │   │   ├── stdafx.h
│       │   │   │   └── targetver.h
│       │   │   └── iterator_logging_server
│       │   │       ├── iterator_logging_server.cpp
│       │   │       ├── iterator_logging_server.h
│       │   │       ├── iterator_logging_server.vcxproj
│       │   │       ├── iterator_logging_server.vcxproj.filters
│       │   │       ├── logging_handle.cpp
│       │   │       ├── logging_handle.h
│       │   │       ├── logging_server.cpp
│       │   │       ├── logging_server.h
│       │   │       ├── ReadMe.txt
│       │   │       ├── stdafx.cpp
│       │   │       ├── stdafx.h
│       │   │       └── targetver.h
│       │   ├── reactive_logging_server
│       │   │   └── reactive_logging_server
│       │   │       ├── reactive_logging_server.cpp
│       │   │       ├── reactive_logging_server.h
│       │   │       ├── reactive_logging_server.vcxproj
│       │   │       ├── reactive_logging_server.vcxproj.filters
│       │   │       ├── ReadMe.txt
│       │   │       ├── stdafx.cpp
│       │   │       ├── stdafx.h
│       │   │       └── targetver.h
│       │   ├── reactive_logging_server_ex
│       │   │   └── reactive_logging_server_ex
│       │   │       ├── reactive_logging_server_ex.cpp
│       │   │       ├── reactive_logging_server_ex.h
│       │   │       ├── reactive_logging_server_ex.vcxproj
│       │   │       ├── reactive_logging_server_ex.vcxproj.filters
│       │   │       ├── ReadMe.txt
│       │   │       ├── stdafx.cpp
│       │   │       ├── stdafx.h
│       │   │       └── targetver.h
│       │   └── thread_per_connect_logging_server
│       │       └── thread_per_connect_logging_server
│       │           ├── ReadMe.txt
│       │           ├── stdafx.cpp
│       │           ├── stdafx.h
│       │           ├── targetver.h
│       │           ├── thread_per_connect_logging_server.cpp
│       │           ├── thread_per_connect_logging_server.h
│       │           ├── thread_per_connect_logging_server.vcxproj
│       │           └── thread_per_connect_logging_server.vcxproj.filters
│       ├── corpus
│       │   ├── corpus_database.h
│       │   ├── corpus_proxy.h
│       │   ├── doc
│       │   │   ├── corpus.sql
│       │   │   ├── corpus.txt
│       │   │   ├── install_corpus.bat
│       │   │   ├── install.docx
│       │   │   ├── question.txt
│       │   │   └── require.txt
│       │   └── test_corpus_proxy.cc
│       ├── cppunit
│       │   ├── easy_cppunit_mini.h
│       │   ├── easy_cppunit_proxy.h
│       │   ├── easy_cppunit_timer.h
│       │   ├── easy_file_reporter.h
│       │   ├── easy_main
│       │   ├── easy_main.cc
│       │   ├── easy_main.o
│       │   ├── gcc.mak
│       │   ├── gmon.out
│       │   └── Makefile
│       ├── create_use_shared-static_lib
│       │   ├── app
│       │   │   ├── Makefile
│       │   │   └── test.c
│       │   ├── doc
│       │   │   └── Linux\351\235\231\346\200\201\345\272\223\344\270\216\345\212\250\346\200\201\345\272\223\347\232\204\345\210\233\345\273\272\344\270\216\344\275\277\347\224\250
│       │   ├── Makefile
│       │   └── mylib
│       │       ├── Makefile
│       │       ├── Makefile-share
│       │       ├── myadd.c
│       │       ├── myadd.h
│       │       ├── mysub.c
│       │       └── mysub.h
│       ├── crt
│       │   ├── easy_test_dynamic_cast.cc
│       │   └── easy_test_memmove.cc
│       ├── easy_test_allocator.cc
│       ├── easy_test_assert.cc
│       ├── easy_test_big_int.cc
│       ├── easy_test_boost_pool.cc
│       ├── easy_test_bug_check.cc
│       ├── easy_test_byte_buffer.cc
│       ├── easy_test_char_traits.cc
│       ├── easy_test_count_auto_ptr.cc
│       ├── easy_test_c_puls_puls_keyword.cc
│       ├── easy_test_database.cc
│       ├── easy_test_dump.cc
│       ├── easy_test_exception.cc
│       ├── easy_test_guard.cc
│       ├── easy_test_inet_addr.cc
│       ├── easy_test_linux_static_lib.cc
│       ├── easy_test_list.cc
│       ├── easy_test_lock.cc
│       ├── easy_test_locked_queue.cc
│       ├── easy_test_log.cc
│       ├── easy_test_memory.cc
│       ├── easy_test_new_handle_support.cc
│       ├── easy_test_obj_pool.cc
│       ├── easy_test_ring_buffer.cc
│       ├── easy_test_running_time_library.cc
│       ├── easy_test_share_memory.cc
│       ├── easy_test_singleton.cc
│       ├── easy_test_stdandard_allocator.cc
│       ├── easy_test_template.cpp
│       ├── easy_test_threading.cc
│       ├── easy_test_timer.cc
│       ├── easy_test_type_traits.cc
│       ├── easy_unit_test_template.cc
│       ├── effective_c
│       │   ├── item42.cc
│       │   ├── item43.cc
│       │   └── item49.cc
│       ├── epoll
│       │   ├── echo_client.cc
│       │   └── echo_server.cc
│       ├── example
│       ├── exception_c
│       │   ├── item1.cc
│       │   ├── item21.cc
│       │   └── item23.cc
│       ├── exception_define.h
│       ├── gcc
│       │   ├── map_test.cc
│       │   └── vector_test.cc
│       ├── gdb
│       │   └── gdb_test.cc
│       ├── language_database
│       │   ├── easy_test_language_database.cc
│       │   └── language_database.sql
│       ├── libevent
│       │   ├── easy_test_libevent.cc
│       │   ├── echo_server
│       │   └── echo_server.cc
│       ├── linux_c_class_shared_lib
│       │   ├── a.cc
│       │   ├── a.h
│       │   ├── doc
│       │   └── test.cc
│       ├── linux_c_class_shared_lib_callback
│       │   ├── a.cc
│       │   ├── a.h
│       │   ├── doc
│       │   ├── i.h
│       │   └── test.cc
│       ├── linux_c_class_static_lib
│       │   ├── a.cc
│       │   ├── a.h
│       │   ├── doc
│       │   └── test.cc
│       ├── linux_generate_use_static_lib
│       │   ├── puls.cc
│       │   ├── puls.h
│       │   ├── puls_lib.cc
│       │   ├── puls_lib.mak
│       │   ├── puls_lib_test.cc
│       │   └── use_puls_lib.mak
│       ├── linux_use_macro
│       │   ├── doc
│       │   └── test.cc
│       ├── Makefile
│       ├── more_effective_c
│       │   ├── item10.cc
│       │   ├── item13.cc
│       │   ├── item14.cc
│       │   ├── item16.cc
│       │   ├── item1.cc
│       │   ├── item22.cc
│       │   ├── item26.cc
│       │   ├── item27.cc
│       │   ├── item28.cc
│       │   ├── item29.cc
│       │   ├── item2.cc
│       │   ├── item30.cc
│       │   ├── item3.cc
│       │   ├── item4.cc
│       │   ├── item6.cc
│       │   ├── item8.cc
│       │   └── item9.cc
│       ├── more_exception_c
│       │   ├── item3.cc
│       │   └── item4.cc
│       ├── object_c
│       │   └── hello_world.m
│       ├── poll
│       │   ├── echo_client.cc
│       │   └── echo_server.cc
│       ├── select
│       │   ├── echo_client.cc
│       │   └── echo_server.cc
│       ├── slim_allocator.h
│       ├── stl
│       │   ├── stl_advance_test.cc
│       │   └── stl_map_test.cc
│       ├── test.cc
│       └── windows_via_c
│           ├── ch17.cc
│           └── ch9.cc
└── tree.log

67 directories, 708 files
	

	
