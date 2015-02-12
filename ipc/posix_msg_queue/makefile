FLAGS = -g -Wall

all: mq_create mq_unlink mq_send mq_recv get_sys_conf mq_notify01 mq_notify02 mq_notify03 mq_notify04 signal_realtime 
	

mq_create: mq_create.c
	gcc $(FLAGS) mq_create.c -o mq_create -lrt

mq_unlink: mq_unlink.c
	gcc $(FLAGS) mq_unlink.c -o mq_unlink -lrt

mq_send: mq_send.c
	gcc $(FLAGS) mq_send.c -o mq_send -lrt
mq_recv: mq_recv.c
	gcc $(FLAGS) mq_recv.c -o mq_recv -lrt

get_sys_conf: get_sys_conf.c
	gcc $(FLAGS) get_sys_conf.c -o get_sys_conf

mq_notify01: mq_notify01.c
	gcc $(FLAGS) mq_notify01.c -o mq_notify01 -lrt

mq_notify02: mq_notify02.c
	gcc $(FLAGS) mq_notify02.c -o mq_notify02 -lrt

mq_notify03: mq_notify03.c
	gcc $(FLAGS) mq_notify03.c -o mq_notify03 -lrt

mq_notify04: mq_notify04.c
	gcc $(FLAGS) mq_notify04.c -o mq_notify04 -lrt

signal_realtime: signal_realtime.c
	gcc $(FLAGS) signal_realtime.c -o signal_realtime

clean:
	rm -f mq_create mq_unlink mq_send mq_recv get_sys_conf mq_notify01 mq_notify02 mq_notify03 mq_notify04 signal_realtime 

