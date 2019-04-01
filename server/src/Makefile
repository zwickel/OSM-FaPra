CXXFLAGS = -std=c++14 -O3
# CXXFLAGS = -std=c++14 -Wall -g -O3

LIBS = -losmpbf -lprotobuf -lz -lpthread -lboost_thread -lboost_system -lboost_filesystem

# with GeogrpahicLib
# INC = -I./include/ -I./../lib/osmpbf/osmpbf/include -I./../lib/osmpbf -I./ -I../lib/Simple-Web-Server/ -I./../lib/GeographicLib-1.49/include/ -I./../lib/GeographicLib-1.49
# LIB = -L./../lib/osmpbf/build/osmpbf -L./../lib/GeographicLib-1.49/build/include/

#without
INC = -I./include/ -I./../lib/osmpbf/osmpbf/include -I./../lib/osmpbf -I./ -I../lib/Simple-Web-Server/
LIB = -L./../lib/osmpbf/build/osmpbf

default:
	rm -f ../bin/main.out
	g++ $(CXXFLAGS) $(INC) $(LIB) Edge.cpp Node.cpp Graph.cpp GraphReader.cpp WebServer.cpp main.cpp $(LIBS) -o ../bin/main.out
# g++ $(CXXFLAGS) $(INC) $(LIB) GraphReader.cpp main.cpp $(LIBS) -o main.out
