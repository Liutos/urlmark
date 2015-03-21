CFLAGS = -Wall -g -fPIC -O0

FCGI_DIR = $(HOME)/local/fcgi
FCGI_INC = -I$(FCGI_DIR)
FCGI_LD = -L$(FCGI_DIR)/lib -lfcgi

# object files

cJSON.o: cJSON.c
	gcc $(CFLAGS) -c -o $@ $<

handlers.o: handlers.cpp
	g++ $(CFLAGS) -c -o $@ $< $(FCGI_INC)

main.o: main.cpp
	g++ $(CFLAGS) -c -o $@ $< $(FCGI_INC)

utils.o: utils.cpp
	g++ $(CFLAGS) -c -o $@ $< $(FCGI_INC)

# shared object files

libhandlers.so: handlers.o utils.o
	g++ -shared -o $@ $^

# executables

main: cJSON.o main.o utils.o
	g++ -Wall -g -O0 -o $@ $^ $(FCGI_LD) -ldl