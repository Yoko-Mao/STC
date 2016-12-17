#ifndef TESTSUITE_TASK_H
#define TESTSUITE_TASK_H
#include "common/core/task/Thread.h"
#include <boost/test/included/unit_test.hpp>
#include <boost/optional.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(MessageQueue)


// Create 1 thread with blocking work queue
// Create NUM_PROD_THREAD threads that schedule NUM_MESSSAGE_EACH_PROD_THREAD messages on this work thread.
// Each messages increase some integer by 1. 
// When the working thread is done handling all messages the value of the int
// should be NUM_PROD_THREAD * NUM_MESSSAGE_EACH_PROD_THREAD.
BOOST_AUTO_TEST_CASE(Push)
{
  const int NUM_PROD_THREAD = 3;
  const int NUM_MESSSAGE_EACH_PROD_THREAD = 500;
  const int EXPECTED_TOTAL = NUM_PROD_THREAD * NUM_MESSSAGE_EACH_PROD_THREAD;

  class Test_t
  {
  public:
    uint32_t m_NumberL;

    Test_t() : m_NumberL(0), m_Thread() { }
    Core::WorkOrderQueueThread_t m_Thread;

    auto IncreaseNumberL()
    {
      return m_Thread.ScheduleWork([&]
      {
        m_NumberL++; return Core::WorkOrderResult_t();
      });
    }

  };
  Test_t Test;


  std::vector<std::thread> threads;
  for (int i = 0; i < NUM_PROD_THREAD; i++)
  {
    threads.push_back(
      std::thread([&]
      {
        for (int j = 0; j < NUM_MESSSAGE_EACH_PROD_THREAD; j++)
        {
          //Don't wait for the reply; dont really care it should work.
          boost::optional<std::future<Core::WorkOrderResult_t> > Future = Test.IncreaseNumberL();
        }
      }));
  }

  //wait for all threads to queue their messages.
  for (auto&& thread : threads)
  {
    thread.join();
  }
  
  // All threads are done can stop now 
  // this will still handle all messages that are queued (if any).
  Test.m_Thread.Stop();

  BOOST_TEST(EXPECTED_TOTAL == Test.m_NumberL);


}
BOOST_AUTO_TEST_SUITE_END()

#endif /* TESTSUITE_TASK_H */

