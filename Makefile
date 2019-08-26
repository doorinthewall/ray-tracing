#CC = g++
#CFLAGS = -std=c++11 -O3 -fopenmp

#all: stage

#stage: primitive.o raything.o io.o EasyBMP.o
#	$(CC) primitive.o raything.o io.o EasyBMP.o stage.o -o res

#primitive.o: primitive.cpp
#	$(CC) $(CFLAGS) primitive.cpp

#raything.o: raything.cpp
#	$(CC) $(CFLAGS) raything.cpp

#io.o: io.cpp
#	$(CC) $(CFLAGS) io.cpp

#EasyBMP.o: EasyBMP.cpp
#	$(CC) $(CFLAGS) EasyBMP.cpp

#stage.o: stage.cpp
#		$(CC) $(CFLAGS) stage.cpp

all: stage.cpp primitive.cpp raything.cpp io.cpp EasyBMP.cpp stage.cpp
	g++ stage.cpp primitive.cpp raything.cpp EasyBMP.cpp io.cpp -fopenmp -O3 -o a -std=c++11