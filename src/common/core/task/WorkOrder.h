#ifndef IMESSAGE_T_H
#define IMESSAGE_T_H
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <utility>
namespace Core {

class WorkOrderResult_t {
};



/*! \brief One item of work which can be performed.
 *  
 *  This is more or less a wrapper around a closure.
 *  It cannot be instantiated as support for interfaces 'IWorkOrderForCaller_t'
 *  and 'IWorkOrderForCallee_t' are not implemented.
 *  
 */
class WorkOrder_t{

public:
  WorkOrder_t():m_Function(),m_Promise() {}; ///< Need default constructor.
  WorkOrder_t(std::function<WorkOrderResult_t(void)> Function) : m_Function(Function), m_Promise() {}
  
  
  WorkOrder_t(WorkOrder_t const& Orig) = delete;
  WorkOrder_t& operator=(WorkOrder_t const& Orig) = delete;
  
  WorkOrder_t(WorkOrder_t&& Orig):
    m_Function(std::move(Orig.m_Function)),
    m_Promise(std::move(Orig.m_Promise)){}

  WorkOrder_t& operator=(WorkOrder_t&& Orig)
  {
    m_Function = std::move(Orig.m_Function);
    m_Promise = std::move(Orig.m_Promise);
    return *this;
  }
  virtual ~WorkOrder_t() {};
  virtual void DoWork()
  {
    m_Promise.set_value(m_Function());
  }

  auto GetFuture(){return m_Promise.get_future();}
  auto GetWork(){return m_Function;}
protected:
  std::function<WorkOrderResult_t(void)> m_Function;
  std::promise<WorkOrderResult_t> m_Promise;
 
};

}
#endif /* IMESSAGE_T_H */

