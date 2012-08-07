NAME=	main.exe
OBJS=	obj\main.obj

# Set tools and default compilation flags.

CC= cl
LD= link
CFLAGS= /c /D_CRT_SECURE_NO_DEPRECATE
INCLUDES= /I../../../include
LIBS=	../../../lib/libEGL.lib ../../../lib/libGLESv1_CM.lib user32.lib

DIRS=	obj

all: dirs $(NAME)

dirs: $(DIRS)

obj:
	mkdir $@

$(NAME): $(OBJS)
	$(LD) $(LDFLAGS) /OUT:$@ $(OBJS) $(LIBS)


obj\main.obj: main.c
	$(CC) $(CFLAGS) $(INCLUDES) /Fo$@ main.c

clean:
	rm $(OBJS) $(NAME) 
	rmdir $(DIRS)




