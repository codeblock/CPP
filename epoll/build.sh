#!/bin/bash

echo 'compile myUTIL.cpp ...'
g++ -o ./output/myUTIL.o -c ./src/myUTIL.cpp
echo 'compile DB.cpp ...'
g++ -o ./output/DB.o -c ./src/DB/DB.cpp
echo 'compile MySQL.cpp ...'
g++ -o ./output/MySQL.o -c `mysql_config --cflags` ./src/DB/MySQL/MySQL.cpp

echo 'compile Logger.cpp ...'
g++ -o ./output/Logger.o -c ./src/Logger.cpp -std=gnu++0x -lpthread

echo 'compile Pool.cpp ...'
g++ -o ./output/Pool.o -c `mysql_config --cflags` ./src/Pool.cpp -std=gnu++0x -lpthread

echo 'compile query.cpp ...'
g++ -o ./output/query.o -I ./src/DB/MySQL/mysql/include -I ./src/DB/MySQL/mysql/include/mysql -c ./src/query.cpp -std=gnu++0x
echo 'compile epoll.cpp ...'
g++ -o ./output/epoll.o -I ./src/DB/MySQL/mysql/include -I ./src/DB/MySQL/mysql/include/mysql -c ./src/epoll.cpp -std=gnu++0x

echo 'linking ...'
#g++ -o ./bin/epoll ./output/myUTIL.o ./output/DB.o ./output/MySQL.o ./output/Logger.o ./output/query.o ./output/epoll.o `mysql_config --libs` -std=gnu++0x -pthread
g++ -o ./bin/epoll ./output/myUTIL.o ./output/DB.o ./output/MySQL.o ./output/Logger.o ./output/Pool.o ./output/query.o ./output/epoll.o `mysql_config --libs` -std=gnu++0x -lpthread
echo 'end'
