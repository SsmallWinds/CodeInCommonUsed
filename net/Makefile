LIB_NAME?= litenet
STATIC_NAME ?= lib$(LIB_NAME).a

all: static_lib

static_lib: ${OBJS}
	gcc -c ae.c anet.c adlist.c sds.c;
	ar -cr $(STATIC_NAME) ae.o anet.o adlist.o sds.o;
clean:
	rm  $(STATIC_NAME) *.o