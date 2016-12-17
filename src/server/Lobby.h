#ifndef LOBBY_H
#define LOBBY_H
#include "common/core/task/Thread.h"
#include <set>
#include <functional>
#include <boost/optional.hpp>
class User_t
{
public:
  User_t(std::string Name):m_Name(Name){}
  std::string m_Name;
  
  
  bool operator< (User_t const& Other) const
  {
    std::hash<std::string> HashFunction;
    auto HashOwn = HashFunction(m_Name);
    auto HashOther = HashFunction(Other.m_Name);
    return HashOwn < HashOther;
  }
};
/*! \brief A lobby object stores the state of a server.
 *
 * Work is scheduled by the server RPC calls on a lobby object its WorkOrderQueue.
 * This queue is blocking and the thread owned by the lobby is only supposed 
 * to handle these work items.
 * 
 * Always call <Function>_Threadsafe from RPC implementations.
 */
class Lobby_t
{
public:
  Lobby_t();
  virtual ~Lobby_t();
  
  boost::optional<std::future<Core::WorkOrderResult_t> >AddUser_Threadsafe(std::string const&); ///< Add a new user to this lobby.
private:
  Core::WorkOrderQueueThread_t m_Queue;
  Core::WorkOrderResult_t AddUser_Implementation(std::string const&);
  std::set<User_t> m_Users;

};

#endif /* LOBBY_H */

