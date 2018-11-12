d1:
	vim echo_selectserv.cpp
d2:
	vim echo_client.cpp

run1:
	./server 9222
run2:
	./client 127.0.0.1 9222

clean:
	rm server client

all:
	g++ -o client echo_client.cpp
	g++ -o server echo_selectserv.cpp

