
	CC = 		kos-cc
	STRIP =		strip
	CFLAGS =	-O2 -W -Wall
	LIBS =		 -lgl -lkmg -lkosutils -lpng -lz -lm
	LDFLAGS =	
	GENROMFS =      $(KOS_GENROMFS)

	OBJS =	transform-dc.o convex.o perlin.o banner.o timing.o romdisk.o

	TARGET = dcdemo.elf

	SRC =	transform-dc.c convex.c perlin.c banner.c timing.c
	ROMDISK = romdisk.img

.PHONY:	clean

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

cleaner: clean
	rm -f *.d

$(TARGET):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

.PHONY: $(ROMDISK)
$(ROMDISK): imagedir_clean
	$(GENROMFS) -f $@ -d imagedir_clean

.PHONY: imagedir_clean
imagedir_clean:
	rm -rf imagedir_clean
	rsync -Pav --exclude='.svn' imagedir/ imagedir_clean

romdisk.o:      $(ROMDISK)
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

romdisk.d:
	touch $@

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.d:    %.c
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< > $@

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleaner)
include $(OBJS:.o=.d)
endif
endif
