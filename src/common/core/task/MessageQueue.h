#ifndef MESSAGEQUEUE_T_H
#define MESSAGEQUEUE_T_H
#include "common/core/task/IWorkOrder.h"
#include <vector>
namespace Core
{
class MessageQueue_t {
public:
  MessageQueue_t() = default;
  MessageQueue_t(MessageQueue_t const& Orig) = delete;
  MessageQueue_t(MessageQueue_t const&& Orig) = delete;
  MessageQueue_t& operator=(MessageQueue_t const& Orig) = delete;
  MessageQueue_t& operator=(MessageQueue_t const&& Orig) = delete;
  virtual ~MessageQueue_t() {}
  auto ScheduleWork(std::shared_ptr<IWorkOrder_t> Message) -> std::shared_ptr<IWorkOrderForCaller_t>
  {
    m_Queue.push_back(Message);
    auto Message_ = std::static_pointer_cast<IWorkOrderForCaller_t>(m_Queue.back());
    return Message_;
  }
  void DoWork()
  {
    IWorkOrderForCallee_t* Message = static_cast<IWorkOrder_t*>(m_Queue.back().get());
    Message->DoWork();
    Message->SignalWorkCompleted();
    m_Queue.pop_back();
  }
  
private:
  
  std::vector<std::shared_ptr<IWorkOrder_t> > m_Queue;
};
}


#endif /* MESSAGEQUEUE_T_H */

