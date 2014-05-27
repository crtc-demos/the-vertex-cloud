
	CC = 		gcc
	STRIP =		strip
	CFLAGS =	-O3 -ffast-math -fomit-frame-pointer -W -Wall -g
	DEPFLAGS =
	LIBS =		-lGL -lGLU -lglut -lm
	LDFLAGS =	-g

	OBJS =	transform-gl.o convex.o perlin.o

	TARGET = convex

	SRC =	transform-gl.c convex.c perlin.c

.PHONY:	.depend

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

.depend:	Makefile $(SRC)
	$(CC) $(DEPFLAGS) $(INCLUDE) -MM $(SRC) > .depend

include .depend
