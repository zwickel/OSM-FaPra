# OSM-FaPra

## Meeting Point Finder

The application is build to find a location where several people starting at different positions should meet.
The meeting location is meant to be a place where each participant needs the same effort to get to.
This effort is represented by costs.
The costs could be changed by using another metric of function to set them.

In its ground form the costs in this project are dependent on the length of the edge.
These are calculated by the pure osm-data (osm.pbf-files) which are used as input in the beginning.

To find the meeting point, from each start position a dijkstra algorithm is executed (it is executed until all the othere start positions are reached).
Then all the nodes that have been visited in each dijkstra are traversed and the appropriate costs from each start position are used to calculate the minimal variance.
The node with the minimal variance is chosen as the meeting point.

```sh
for each (node n that is visited by all dijkstras):
  # sum all differences between the costs of each pair of start positions
  costs.push(sum(abs(x_i - x_j)))

# and select the minimum of them
return min(costs)
```

## Setup

### Dependencies

#### for osmpbf

- protobuf
- zlib

#### for Simple-Web-Server

- Boost.Asio or standalone Asio

### Unzip

Unzip the osm-fapra-ma.zip archive in a folder of your choice.

### Get and install dependent repos

#### osmpbf

```sh
$ mkdir OSM-FaPra/server/lib
$ cd OSM-FaPra/server/lib

$ git clone https://github.com/inphos42/osmpbf
$ cd osmpbf
$ git submodule init
$ git pull --recurse-submodules
$ mkdir build
$ cd build
$ cmake ..
$ make
```

#### Simple-Web-Server

```sh
$ cd ../../ # navigate back to lib-folder

$ git clone https://github.com/eidheim/Simple-Web-Server.git
$ cd Simple-Web-Server
$ mkdir build
$ cd build
$ cmake ..
$ make
```

#### GeographicLib (not needed at the moment - also not used in makefile)

download GeographicLib from <http://geographiclib.sourceforge.net> and unzip in to the lib-folder  
currently version 1.49 is used so

```sh
$ cd ../../ # navigate back to lib-folder
$ tar xfpz GeographicLib-1.49.tar.gz
$ cd GeographicLib-1.49
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Building the actual project

```sh
$ cd ../../../ # navigate back to server-folder
$ mkdir bin
$ cd src
$ make
```

## Start

```sh
$ cd ../bin
$ ./../bin/main.out /path/to/germany.osm.pbf
```

After the graph has been read and the web server starts your console should output:  
`starting server at http://localhost:8091`

You can access the server with your browser at `http://localhost:8091`

## Usage

The start positions are chosen by clicking on the map.
On the right side (in the positions list) the newly selected position is displayed.
It can be deselected/deleted by clicking the x-icon in the upper right corner.

When there are at least 2 positions selected one can click the button in the lower right `FIND MEETING POINT`.
Then the data is sent to the server where the dijkstras, the position to meet and the paths from each start position to the meeting point is calculated.
When it is sent back to the client everything is diplayed on the map.

There is an additional button in the lower right `CLEAR MAP` to clear all the elements on the map.
