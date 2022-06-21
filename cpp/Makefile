all: ThreadMsg_Test

ThreadMsg_Test: test/main.cpp src/OsEvent.cpp src/MsgBox.cpp
	g++ $^ -o $@ -lpthread -I inc -std=c++11

PHONY: clean
clean:
	rm -fr ThreadMsg_Test