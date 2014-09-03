all: semcreate semunlink semgetvalue semwait sempost

semcreate: semcreate.c
	gcc semcreate.c -o semcreate -pthread

semunlink: semunlink.c
	gcc semunlink.c -o semunlink -pthread

semgetvalue: semgetvalue.c
	gcc semgetvalue.c -o semgetvalue -pthread

semwait: semwait.c
	gcc semwait.c -o semwait -pthread

sempost: sempost.c
	gcc sempost.c -o sempost -pthread

clean:
	rm semcreate -f
	rm semunlink -f
	rm semgetvalue -f
	rm semwait -f
	rm sempost -f
