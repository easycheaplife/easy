/*
 * Copyright (c) 2009-2013 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * compiler:
		$g++ -g -o jansson_test jansson_test.cc  -I../../../dep/jansson/src -L../../../dep/jansson/src/.libs -ljansson
 * run
		$export LD_LIBRARY_PATH=../../../dep/jansson/src/.libs
		$./jansson_test
 */
#include <string>
#include <string.h>
#include <vector>
#include <jansson.h>


typedef unsigned int						u_32;
void run_tests() {
    //	test json array
    int __val_int = 5;
    json_t* __ret_int = 0;

    float __val_float = 8.00f;
    json_t* __ret_float = 0;

    std::string __val_str = "hello jansson";
    json_t* __ret_str = 0;

    json_t* __val_float2 = json_real(278.00f);
    json_t* __ret_float2 = 0;

    json_t* arrayJson1 = json_array();

    json_array_append_new(arrayJson1, json_integer(__val_int));
    __ret_int = json_array_get(arrayJson1,0);

    json_array_append_new(arrayJson1, json_real(__val_float));
    __ret_float = json_array_get(arrayJson1,1);

    json_array_append_new(arrayJson1, json_string(__val_str.c_str()));
    __ret_str = json_array_get(arrayJson1,2);

    int __size = json_array_size(arrayJson1);

    json_array_insert(arrayJson1,0,__val_float2);
    __ret_float2 = json_array_get(arrayJson1,0);

    __size = json_array_size(arrayJson1);


    float __ret = json_real_value(__ret_float2);

    json_decref(__ret_int);
    json_decref(__ret_float);
    json_decref(__ret_str);

    std::vector<int>				m_vIntData;
    std::vector<std::string>		m_vStringData;

    std::vector<int>				m_vIntDataFlag;
    std::vector<int>				m_vStringDataFlag;


    m_vIntData.clear();
    m_vIntDataFlag.clear();
    for(int i = 0; i < 3; ++i) {
        m_vIntData.push_back(i+12);
        m_vIntDataFlag.push_back(1);
    }

    for(int i = 0; i < 3; ++i) {
        m_vStringData.push_back("hello");
        m_vStringDataFlag.push_back(1);
    }

    json_t* arrayJson = json_array();
    json_t* arrayJsonInt = json_array();


    for (u_32 i = 0; i < m_vIntDataFlag.size(); ++i) {
        json_t* elementIntJson = json_array();
        if(0 != m_vIntDataFlag[i]) {
            json_array_append_new(elementIntJson, json_integer(i));
            json_array_append_new(elementIntJson, json_integer(m_vIntData[i]));
        }
        json_array_append_new(arrayJsonInt, elementIntJson);
    }

    json_array_append_new(arrayJson, arrayJsonInt);

    json_t* arrayJsonStr = json_array();
    for (u_32 i = 0; i < m_vStringDataFlag.size(); ++i) {
        json_t* elementStrJson = json_array();
        if(0 != m_vStringDataFlag[i]) {
            json_array_append_new(elementStrJson, json_integer(i));
            json_array_append_new(elementStrJson, json_string(m_vStringData[i].c_str()));
        }
        json_array_append_new(arrayJsonStr, elementStrJson);
    }
    json_array_append_new(arrayJson, arrayJsonStr);

    //	dump json to file

    char __str[1024];
    memset(__str,0,1024);
#if 0
    snprintf((char*)__str, 1024, "%s", json_dumps(arrayJson, 0));
    FILE* __file = fopen("array.json","w+");
    if (__file) {
        fwrite(__str,sizeof(char),strlen(__str),__file);
    }
    fclose(__file);
#else
    int __dump_res = json_dump_file(arrayJson,"array.json",0);
#endif

    //	load json file
    json_error_t* __json_error = NULL;
    json_t* __json_load = json_load_file("array.json",1,__json_error);
    if (__json_load) {
        memset(__str,0,1024);
        snprintf((char*)__str, 1024, "%s", json_dumps(__json_load, 0));
    }
}

int main() {
    run_tests();
    return 0;
}
