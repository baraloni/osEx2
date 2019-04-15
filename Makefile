CC = g++ -std=c++11
CFLAGS = -Wextra -Wall -Wvla -g -I.
TARGET= libuthreads.a
OBJ = uthreads.o scheduler.o thread_manager.o thread.o virtual_timer.o real_timer.o sleeping_threads_list.o
all: libuthreads.a

libuthreads.a: $(OBJ)
	ar rcs $(TARGET) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $(NDB) -c  $< -o $@ 

clean:
	rm -f *.o *.a *.tar *.out

tar:
	tar cvf ex2.tar uthreads.cpp scheduler.cpp thread_manager.cpp thread.cpp virtual_timer.cpp real_timer.cpp sleeping_threads_list.cpp scheduler.h thread_manager.h thread.h virtual_timer.h real_timer.h sleeping_threads_list.h README Makefile


