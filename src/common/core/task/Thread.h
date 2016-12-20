#ifndef THREAD_T_H
#define THREAD_T_H
#include "common/core/task/WorkOrderQueue.h"
#include <thread>
#include <atomic>
#include <thread>
#include <chrono>
#include <boost/optional.hpp>
namespace Core {

/*! \brief Basic thread implementation.
 *
 *	When created an object of type Thread_t will run the Closure 'work'
 *  until it is finished. This is basically a wrapper around std::thread.
 * 
 *  \note Default constructor is deleted because some closure needs to be provided.
 *
 */
class Thread_t {
public:
  Thread_t() = delete;

  Thread_t(std::function<void(void) > Work) : m_Thread(Work) {
  }
  Thread_t(Thread_t const& Orig) = delete;
  Thread_t(Thread_t const&& Orig) = delete;
  Thread_t& operator=(Thread_t const& Orig) = delete;
  Thread_t& operator=(Thread_t const&& Orig) = delete;

  virtual ~Thread_t() {
  }

  std::thread m_Thread;
  std::function<void(void) > m_Work;
};

/*! \brief Thread implementation with work queue
 *
 *  Each constructor, constructs a thread with different behavior.
 *
 */
class WorkOrderQueueThread_t : Thread_t {
public:

  WorkOrderQueueThread_t(std::function<void(void)> Func);
  WorkOrderQueueThread_t();
  WorkOrderQueueThread_t(WorkOrderQueueThread_t const& Orig) = delete;
  WorkOrderQueueThread_t(WorkOrderQueueThread_t&& Orig) = delete;
  WorkOrderQueueThread_t& operator=(WorkOrderQueueThread_t const& Orig) = delete;
  WorkOrderQueueThread_t& operator=(WorkOrderQueueThread_t&& Orig) = delete;
  virtual ~WorkOrderQueueThread_t() override {Stop();}

  boost::optional<std::future<WorkOrderResult_t> > ScheduleWork(std::function<WorkOrderResult_t(void)> Func);
  void Stop();

private:
  std::atomic_bool m_ShouldStop; ///< Indicate that the thread should close.
  std::function<void(void)> m_WorkToDoEachIteration; ///< Work that should be executed sequentially on this thread. Can be none.
  const std::unique_ptr<WorkOrderQueue_i> m_WorkQueue; ///< Any WorkOrder queue impl that supports the WorkOrderQueue_i interface.
};
}


#endif /* THREAD_T_H */

