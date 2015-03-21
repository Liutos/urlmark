#!/bin/bash

pg_main=main

ps aux | grep ${pg_main} | grep -v 'grep' | grep `pwd` | awk '{ print $2 }' | xargs kill
