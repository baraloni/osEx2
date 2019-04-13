#include "uthreads.h"
#include <iostream>
#define SECOND 1000000
#include <unistd.h>
#include <cassert>

#define STACK_SIZE 4096
#include "thread_manager.h"
#include "thread.h"

// THE ULTIMATE TEST-----------------------------------------------------------------------------------------------------------------

//void g()
//{
//    //TIME 4:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    assert(uthread_get_total_quantums() == 4);
//    while (1)
//    {
//        //assert(uthread_get_quantums(uthread_get_tid()) == 1);
//    }
//}
//
//void f() {
//
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    assert(uthread_get_total_quantums() == 2);
//    std::cerr << "created: T"<<uthread_spawn(g)<<std::endl;
//    while (uthread_get_total_quantums() != 5)
//    {}
//
//    //TIME 5:
//    std::cerr << "terminating T2" << std::endl;
//    uthread_terminate(2);
//    while (1) {
//        //assert(uthread_get_quantums(uthread_get_tid()) == 2);
//    }
//}
//
//
//
//int main()
//{
//    int ret = uthread_init(1);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    assert(uthread_get_total_quantums() == 1);
//    std::cerr << "created: T"<<uthread_spawn(f)<<std::endl;
//    while (uthread_get_total_quantums() != 3) {}
//
//    //TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    assert(uthread_get_total_quantums()  == 3);
//    while (uthread_get_total_quantums() != 6){}
//
//    //TIME 6:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(1);
//    std::cerr <<"terminated: T1\n";
//    while (uthread_get_total_quantums() != 7) {}
//
//    //TIME 7:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(0);
//    return 0;
//}


// BLOCK TEST----------------------------------------------------------------------------------------------------------------

//void t1(void)
//{
//    //TIME 2/5:
//    while (1){}
//}
//
//int main()
//{
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << " created" << uthread_spawn(t1) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    //TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_block(1);
//    std::cerr << "blocking : T1" << std::endl;
//
//    while (uthread_get_total_quantums() != 4){}
//
//    //TIME 4:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_resume(1);
//    std::cerr << "resuming : T1" << std::endl;
//
//    while (uthread_get_total_quantums() != 6){}
//
//    //TIME 6:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T1" << std::endl;
//
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//
//    return 0;
//
//
//}




//Basic TEST------------------------------------------------------------------------------------------------------------------------

//void f0(void)
//{
//    //TIME 2/4:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    while (1)
//    {
//        std::cerr << "T1...";
//    }
//}
//
//int main()
//{
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_spawn(f0);
//
//  //  uthread_spawn(f1);
//    while (uthread_get_total_quantums() != 3){}
//
//    //TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    while (uthread_get_total_quantums() != 5){}
//
//    //TIME 5:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "terminating T1" << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//
//
//    return 0;
//}

//THREAD TEST-------------------------------------------------------------------------------------------------------



//thread t0 = thread();
//thread t1 = thread();
//
//
//void switchContext(thread& curr, thread& next)
//{
//    int retVal = sigsetjmp(curr._env, 1);
//    if (retVal == 0)
//    {
//        std::cerr << "switch" << std::endl;
//        siglongjmp(next._env, 1);
//    }
//}
//
//void f0(void)
//{
//    int i = 0;
//    while (1){
//        ++i;
//        std::cerr << "T0...";
//        if (i % 5)
//        {
//            switchContext(t0, t1);
//        }
//    }
//}
//
//void f1(void)
//{
//    int i = 0;
//    while (1){
//        std::cerr << "T1...";
//        ++i;
//        if (i % 10)
//        {
//         switchContext(t1, t0);
//        }
//    }
//}
//
//int main() {
//    t0.setupThread(f0, STACK_SIZE);
//    t1.setupThread(f1, STACK_SIZE);
//    switchContext(t1, t0);
//}


//DEMO TEST-----------------------------------------------------------------------------------------------------------------\

//
//thread t0 = thread();
//thread t1 = thread();
//
//
//
//void switchThreads(int currTid)
//
//{
//
//    if (currTid == 0)
//    {
//        int retVal = sigsetjmp(t0._env, 1);
//        if (retVal == 0)
//        {
//            siglongjmp(t1._env, 1);
//        }
//    } else{
//        int retVal = sigsetjmp(t1._env, 1);
//        if (retVal == 0)
//        {
//            siglongjmp(t0._env, 1);
//        }
//    }
//}
//void f(void)
//{
//    int i = 0;
//    while(1){
//        ++i;
//        std::cerr << "in f ("<<i<<")\n";
//        if (i % 3 == 0) {
//            std::cerr <<"f: switching\n";
//            switchThreads(0);
//        }
//        usleep(SECOND);
//    }
//}
//
//void g(void)
//{
//    int i = 0;
//    while(1){
//        ++i;
//        std::cerr << "in g ("<<i<<")\n";
//        if (i % 5 == 0) {
//            std::cerr <<"f: switching\n";
//            switchThreads(1);
//        }
//        usleep(SECOND);
//    }
//}
//
//
//int main(void)
//{
//    t0.setupThread(f, STACK_SIZE);
//    t1.setupThread(g, STACK_SIZE);
//    siglongjmp(t0._env, 1);
//}


// SIGNAL MASKING TEST
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    while (1)
//    {
//        std::cerr << "T1...";
//    }
//}
//
//
//int main(void){
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    //TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T1" << std::endl;
//
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//}
//

