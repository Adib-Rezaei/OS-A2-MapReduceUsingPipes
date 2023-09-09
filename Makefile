all:cl
	g++ -std=c++17 main.cpp -lstdc++fs
	g++ -std=c++17 map.cpp -o map.out
	g++ -std=c++17 reduce.cpp -o reduce.out

cl:
	rm -f fifo* *.out
