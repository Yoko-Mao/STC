#pragma once
#include "common/core/task/WorkOrder.h"
#include "server/state/User.h"

/*! \brief This interface describes all functionalities a communication
 *  proxy object should support. 
 * 
 *  Typically these requirements are implemented by inheriting from the CommunicationBase_t class 
 *  but that is obviously not a necessity. 
 * 
 *  The communication_i interface is typically used in the context of the server state
 *  and is the only way the communication entities (database, RPC,..) are exposed to
 *  the state of the server.
 *  
 */
class Communication_i
{
public:
  virtual boost::optional<User_t> ReadUser(std::string const& UserName) = 0; //Retrieve User from database.

  //Add stuff here that should be called from state.
};
