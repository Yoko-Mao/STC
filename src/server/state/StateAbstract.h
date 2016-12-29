#pragma once

#include "common/core/task/WorkOrder.h"
#include "server/state/User.h"

/*! \brief This interface describes all functionalities the state supports.
 * 
 *  The state_i interface is typically used in the context of the communication side of things.
 *  It allows the communication proxy object to access the internal entities of the state.
 * 
 *  
 */
class State_i
{
public:
  virtual Core::WorkOrderResult_t RemoveUser(std::string const& UserName) = 0;
  virtual Core::WorkOrderResult_t AddUser(std::string const& UserName) = 0;  
  virtual Core::WorkOrderResult_t GetUsers(std::set<User_t>& Users) = 0;
};