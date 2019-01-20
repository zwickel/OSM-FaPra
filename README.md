# OSM-FaPra

## How-To build

### Dependencies

#### for osmpbf

* protobuf
* zlib

#### for Simple-Web-Server

* Boost.Asio or standalone Asio

### Unzip

Unzip the osm-fapra-ma.zip archive in a folder of your choice.

### Get dependent repos

#### osmpbf

```sh
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

### Building the actual project

```sh
$ cd ../../src
$ make
```

## Start

`$ ./main.out /path/to/germany.osm.pbf`

After the graph has been read and the web server starts your console should output:  
`starting server at http://localhost:8091`

You can access the server with your browser at `http://localhost:8091`
