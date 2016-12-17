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
 * \param UserName ID of new user to add; Expected to be unique.
 * 
 * \return future that will be completed once the work item is handled or boost::none if work could not be scheduled.
 */
boost::optional<std::future<Core::WorkOrderResult_t> > Lobby_t::AddUser_Threadsafe(std::string const& UserName)
{
  return m_Queue.ScheduleWork([&]{ return AddUser_Implementation(UserName);});
}

/*! \brief Implementation to add new user to Lobby.
 *
 * Not threadsafe; Should not be called from outside the lobby.
 * 
 * 
 * \param UserName ID of new user to add; Expected to be unique.
 * 
 * 
 * \return future that will be completed once the work item is handled.
 */
Core::WorkOrderResult_t Lobby_t::AddUser_Implementation(std::string const& UserName)
{
  auto Pair = m_Users.insert(User_t(UserName));
  if (!Pair.second)
  {
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::DUPLICATE, "User already exists in Lobby.");
  }
  else
  {
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::SUCCESS);
  }

}
