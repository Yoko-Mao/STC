#ifndef THREAD_T_H
#define THREAD_T_H
#include "common/core/task/MessageQueue.h"
namespace Core {

class Thread_t {
public:
  Thread_t() = default;
  Thread_t(Thread_t const& Orig) = delete;
  Thread_t(Thread_t const&& Orig) = delete;
  Thread_t& operator=(Thread_t const& Orig) = delete;
  Thread_t& operator=(Thread_t const&& Orig) = delete;
  virtual ~Thread_t() {}
  
  template <typename T>
  auto ScheduleWork(std::function<WorkOrderResult_t(void)> Func) -> std::shared_ptr<IWorkOrderForCaller_t>
  {
    return m_WorkQueue.ScheduleWork(std::make_shared<T>(Func));
  }
private:
  auto DoWork()
  {
    m_WorkQueue.DoWork();
  }
  MessageQueue_t m_WorkQueue;
};
}


#endif /* THREAD_T_H */

