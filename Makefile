CC=gcc
OBJS=Queue.o Search.o ThreadPool.o
EXEC=Search
DEBUG = -g 
FLAGS= -Wall

$(EXEC) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ -pthread


Queue.o: Queue.c Queue.h
Queue.o: Queue.h
Search.o: Search.c Search.h Queue.h ThreadPool.h
Search.o: Search.h Queue.h ThreadPool.h
ThreadPool.o: ThreadPool.c ThreadPool.h Queue.h
ThreadPool.o: ThreadPool.h Queue.h






clean: 
	rm -f $(EXEC) $(OBJS)
	
