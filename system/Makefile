# Makefile
all:meme4916.out
CC = gcc
INCLUDE = .
CFLAGS = -O -Wall -ansi -std=gnu99
objects = app/main.o app/erthItity.o app/ws2812bdisplay.o app/fileWRtask.o app/buzzertask.o app/envirtask.o app/garsigtask.o interface/adxl485api.o interface/wbs2812bapi.o interface/pcf8591api.o interface/buzzerapi.o interface/dht11api.o interface/colorledapi.o interface/hchr04api.o
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
