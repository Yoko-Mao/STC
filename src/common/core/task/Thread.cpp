#include "Thread.h"

/*! \brief Constructor
 *
 *  The entity created with this constructor will handle one work item
 *  and then execute the closure. If the queue is empty it will not block
 *  and only execute the provided closure. It will keep executing until
 *  'stop' is called.
 * 
 *  \param Func Closure to execute in the newly created thread..
 *
 */
Core::WorkOrderQueueThread_t::WorkOrderQueueThread_t(decltype(m_Work) Func) :
Thread_t(
[&]
{
  while (!m_ShouldStop.load(std::memory_order_acquire))
  {
    // Should finish all work in queue before finishing? May block other threads.
    // Perhaps deliver some 'can't deliver' result in future.
    m_WorkQueue->DoWork();
    m_WorkToDoEachIteration();
  }

  m_WorkQueue->DoAllWork();
}),
// TODO: There MAY be a problem here as m_WorkQueue and m_WorkToDoEachIteration 
// are dereferenced inside the Thread_t functor. Requires investigation.
// I'm unsure about the rules concerning member initializer lists.
m_WorkToDoEachIteration(Func),
m_WorkQueue(new Core::WorkOrderQueue_t()) 
{
  m_ShouldStop.store(false, std::memory_order_release);
}

/*! \brief Constructor
 *
 *  The entity created with this constructor will only handle work items.
 *  if no work items are present the thread will block. The thread will be automatically
 *  unblocked if new work has been scheduled. It will keep executing until
 *  'stop' is called.
 *
 */
Core::WorkOrderQueueThread_t::WorkOrderQueueThread_t() :
Thread_t(
[&]
{
  while (!m_ShouldStop.load(std::memory_order_acquire))
  {
    m_WorkQueue->DoWork();
  }

  m_WorkQueue->DoAllWork();
}),
m_WorkQueue(new WorkOrderBlockingQueue_t())
{
  m_ShouldStop.store(false, std::memory_order_release);
}

/*! \brief Schedules work on the work queue assigned to this thread.
 *
 *  When the tread is closing down no new work can be scheduled on the thread 
 *  any longer and the function will return false.
 * 
 *  \param Func Closure to be executed by this thread.
 *  \param Future std::future that will be completed when the work item was handled.
 *  \returns True if queue allowed work to be scheduled, else false.
 *
 */
bool Core::WorkOrderQueueThread_t::ScheduleWork(std::function<WorkOrderResult_t(void)> Func, std::future<Core::WorkOrderResult_t>& Future)
{
  bool IsStopped = m_ShouldStop.load(std::memory_order_release);

  if (!IsStopped)
  {
    Future = m_WorkQueue->ScheduleWork(Func);
  }

  return !IsStopped;
}

/*! \brief Stops the thread.
 *
 *  This is not a hard stop. But it will:
 *    - Complete the current iteration (e.g. finish current closure execution).
 *    - Handle remaining work items on the queue.
 * 
 *  \note This blocks the calling thread and returns when the callee thread has stopped.
 *
 */
void Core::WorkOrderQueueThread_t::Stop()
{
  m_ShouldStop.store(true, std::memory_order_release);
  
  //Need to schedule an additional work item to unblock any blocking work queues.
  m_WorkQueue->ScheduleWork([]()
  {
    return WorkOrderResult_t();
  });
  m_Thread.join();
}