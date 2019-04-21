OBJS  = exec.o utils.o
OBKS  = gen.o utils.o
OOPTS = -Wall -Wextra -g -c -std=c99
LOPTS = -Wall -Wextra -g

all: schedule gen

schedule: $(OBJS)
	gcc $(LOPTS) $(OBJS) -o schedule -lm

gen		: gen.o utils.o
	gcc	$(LOPTS) $(OBKS) -o gen

gen.o	: gen.c utils.h
	gcc $(OOPTS) gen.c

utils.o	: utils.c utils.h
	gcc	$(OOPTS) utils.c

exec.o	: exec.c utils.h
	gcc $(OOPTS) exec.c

generator   : gen
	./gen

scheduler	: schedule
	./schedule

clean	: 
	rm -f *.o schedule gen