#ifndef IMESSAGE_T_H
#define IMESSAGE_T_H
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <utility>
#include <boost/optional.hpp>
//#include <experimental/any>
namespace Core {

class WorkOrderResult_t
{
public: 
  enum class ErrorCode_t: uint32_t
  {
    SUCCESS,  ///< Work has been completed successfully
    ERROR,    ///< General Error; unspecified. Values below are more specific.
    DUPLICATE,///< Tried adding an unique item that already exists.
    NOT_FOUND ///< Tried modifying an existing unique item but it does not exist.
  };
  WorkOrderResult_t():m_Value(ErrorCode_t::SUCCESS) {}
  WorkOrderResult_t(ErrorCode_t Value): m_Value(Value) {}
  WorkOrderResult_t(ErrorCode_t Value, std::string const& Info): m_Value(Value), m_Info(Info) {}
  inline bool IsSuccessfull() const{return m_Value == ErrorCode_t::SUCCESS;}
  inline bool IsFailure() const {return m_Value != ErrorCode_t::SUCCESS;}
  inline std::string GetReasonForFailure(){ return m_Info;}
  inline ErrorCode_t GetErrorCode(){return m_Value;}
  
  WorkOrderResult_t(boost::optional<std::future<WorkOrderResult_t>> Optional)
  {
    m_Value = (Optional) ? Optional->get().m_Value : Core::WorkOrderResult_t::ErrorCode_t::ERROR;
    m_Info = "Future was not set";
  }
private:
    ErrorCode_t m_Value; ///< Indicates success or failure
    std::string m_Info; ///< More info why the call succeeded or failed.
    //std::experimental::any m_Object; ///< Additional object storage

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

