FLAGS = -Wall -g

all: count01 count02 shmcreate shmunlink shmread shmwrite count03_client count03_server producer consumer

count01: count01.c
	gcc $(FLAGS) count01.c -o count01 -pthread

count02: count02.c
	gcc $(FLAGS) count02.c -o count02 -pthread

shmcreate: shmcreate.c
	gcc $(FLAGS) shmcreate.c -o shmcreate -lrt

shmunlink: shmunlink.c
	gcc $(FLAGS) shmunlink.c -o shmunlink -lrt

shmread: shmread.c
	gcc $(FLAGS) shmread.c -o shmread -lrt

shmwrite: shmwrite.c
	gcc $(FLAGS) shmwrite.c -o shmwrite -lrt

count03_server: count03_server.c
	gcc $(FLAGS) count03_server.c -o count03_server -lrt -pthread

count03_client: count03_client.c
	gcc $(FLAGS) count03_client.c -o count03_client -lrt -pthread

producer: producer.c
	gcc $(FLAGS) producer.c -o producer -lrt -pthread

consumer: consumer.c
	gcc $(FLAGS) consumer.c -o consumer -lrt -pthread

clean:
	rm -f count01
	rm -f count02
	rm -f shmcreate
	rm -f shmunlink
	rm -f shmread
	rm -f shmwrite
	rm -f count03_client
	rm -f count03_server
	rm -f producer
	rm -f consumer


