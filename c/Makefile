all: ThreadMsg_Test

ThreadMsg_Test: test/main.c src/OsEvent.c src/OsMsg.c
	gcc $^ -o $@ -lpthread -I inc

PHONY: clean
clean:
	rm -fr ThreadMsg_Test