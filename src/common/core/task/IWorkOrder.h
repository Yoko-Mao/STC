#ifndef IMESSAGE_T_H
#define IMESSAGE_T_H
#include <functional>
#include <mutex>
#include <condition_variable>
namespace Core {

class WorkOrderResult_t {
};

class IWorkOrderForCaller_t {
public:
  virtual ~IWorkOrderForCaller_t() = 0;
  virtual void BlockUntilWorkCompleted() = 0;
};

class IWorkOrderForCallee_t {
public:
  virtual ~IWorkOrderForCallee_t() = 0;
  virtual void DoWork() = 0;
  virtual void SignalWorkCompleted() = 0;
};

//operators??? consructors??? delete/defaults???
class IWorkOrder_t : public IWorkOrderForCaller_t, public IWorkOrderForCallee_t {
protected:
  std::function<WorkOrderResult_t(void)> m_Function;
  
public:
  virtual ~IWorkOrder_t() = 0;

  virtual void DoWork() {
    m_Function();
  }

  IWorkOrder_t(std::function<WorkOrderResult_t(void)> Function) : m_Function(Function) {

  }


};

}
#endif /* IMESSAGE_T_H */

