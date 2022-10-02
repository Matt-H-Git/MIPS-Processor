CXX = g++
CXXFLAGS = -Wall
main: main.o
	$(CXX) main.o -o main $(CXXFLAGS)

main.o: main.cpp pc.h if.h id.h ex.h mem.h wb.h
	$(CXX) -c main.cpp $(CXXFLAGS)
