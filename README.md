KVV ncurses UI
==============

Lists streetcar departures in Karlsruhe.

Building
--------

`mkdir build && cd build && cmake .. && make`

Usage
-----

`./kvv stopId stopName`

The `stopId` can be obtained by running `curl https://live.kvv.de/webapp/stops/byname/Marktplatz\?key\=377d840e54b59adbe53608ba1aad70e8`.