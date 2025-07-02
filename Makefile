CC=g++
target = VirtualKeyboard

RESDIR:=res

SRCS=$(wildcard ./src/*.cpp)
OBJS=$(patsubst %cpp,%o,$(SRCS))

#INCLUDE = -I/usr/include/SDL2
INCLUDE =  $(shell sdl2-config --cflags)
#LIB = -L/usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf 
#LIB = -lSDL2 -lSDL2_image -lSDL2_ttf 
LIB = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer

all:$(OBJS)
	$(CC) $(OBJS) -o $(target) $(LIB)

%.o:%.cpp
	$(CC) -std=c++11 -DRESDIR="\"$(RESDIR)\"" -c $< -o $@  $(INCLUDE) 

clean:
	rm -f $(OBJS) $(target) 

