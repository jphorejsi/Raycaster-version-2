target: raytracer1b.o
	g++ raytracer1b.o -o target

raycast.o: raytracer1b.cc
	g++ -c raytracer1b.cc

clean:
	rm *.o target