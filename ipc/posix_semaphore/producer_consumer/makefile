all:producer_consumer01 producer_consumer02

producer_consumer01: producer_consumer01.c
	gcc producer_consumer01.c -o producer_consumer01 -pthread

producer_consumer02: producer_consumer02.c
	gcc producer_consumer02.c -o producer_consumer02 -pthread

clean:
	rm -f producer_consumer01
	rm -f producer_consumer02

