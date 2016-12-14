#include "server/Lobby.h"
#include "RPC.h"

Lobby_t::Lobby_t():
  m_Queue()
{
  
}

Lobby_t::~Lobby_t()
{
}
/*! \brief Add a new user to the lobby.
 *
 * \param UserName ID of new user to add.
 * 
 * \return future that will be completed once the work item is handled.
 */
std::future<Core::WorkOrderResult_t> Lobby_t::AddUser(std::string const& UserName)
{
  
  std::future<Core::WorkOrderResult_t> Future;
  m_Queue.ScheduleWork([&]{ return Core::WorkOrderResult_t();},Future);
  return Future;
}
std::future<Core::WorkOrderResult_t> Lobby_t::AddGame()
{
  std::future<Core::WorkOrderResult_t> Future;
  m_Queue.ScheduleWork([&]{ return Core::WorkOrderResult_t();},Future);
  return Future;
}

std::future<Core::WorkOrderResult_t> Lobby_t::GetGames()
{
  std::future<Core::WorkOrderResult_t> Future;
  m_Queue.ScheduleWork([&]{ return Core::WorkOrderResult_t();},Future);
  return Future;
}

std::future<Core::WorkOrderResult_t> Lobby_t::RemoveGame()
{
  std::future<Core::WorkOrderResult_t> Future;
  m_Queue.ScheduleWork([&]{return Core::WorkOrderResult_t();},Future);
  return Future;
}
