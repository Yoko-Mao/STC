#ifndef MESSAGEQUEUE_T_H
#define MESSAGEQUEUE_T_H
#include "common/core/task/WorkOrder.h"
#include "common/core/task/ConcurrentQueue.h"
#include "common/core/task/BlockingConcurrentQueue.h"
#include <vector>
#include <utility>
#include <list>
namespace Core {

class WorkOrderQueue_i {
public:
  virtual std::future<WorkOrderResult_t> ScheduleWork(std::function<WorkOrderResult_t(void)> Work) = 0;
  virtual void DoWork() = 0; ///< Perform one item of work. 
  virtual void DoAllWork() = 0; ///< Perform all work on queue.
  virtual ~WorkOrderQueue_i() {
  }
};

/*! \brief A Queue onto which work can be scheduled.
 *
 *	Threadsafe implementation of a workorder queue.
 *  Other threads can schedule work for an object which has its scheduled work done 
 *  in another thread.
 *
 */
class WorkOrderQueue_t : public WorkOrderQueue_i {
public:
  WorkOrderQueue_t() = default;
  WorkOrderQueue_t(WorkOrderQueue_t const& Orig) = delete;
  WorkOrderQueue_t(WorkOrderQueue_t&& Orig) = delete;
  WorkOrderQueue_t& operator=(WorkOrderQueue_t const& Orig) = delete;
  WorkOrderQueue_t& operator=(WorkOrderQueue_t&& Orig) = delete;

  virtual ~WorkOrderQueue_t() {
  }

  //Implementation of WorkOrderQueue_i
  std::future<WorkOrderResult_t> ScheduleWork(std::function<WorkOrderResult_t(void)> Work) override;
  void DoWork() override;
  void DoAllWork() override;
private:
    moodycamel::ConcurrentQueue<WorkOrder_t> m_Queue;
};

/*! \brief A Blocking Queue onto which work can be scheduled.
 *
 *	The Queue blocks the thread if no more work can be done.
 *
 */
class WorkOrderBlockingQueue_t : public WorkOrderQueue_i {
public:
  WorkOrderBlockingQueue_t() = default;
  WorkOrderBlockingQueue_t(WorkOrderBlockingQueue_t const& Orig) = delete;
  WorkOrderBlockingQueue_t(WorkOrderBlockingQueue_t&& Orig) = delete;
  WorkOrderBlockingQueue_t& operator=(WorkOrderBlockingQueue_t const& Orig) = delete;
  WorkOrderBlockingQueue_t& operator=(WorkOrderBlockingQueue_t&& Orig) = delete;

  virtual ~WorkOrderBlockingQueue_t() {
  }

  //Implementation of WorkOrderQueue_i
  std::future<WorkOrderResult_t> ScheduleWork(std::function<WorkOrderResult_t(void)> Work) override;
  void DoWork() override;
  void DoAllWork() override;
private:
    moodycamel::BlockingConcurrentQueue<WorkOrder_t> m_Queue;
};

}


#endif /* MESSAGEQUEUE_T_H */

