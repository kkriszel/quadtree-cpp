main : main.o shape_container.o shape_quadtree.o shape.o util.o bound.o
	g++ -Wall -o main main.o shape_container.o shape_quadtree.o shape.o util.o bound.o -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
	
main.o : main.cpp olc/olcPixelGameEngine.h shape.hpp shape_container.hpp lib/bound.hpp lib/util.hpp
	g++ -Wall -Wno-unknown-pragmas -c main.cpp

shape_container.o : shape_container.hpp shape_container.cpp shape.hpp lib/util.hpp lib/bound.hpp lib/quadtree.hpp
	g++ -Wall -c shape_container.cpp

shape_quadtree.o : shape_quadtree.cpp shape.hpp lib/quadtree.hpp lib/quadtree.cpp
	g++ -Wall -c shape_quadtree.cpp

shape.o : shape.hpp shape.cpp lib/util.hpp lib/bound.hpp
	g++ -Wall -c shape.cpp

util.o : lib/util.hpp lib/util.cpp
	g++ -Wall -c lib/util.cpp

bound.o : lib/bound.hpp lib/bound.cpp
	g++ -Wall -c lib/bound.cpp

.PHONY : clean
clean :
	rm -f main main.o shape_container.o shape_quadtree.o shape.o util.o bound.o

