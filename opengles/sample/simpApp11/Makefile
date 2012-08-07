NAME=	simpApp11.exe
OBJS=	obj\simpApp11.obj

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
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) /OUT:$@ 


obj\simpApp11.obj: simpApp11.c
	$(CC) $(CFLAGS) $(INCLUDES) /Fo$@ simpApp11.c

clean:
	rm $(OBJS) $(NAME) 
	rmdir $(DIRS)