//// SLEEP TEST
//
//static int awake = 0;
//
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "Sleep t1"  << std::endl;
//    uthread_sleep(10000);
//
//    //TIME ? :
//    awake = 1;
//    std::cerr << "terminated t1 "<< std::endl;
//    uthread_terminate(1);
//}
//
//
//int main(void){
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    // TIME 3
//    while(!awake){
//        //std::cerr << "T0...";
//    }
//
//    //TIME ? + 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<<"\nTIME: " <<uthread_get_total_quantums()<< std::endl;
//    std:: cerr << "terminated : t0 " << std::endl;
//    uthread_terminate(0);
//
//    //TIME X:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T1" << std::endl;
//
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//}


////BASIC SLEEP TEST:
//
//
//static int awake = 0;
//
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "Sleep t1"  << std::endl;
//    uthread_sleep(10000);
//
//    //TIME ? :
//
//    awake = 1;
//    while(1){
//        std::cerr << "T1...";
//    }
//}
//
//
//int main(void){
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    // TIME 3
//    while(!awake){
//     }
//
//    //TIME ? + 1:
//    std::cerr << "terminated t1 "<< std::endl;
//    uthread_terminate(1);
//
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std:: cerr << "terminated : t0 " << std::endl;
//    uthread_terminate(0);
//
//    //TIME X:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T1" << std::endl;
//
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//    }

////BLOCK->SLEEP TEST:


//static int awake = 0;
//
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": block T 2" << std::endl;
//    uthread_block(2);
//    std::cerr << "T" << uthread_get_tid()<< ": Sleep t1"  << std::endl;
//    uthread_sleep(10000);
//
//    //TIME ? :
//    awake = 1;
//    std::cerr << "AWAKEN!\n";
//    while(1){}
//
//}
//
//void f1(void)
//{
//    //TIME 4:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "resume thread: 1" <<  std::endl;
//    uthread_resume(1);
//
//    while(1){}
//
//}
//
//int main(void){
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f1) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    // TIME 3
//    std::cerr << "T" << uthread_get_tid()<< ": " << "block thread: 1" <<  std::endl;
//    uthread_block(1);
//    std::cerr << "T" << uthread_get_tid()<< ": " << "resume thread: 2" <<  std::endl;
//    uthread_resume(2);
//
//    while(!awake){}
//
//    // TIME (?+1) or (?+2):
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<< std::endl;
//    std:: cerr << "terminated : t2 " << std::endl;
//    uthread_terminate(2);
//    std::cerr << "terminating T1" << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//}


////SLEEP->BLOCK TEST:
//
//static int awake = 0;
//
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": Sleep t1"  << std::endl;
//    uthread_sleep(10000);
//
//    //TIME 12 :
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    awake = 1;
//    std::cerr << "AWAKEN!\n";
//    while(1){} // TIME
//
//}
//
//
//int main(void){
//    assert (uthread_init(1) == 0);
//
//    //TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//
//    while (uthread_get_total_quantums() != 3){}
//
//    // TIME 3
//    std::cerr << "T" << uthread_get_tid()<< ": " << "block thread: 1" <<  std::endl;
//    uthread_block(1);
//
//    //TIME 4-8:
//    while (uthread_get_total_quantums() != 9){}
//
//    //TIME 9,10:
//    int limitQuants = uthread_get_quantums(0) + 2;
//    while (uthread_get_quantums(0) != limitQuants);
//
//    // TIME (11)
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<< std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "resume thread: 1" <<  std::endl;
//    uthread_resume(1);
//    while (!awake){};
//
//    // TIME (13):
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<< std::endl;
//    std::cerr << "terminating T1" << std::endl;
//    uthread_terminate(1);
//    std::cerr << "terminating T0" << std::endl;
//    uthread_terminate(0);
//}


//// SELF TERMINATION TEST

//void f1(void)
//{
//    while(1){
//
//    }
//}
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": Terminates t2"  << std::endl;
//    std::cerr << "T" << uthread_terminate(2)<< std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": Terminates myself (t1)"  << std::endl;
//    std::cerr << "T" << uthread_terminate(1)<< std::endl;
//}
//
//int main(void){
//    uthread_init(1);
//
//    // TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f1) << std::endl;
//
//    while(uthread_get_total_quantums() != 3){
//
//    }
//
//    // TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<< std::endl;
//    std::cerr << "terminating T0" << std::endl;
//    std::cerr << uthread_terminate(0) << std::endl;
//
//}


////SLEEPING AT THE "SAME" TIME TEST:
//
//int awake1 = 0, awake2 = 0;
//
//void f1(void)
//{
//    //TIME 3:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": Sleep t2"  << std::endl;
//    uthread_sleep(10);
//
//    // AFTER WAKING:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    awake2 = 1;
//    std::cerr << "T2 awaken!" << std::endl;
//    while (1){}
//}
//
//void f0(void)
//{
//    //TIME 2:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": Sleep t1"  << std::endl;
//    uthread_sleep(40);
//
//    // AFTER WAKING:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    awake1 = 1;
//    std::cerr << "T1 awaken!" << std::endl;
//    while (1){}
//}
//
//int main(void){
//    uthread_init(1);
//
//    // TIME 1:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid()) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f0) << std::endl;
//    std::cerr << "T" << uthread_get_tid()<< ": " << "spawn thread: " << uthread_spawn(f1) << std::endl;
//
//    while(uthread_get_total_quantums() != 4){
//
//    }
//
//    // TIME 4:
//    while (!awake1 || !awake2 ) {}
//
//    //TIME ?:
//    std::cerr << "T" << uthread_get_tid()<< ": " << uthread_get_quantums(uthread_get_tid())<< std::endl;
//    std::cerr << "terminating T1" << std::endl;
//    std::cerr << uthread_terminate(1) << std::endl;
//    std::cerr << "terminating T2" << std::endl;
//    std::cerr << uthread_terminate(2) << std::endl;
//    std::cerr << "terminating T0" << std::endl;
//    std::cerr << uthread_terminate(0) << std::endl;
//
//}