/*
 * handlers.cpp
 *
 *  Created on: 2015年3月21日
 *      Author: liutos
 */
#include <cstdlib>
#include <cstring>

#include "utils.hpp"

extern "C" {

void handler_query_string(void)
{
    const char *query_string = getenv("QUERY_STRING");
    resp(query_string, strlen(query_string));
}

void handler_request_uri(void)
{
    const char *route = getenv("DOCUMENT_URI");
    resp(route, strlen(route));
}

void handler_request_method(void)
{
    const char *method = getenv("REQUEST_METHOD");
    resp(method, strlen(method));
}

}
