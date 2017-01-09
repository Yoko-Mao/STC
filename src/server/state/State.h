#ifndef LOBBY_H
#define LOBBY_H
#include "common/core/task/Thread.h"
#include "server/communication/CommunicationAbstract.h"
#include "server/state/StateAbstract.h"

/*! \brief A State object stores all the entities alive on the server.
 *
 * Work is scheduled by the state object its WorkOrderQueue.
 * Pushing on this queue is blocking and the thread owned by the lobby is only supposed 
 * to handle these work items. It does no additional work except handling scheduled work items.
 * The server state is effectively a reactive entity that responds to work items mostly generated from outside its thread context.
 * 
 * The state also interfaces with the database. For example, adding an user to the state
 * will also add it to the database. Removing it from the state will also remove it
 * from the database. 
 * Something that is expected to be unique in the server state is also checked for its
 * uniqueness in the database. While the entity may not be 'alive' now 
 * it may be loaded later from the database and conflicts should be avoided.
 * 
 */
class State_t : public State_i
{
public:
  State_t(Communication_i& CommunicationForState);
  virtual ~State_t();
  
  //State_i
  Core::WorkOrderResult_t AddUser(std::string const& UserName) override; ///< Add a new persistent user.
  Core::WorkOrderResult_t RemoveUser(std::string const& UserName) override; ///< Remove persistent user.
  Core::WorkOrderResult_t GetUsers(std::set<User_t>&) override; ///< Get ALL persistent users.
  
private:
  Core::WorkOrderResult_t AddUser_Implementation(std::string const&);
  Core::WorkOrderResult_t RemoveUser_Implementation(std::string const&);
  Core::WorkOrderResult_t GetUsers_Implementation(std::set<User_t>& );
  Core::WorkOrderQueueThread_t m_Queue; ///< Lobby thread; work for server is scheduled on this thread.
  Communication_i& m_Communication; ///< Ref to some object handling database access.
  std::set<User_t> m_Users; ///< Users in lobby.
};

#endif /* LOBBY_H */

