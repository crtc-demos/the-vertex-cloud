
	CC = 		gcc
	STRIP =		strip
	CFLAGS =	-O0 -fomit-frame-pointer -W -Wall -g
	LIBS =		-lGL -lGLU -lglut -lm
	LDFLAGS =	-g

	OBJS =	transform-gl.o convex.o perlin.o

	TARGET = convex

	SRC =	transform-gl.c convex.c perlin.c

.PHONY:	clean

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

cleaner: clean
	rm -f *.d

$(TARGET):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.d:    %.c
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< > $@

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleaner)
include $(OBJS:.o=.d)
endif
endif
