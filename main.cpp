#include "uthreads.h"
#include <iostream>
#define SECOND 1000000
#include <unistd.h>


void f()
{
    int i = 0;
    while(1){
        ++i;
        printf("in f (%d)\n",i);
        usleep(SECOND);

    }
}

void g()
{
    int i = 0;
    while(1){
        ++i;
        printf("in g (%d)\n",i);
        usleep(SECOND);
    }
}

void h()
{
    int i = 0;
    while(1){
        ++i;
        printf("in h (%d)\n",i);
        usleep(SECOND);
    }
}

int main()
{

    int ret = uthread_init(5* SECOND);
    std::cout << "ret: " << ret << std::endl;

    int tidf = uthread_spawn(f);
    std::cout << "created: " << tidf << std::endl;

    //int tidg = uthread_spawn(g);
    //std::cout << "created: " << tidg << std::endl;

    //int tidh = uthread_spawn(h);
    //std::cout << "created: " << tidh << std::endl;

    std::cout << "quantums so far: " << uthread_get_total_quantums() << std::endl;

    for (;;)
    {

    }

}


