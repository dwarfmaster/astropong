OBJS=stdafx.o trait.o musique.o joueur.o terrain.o bonus.o balle.o menu.o partieSolo.o partie.o main.o config.o
CXXFLAGS=-DRCDIR=\"res/\"
# Les deux lignes suivantes à adapter à votre système
CXXFLAGS+= -I/usr/include/sfml1.6/
LDFLAGS=-lsfml1.6-{audio,graphics,system,window,network}
PROG=astropong

all : $(OBJS)
	g++ $(CXXFLAGS)    -o $(PROG)	$^ $(LDFLAGS)

%.o : %.cpp
	g++ $(CXXFLAGS) -c -o $@	$<

clean :
	@touch $(OBJS) $(PROG)
	@rm -v $(OBJS) $(PROG)

rec : clean all

.PHONY: all clean rec


