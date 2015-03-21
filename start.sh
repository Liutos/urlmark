#!/bin/bash

cnt_proc=1

file_sock=`pwd`/urlmark.sock

pg_spawn=${HOME}/local/nginx/sbin/spawn-fcgi
pg_main=`pwd`/main

cmd="${pg_main}"

${pg_spawn} -f "${cmd}" -s ${file_sock} -F ${cnt_proc}
