#include "server/state/State.h"

State_t::State_t(Communication_i& CommunicationForState):
  m_Queue(),
  m_CommunicationForState(CommunicationForState),
  m_Users()
{

}

State_t::~State_t()
{
}

/*! \brief Add a new user to the lobby.
 *
 * This is a blocking and threadsafe call. The optional future returned
 * from the scheduled work item is converted to a work order result
 * using the specific constructor of WorkOrderResult_t.
 * 
 * \param UserName ID of new user to add; Expected to be unique.
 * 
 * \return Result of the work. Value set to ErrorCode_t::ERROR if the task
 * blocked the queuing of additional work items.
 * 
 */
Core::WorkOrderResult_t State_t::AddUser(std::string const& UserName)
{
  return m_Queue.ScheduleWork([&]{ return AddUser_Implementation(UserName);});
}

/*! \brief Remove an existing user from the lobby.
 *
 * This is a blocking and threadsafe call. The optional future returned
 * from the scheduled work item is converted to a work order result
 * using the specific constructor of WorkOrderResult_t.
 * 
 * \param UserName ID of the existing user to remove
 * 
 * \return Result of the work. Value set to ErrorCode_t::ERROR if the task
 * blocked the queuing of additional work items.
 * 
 */
Core::WorkOrderResult_t State_t::RemoveUser(std::string const& UserName)
{
  return m_Queue.ScheduleWork([&]{ return RemoveUser_Implementation(UserName);});
}

/*! \brief Add a new user to the lobby.
 *
 * This is a blocking and threadsafe call. The optional future returned
 * from the scheduled work item is converted to a work order result
 * using the specific constructor of WorkOrderResult_t.
 * 
 * \param UserName ID of new user to add; Expected to be unique.
 * 
 * \return Result of the work. Value set to ErrorCode_t::ERROR if the task
 * blocked the queuing of additional work items.
 * 
 */
Core::WorkOrderResult_t State_t::GetUsers(std::set<User_t>&  Users)
{
  return m_Queue.ScheduleWork([&]{ return GetUsers_Implementation(Users);});
}

/*! \brief Implementation to add new user to Lobby.
 *
 * Not threadsafe; Should not be called from outside the lobby.
 * 
 * 
 * \param UserName ID of new user to add; Expected to be unique.
 * 
 * 
 * \return Future that will be completed once the work item is handled.
 */
Core::WorkOrderResult_t State_t::AddUser_Implementation(std::string const& UserName)
{
  User_t User(UserName);
  auto Pair = m_Users.insert(User);
  if (!Pair.second)
  {
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::DUPLICATE, "User already exists in Lobby.");
  }
  else
  {
    //m_Database.Insert(User);
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::SUCCESS);
  }

}

/*! \brief Implementation to remove an existing user from the lobby.
 *
 * Not threadsafe; Should not be called from outside the lobby.
 * 
 * \param UserName ID of user to remove.
 * 
 * \return Future that will be completed once the work item is handled.
 */
Core::WorkOrderResult_t State_t::RemoveUser_Implementation(std::string const& UserName)
{
  auto WasRemoved = m_Users.erase(User_t(UserName));
  if (!WasRemoved)
  {
    //m_Database.Delete(UserName);
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::NOT_FOUND, "Can not remove non existing user.");
  }
  else
  {
    return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::SUCCESS);
  }
}
Core::WorkOrderResult_t State_t::GetUsers_Implementation(std::set<User_t>& Users)
{
  for (auto User: m_Users)
  {
    Users.insert(User);
  }
  
  return Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::SUCCESS);
}