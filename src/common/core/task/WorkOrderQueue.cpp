#include "WorkOrderQueue.h"
#include <functional>

/*! \brief Perform one item of work.
 *
 *  Completes a work item from the queue in the calling thread context.
 *  For example if one thread is considered to be the owner of this WorkOrderQueue,
 *  it should be the only calling DoWork.
 * 
 * \note If the queue is empty no state is changed and it was as if this function
 * was never called.
 *
 */
void Core::WorkOrderQueue_t::DoWork()
{
    WorkOrder_t WorkOrder;
    if (m_Queue.try_dequeue(WorkOrder))
    {
      WorkOrder.DoWork();
    }
}

/*! \brief Threadsafe schedule work.
 *
 *  \parameter Work Closure that should be executed in target thread context.
 *  \returns std::future for the work order result.
 * 
 *  \note Because work items may be handled in another thread a lock is required
 *  to schedule work. Scheduling pushes items on the to do queue.
 *
 */
std::future<Core::WorkOrderResult_t> Core::WorkOrderQueue_t::ScheduleWork(std::function<Core::WorkOrderResult_t(void)> Work)
{
  Core::WorkOrder_t WorkOrder(Work);
  auto Future = WorkOrder.GetFuture();
  m_Queue.enqueue(std::move(WorkOrder));
  return Future;
}

/*! \brief Perform one item of work.
 *
 *  Completes a work item from the queue in the calling thread context.
 *  For example if one thread is considered to be the owner of this WorkOrderQueue,
 *  it should be the only calling DoWork.
 * 
 *  \note Blocks the calling thread if the queue is empty.
 *
 */
void Core::WorkOrderBlockingQueue_t::DoWork()
{
  WorkOrder_t WorkOrder;
  m_Queue.wait_dequeue(WorkOrder);
  WorkOrder.DoWork(); 
}

/*! \brief Perform all items of work.
 *
 *  Empties the queue by completing all work items left in the queue.
 * 
 *  \note Does NOT blocks the calling thread if the queue is empty.
 *
 */
void Core::WorkOrderBlockingQueue_t::DoAllWork()
{
  WorkOrder_t WorkOrder;
  while (m_Queue.try_dequeue(WorkOrder))
  {
    WorkOrder.DoWork(); 
  }
}
/*! \brief Perform all items of work.
 *
 *  Empties the queue by completing all work items left in the queue.
 * 
 *
 */
void Core::WorkOrderQueue_t::DoAllWork()
{
  WorkOrder_t WorkOrder;
  while (m_Queue.try_dequeue(WorkOrder))
  {
    WorkOrder.DoWork(); 
  }
}
/*! \brief Schedule work threadsafe on this queue.
 *
 *  \parameter Work Closure that should be executed in target thread context.
 *  \returns std::future for the work order result.
 * 
 *  \note If the queue was empty prior to pushing the work item, the thread handling the 
 *  work items will be unblock automatically. No other function calls are required.
 *
 */
std::future<Core::WorkOrderResult_t> Core::WorkOrderBlockingQueue_t::ScheduleWork(std::function<Core::WorkOrderResult_t(void)> Work)
{
  Core::WorkOrder_t WorkOrder(Work);
  auto Future = WorkOrder.GetFuture();
  m_Queue.enqueue(std::move(WorkOrder));
  return Future;
}