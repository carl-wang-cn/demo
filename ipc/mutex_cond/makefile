FLAGS = -Wall -g

all: prodcons_mutex prodcons_mutex_cond

prodcons_mutex: prodcons_mutex.c
	gcc $(FLAGS) prodcons_mutex.c -o prodcons_mutex -pthread

prodcons_mutex_cond: prodcons_mutex_cond.c
	gcc $(FLAGS) prodcons_mutex_cond.c -o prodcons_mutex_cond -pthread

clean:
	rm -f prodcons_mutex prodcons_mutex_cond
