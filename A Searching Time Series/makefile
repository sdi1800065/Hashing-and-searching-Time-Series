all: main unit_tests

main:main.o FileHandling.o functionalities.o search.o hash.o curves.o
	g++ --std=c++11 main.o FileHandling.o functionalities.o search.o hash.o curves.o -o search
unit_tests: unit_tests.o hash.o FileHandling.o functionalities.o search.o curves.o
	g++ --std=c++11 -O2 hash.o unit_tests.o FileHandling.o functionalities.o search.o curves.o -lcppunit -o unit_tests
unit_tests.o :unit_tests.cpp
	g++ --std=c++11 -O2 -c unit_tests.cpp
main.o :main.cpp hash.hpp FileHandling.hpp search.hpp functionalities.hpp curves.hpp
	g++ --std=c++11 -c main.cpp
hash.o :hash.cpp hash.hpp FileHandling.hpp search.hpp functionalities.hpp curves.hpp
	g++ --std=c++11 -c hash.cpp
search.o :search.cpp hash.hpp FileHandling.hpp search.hpp functionalities.hpp curves.hpp
	g++ --std=c++11 -c search.cpp
FileHandling.o :FileHandling.cpp hash.hpp FileHandling.hpp  search.hpp functionalities.hpp curves.hpp
	g++ --std=c++11 -c FileHandling.cpp
functionalities.o :functionalities.cpp hash.hpp FileHandling.hpp search.hpp functionalities.hpp curves.hpp
	g++ --std=c++11 -c functionalities.cpp
curves.o :curves.cpp hash.hpp FileHandling.hpp search.hpp functionalities.hpp curves.hpp
	g++ -O2 -c curves.cpp

clean:
	rm search unit_tests main.o hash.o FileHandling.o functionalities.o search.o curves.o unit_tests.o
run_tests:
	./unit_tests