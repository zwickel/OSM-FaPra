CXXFLAGS = -std=c++14 -O2
# CXXFLAGS = -std=c++14 -Wall -g -O2

LIBS = -losmpbf -lprotobuf -lz -lpthread -lboost_thread -lboost_system -lboost_filesystem

LIB = -L ./../lib/osmpbf/build/osmpbf
INC = -I ./include/ -I ./../lib/osmpbf/osmpbf/include -I ./../lib/osmpbf -I ./ -I ../lib/Simple-Web-Server/

default:
	rm -f ../bin/main.out
	g++ $(CXXFLAGS) $(INC) $(LIB) Edge.cpp Node.cpp Graph.cpp GraphReader.cpp WebServer.cpp main.cpp $(LIBS) -o ../bin/main.out
# g++ $(CXXFLAGS) $(INC) $(LIB) GraphReader.cpp main.cpp $(LIBS) -o main.out
