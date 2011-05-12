all:run

run: main.o NetUnit/NetUnit.o NetUnit/ReceiveThread.o
	g++ main.o NetUnit/NetUnit.o NetUnit/ReceiveThread.o -lboost_thread -o Debug/run

ReceiveThread.o: NetUnit/ReceiveThread.cpp NetUnit/ReceiveThread.h
	g++ -c ReceiveThread.cpp

NetUnit.o: NetUnit/NetUnit.cpp NetUnit/NetUnit.h NetUnit/ReceiveThread.h
	g++ -c NetUnit.cpp -lboost_thread

main.o: main.cpp NetUnit/NetUnit.h
	g++ -c main.cpp

clean:
	rm -rf main.o NetUnit/ReceiveThread.o NetUnit/NetUnit.o
