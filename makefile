COMPLR = $(shell which clang >/dev/null; echo $$?)
ifeq "$(COMPLR)" "0"
CC=clang
else
CC=gcc
endif

#CC=clang
#CC=gcc

ifeq ($(CC), clang)
    CFLAGS=-O3 -pedantic -Wall -Wextra -Weverything
else
    CFLAGS=-O3 -pedantic -Wall -Wextra -std=c99
endif

demo: demo.o aes256.o
	$(CC) $(CFLAGS) $^ -o $@

aes256.o: aes256.c aes256.h
	$(CC) $(CFLAGS) -c -o $@ $<

demo.o: demo.c
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	@echo cleaning...
	rm -f demo *.o
