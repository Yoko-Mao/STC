#ifndef CONDITIONVARIABLEWORKORDER_T_H
#define CONDITIONVARIABLEWORKORDER_T_H

#include "IWorkOrder.h"


namespace Core {
class ConditionVariableWorkOrder_t : public IWorkOrder_t{
public:
  ConditionVariableWorkOrder_t(std::function<WorkOrderResult_t(void)> Function):
    IWorkOrder_t(Function),
    m_Mutex(),
    m_ConditionVariable(){}
    
  ConditionVariableWorkOrder_t(ConditionVariableWorkOrder_t const& Orig):
    IWorkOrder_t(Orig.m_Function),
    m_Mutex(),
    m_ConditionVariable(){}

  ConditionVariableWorkOrder_t(ConditionVariableWorkOrder_t&& Orig):
    IWorkOrder_t(std::move(Orig.m_Function)),
    m_Mutex(),
    m_ConditionVariable(){}
  
  ConditionVariableWorkOrder_t& operator=(ConditionVariableWorkOrder_t const& Orig) = delete;
  ConditionVariableWorkOrder_t& operator=(ConditionVariableWorkOrder_t const&& Orig) = delete;
  
  void SignalWorkCompleted()
  {
    m_ConditionVariable.notify_one();
  }
  
  void BlockUntilWorkCompleted()
  {
    std::unique_lock<std::mutex> Lock(m_Mutex);
    m_ConditionVariable.wait(Lock);
  }
  
  
private:
  std::mutex m_Mutex;
  std::condition_variable m_ConditionVariable;
};
}


#endif /* CONDITIONVARIABLEWORKORDER_T_H */

