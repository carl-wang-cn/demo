FLAGS = -Wall -g

all: semcreate semunlink semgetvalue semwait sempost get_sys_conf

semcreate: semcreate.c
	gcc $(FLAGS) semcreate.c -o semcreate -pthread

semunlink: semunlink.c
	gcc $(FLAGS) semunlink.c -o semunlink -pthread

semgetvalue: semgetvalue.c
	gcc $(FLAGS) semgetvalue.c -o semgetvalue -pthread

semwait: semwait.c
	gcc $(FLAGS) semwait.c -o semwait -pthread

sempost: sempost.c
	gcc $(FLAGS) sempost.c -o sempost -pthread

get_sys_conf: get_sys_conf.c
	gcc $(FLAGS) get_sys_conf.c -o get_sys_conf

clean:
	rm -f semcreate
	rm -f semunlink
	rm -f semgetvalue
	rm -f semwait
	rm -f sempost
	rm -f get_sys_conf
