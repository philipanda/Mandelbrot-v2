all: main.o mandelbrot.o
	g++ main.o mandelbrot.o -o mandelbrot -lSDL2 -lSDL2_image -g

main.o: main.cpp
	g++ -c main.cpp -g

mandelbrot.o: mandelbrot.cpp
	g++ -c mandelbrot.cpp -g

clean:
	rm -f *.o mandelbrot
