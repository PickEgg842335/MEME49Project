# Makefile
all:meme4916.out
CC = gcc
INCLUDE = .
CFLAGS = -O -Wall -ansi -std=gnu99
objects = app/main.o app/erthItity.o app/ws2812bdisplay.o app/sharedMemtask.o interface/adxl485api.o interface/wbs2812bapi.o
MYLIB = mylib.a
LDLIBS += -L./usrLib -lm
meme4916.out: $(MYLIB)
	$(CC) -o meme4916.out $(MYLIB) $(LDLIBS)

$(MYLIB): $(MYLIB)($(objects))

.SUFFIXES:.c
.c .o:
	$(CC) -c -I$(INCLUDE) $(CFLAGS) $<

clean:
	-rm *.out *.a
