all: cluster unit_tests
cluster: cluster.o clusterring_functions.o functionalities.o hypercube_func.o lsh.o silhouette.o BTree.o CBTree.o
	g++ --std=c++11 -O2 cluster.o clusterring_functions.o functionalities.o hypercube_func.o lsh.o silhouette.o BTree.o CBTree.o -o cluster
unit_tests: unit_tests.o lsh.o functionalities.o clusterring_functions.o BTree.o CBTree.o
	g++ --std=c++11 -O2 unit_tests.o lsh.o functionalities.o clusterring_functions.o hypercube_func.o BTree.o CBTree.o -lcppunit -o unit_tests
unit_tests.o :unit_tests.cpp
	g++ --std=c++11 -O2 -c unit_tests.cpp
BTree.o: BTree.cpp
	g++ --std=c++11 -O2 -c BTree.cpp
CBTree.o: CBTree.cpp
	g++ --std=c++11 -O2 -c CBTree.cpp
cluster.o :cluster.cpp
	g++ --std=c++11 -O2 -c cluster.cpp
clusterring_functions.o :clusterring_functions.cpp
	g++ --std=c++11 -O2 -c clusterring_functions.cpp
functionalities.o :functionalities.cpp
	g++ --std=c++11 -O2 -c functionalities.cpp
hypercube_func.o :hypercube_func.cpp
	g++ --std=c++11 -O2 -c hypercube_func.cpp
lsh.o :lsh.cpp
	g++ --std=c++11 -O2 -c lsh.cpp
silhouette.o :silhouette.cpp
	g++ --std=c++11 -O2 -c silhouette.cpp

clean:
	rm cluster cluster.o clusterring_functions.o functionalities.o hypercube_func.o lsh.o silhouette.o BTree.o CBTree.o unit_tests
run_tests:
	./unit_tests
