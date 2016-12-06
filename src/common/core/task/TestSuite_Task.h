#ifndef TESTSUITE_TASK_H
#define TESTSUITE_TASK_H
#include <boost/test/included/unit_test.hpp>
#include "common/core/task/Thread.h"
#include <iostream>
BOOST_AUTO_TEST_SUITE( MessageQueue )

// Create 3 threads.
// One monitor threads and two worker threads.
// Schedule 'work' on both worker threads from the monitor thread.
// The work of worker THREAD_WL, takes way longer than the work of worker thread THREAD_WS.
// The monitor thread (THREAD_M) first waits for the result of THREAD_WL.
// This should force the monitor thread to block until work of THREAD_WL is done.
// However in the mean time THREAD_WS should have already notified that it completed
// its work. But the monitor thread blocks on THREAD_WL.
//BOOST_AUTO_TEST_CASE (Basic_Thread_Sync)
//{
//    Core::WorkOrderQueueThread_t Thread_WS([]{});
//    Core::WorkOrderQueueThread_t Thread_WL([]{});
//    auto LongWork = Thread_WL.ScheduleWork<Core::ConditionVariableWorkOrder_t>([](){
//        return Core::WorkOrderResult_t();
//    });
//    
//    auto ShortWork = Thread_WS.ScheduleWork<Core::ConditionVariableWorkOrder_t>([](){
//        return Core::WorkOrderResult_t();
//    });
//    
//    LongWork->BlockUntilWorkCompleted();
//    ShortWork->BlockUntilWorkCompleted();
//}

// Create 1 thread with work queue
// During its thread function it updates some number by += 2 each iteration.
// 
BOOST_AUTO_TEST_CASE (Push)
{
  class Test_t
  {
  public:
    uint32_t m_NumberR;
    uint32_t m_NumberL;

    Test_t():m_NumberR(0), m_NumberL(0), m_Thread([&]{m_NumberR++;}){}
    Core::WorkOrderQueueThread_t m_Thread;
    

    
//    auto GetNumberL(decltype(m_NumberL)& Ref)
//    {
//      return m_Thread.ScheduleWork([&]{Ref = m_NumberL; return Core::WorkOrderResult_t();});
//    }
//    
//    auto IncreaseNumberL()
//    {
//      return m_Thread.ScheduleWork([&]{m_NumberL++; return Core::WorkOrderResult_t();});
//    }
//      
    
  };
  //Test_t Test;
  Core::WorkOrderQueueThread_t Thread;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  Thread.Stop();
  
  
}
BOOST_AUTO_TEST_SUITE_END()

#endif /* TESTSUITE_TASK_H */

