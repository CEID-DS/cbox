all:run

run: main.o NetUnit/NetUnit.o NetUnit/ReceiveThread.o NetUnit/TransmitThread.o GeneralFunctions.o
	g++ main.o NetUnit/NetUnit.o NetUnit/ReceiveThread.o NetUnit/TransmitThread.o GeneralFunctions.o -lboost_thread -o Debug/run

GeneralFunctions.o: GeneralFunctions.cpp GeneralFunctions.h
	g++ -c GeneralFunctions.cpp

TransmitThread.o: NetUnit/TransmitThread.cpp NetUnit/TransmitThread.h
	g++ -c TransmitThread.cpp

ReceiveThread.o: NetUnit/ReceiveThread.cpp NetUnit/ReceiveThread.h
	g++ -c ReceiveThread.cpp

NetUnit.o: NetUnit/NetUnit.cpp NetUnit/NetUnit.h NetUnit/ReceiveThread.h NetUnit/TransmitThread.h
	g++ -c NetUnit.cpp -lboost_thread

main.o: main.cpp NetUnit/NetUnit.h GeneralFunctions.h
	g++ -c main.cpp

clean:
	rm -rf main.o NetUnit/ReceiveThread.o NetUnit/NetUnit.o NetUnit/TransmitThread.o GeneralFunctions.o
