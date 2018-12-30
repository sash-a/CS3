main: main.o huff.o node.o Driver.o
	g++ -o huffencode main.o HuffmanNode.o HuffmanTree.o Driver.o

main.o: main.cpp
	g++ -c main.cpp -std=c++11

huff.o: HuffmanNode.cpp
	g++ -c HuffmanTree.cpp -std=c++11

node.o: HuffmanNode.cpp
	g++ -c HuffmanNode.cpp -std=c++11

driver.o: Driver.cpp
	g++ -c Driver.cpp -std=c++11

run:
	./huffencode

test: UnitTests.cpp
	g++ -o UnitTests UnitTests.cpp HuffmanNode.cpp HuffmanTree.cpp Driver.cpp

runTest: test
	./UnitTests