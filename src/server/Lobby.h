#ifndef LOBBY_H
#define LOBBY_H
#include "common/core/task/Thread.h"

/*! \brief A lobby object stores the state of a server.
 *
 * Work is scheduled by the server RPC calls on a lobby object its WorkOrderQueue.
 * This queue is blocking and the thread owned by the lobby is only supposed 
 * to handle these work items.
 */
class Lobby_t
{
public:
  Lobby_t();
  virtual ~Lobby_t();
  
  std::future<Core::WorkOrderResult_t> AddGame();
  std::future<Core::WorkOrderResult_t> GetGames();
  std::future<Core::WorkOrderResult_t> RemoveGame();
  std::future<Core::WorkOrderResult_t> AddUser(std::string const&); ///< Add a new user to this lobby.
private:
  Core::WorkOrderQueueThread_t m_Queue;

};

#endif /* LOBBY_H */

