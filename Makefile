CXX        = g++ #wenn
CFLAGS    = -I. -Wall -std=c++11
TARGET    = space_invaders
LIBS      = -lncurses
SOURCE    = main.cpp
HEADER    = enemy.hpp bullet.hpp player.hpp



$(TARGET) : main.o
	$(CXX) $(CFLAGS) -o $(TARGET) main.o $(LIBS)


main.o : $(SOURCE) $(HEADER)
	$(CXX) $(CFLAGS) -c main.cpp $(LIBS)

clean :
	rm -f main.o 
