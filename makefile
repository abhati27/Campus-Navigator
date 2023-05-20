build:
	rm -f program.exe
	g++ -O2 -std=c++11 -Wall main.cpp dist.cpp osm.cpp tinyxml2.cpp -o program.exe

run:
	./program.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./program.exe
