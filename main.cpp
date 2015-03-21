/*
 * main.cpp
 *
 *  Created on: 2015年3月21日
 *      Author: liutos
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

#include <dlfcn.h>
#include <fcgi_stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cJSON.h"
#include "utils.hpp"

using namespace std;

typedef void (*handler)(void);

static map<string, handler> routes;

static char *get_path(void)
{
    return getenv("DOCUMENT_URI");
}

/* 返回路径中前缀之后的部分 */
static char *get_route(void)
{
    char *path = get_path();
#define PREFIX "/urlmark"
    return path + strlen(PREFIX);
#undef PREFIX
}

/* 根据路由规则调度不同的处理函数 */
static void dispatch(char *route)
{
    map<string, handler>::iterator it = routes.find(route);
    if (it != routes.end()) {
        handler f = it->second;
        (*f)();
    } else {
        const char *msg = "Not Found";
        resp(msg, strlen(msg));
    }
}

static void route_register(const char *route, const char *name, void *dl)
{
    handler f = (handler)dlsym(dl, name);
    if (f != NULL) {
        string path("/");
        path.append(route);
        routes[path] = f;
    }
}

static int load_config(cJSON **config)
{
    const char *file = "urlmark.json";
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror(file);
        return -1;
    }
    struct stat buf;
    stat(file, &buf);
    char *text = (char *)malloc(buf.st_size * sizeof(char));
    fread(text, sizeof(char), buf.st_size, fp);
    fclose(fp);
    cJSON *json = cJSON_Parse(text);
    free(text);
    if (json == NULL) {
        puts(cJSON_GetErrorPtr());
        return -1;
    }
    *config = json;
    return 0;
}

/* 加载配置文件设置应用的路由规则 */
static int init(void)
{
    /* 加载配置文件 */
    cJSON *config = NULL;
    if (load_config(&config) != 0) {
        return -1;
    }
    /* 加载动态链接库 */
    void *handle = dlopen("libhandlers.so", RTLD_LAZY);
    if (handle == NULL) {
        puts(dlerror());
        return -1;
    }
    /* 遍历JSON对象，注册路由 */
    cJSON *routes = cJSON_GetObjectItem(config, "routes");
    if (routes == NULL) {
        return 0;
    }
    cJSON *child = routes->child;
    while (child != NULL) {
        route_register(child->string, child->valuestring, handle);
        child = child->next;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (init() != 0) {
        return 1;
    }
    while (FCGI_Accept() == 0) {
        char *route = get_route();
        dispatch(route);
    }
    return 0;
}
