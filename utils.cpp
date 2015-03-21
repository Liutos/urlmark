/*
 * utils.cpp
 *
 *  Created on: 2015年3月21日
 *      Author: liutos
 */
#include <cstdio>

#include <fcgi_stdio.h>

void resp(const char *content, size_t size)
{
    printf("Content-type: text/html\r\n"
            "Content-length: %d\r\n"
            "\r\n"
            "%s",
            size, content);
}
