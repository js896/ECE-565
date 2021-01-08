all: rainfall_pt rainfall_seq

rainfall_seq:rainfall_seq.cpp landscape.cpp 	
	g++ -o rainfall_seq -ggdb3 -std=c++11 rainfall_seq.cpp landscape.cpp


rainfall_pt:rainfall_pt.cpp 
	g++ -o rainfall_pt -std=c++11 rainfall_pt.cpp landscape_pt.cpp -lpthread	
	 
clean:
	rm rainfall_seq rainfall_pt
