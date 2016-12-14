#include "RPC.h"
#include <autobahn/autobahn.hpp>
#include <boost/asio.hpp>
#include <boost/version.hpp>



GRPC_t::GRPC_t(Lobby_t& Lobby) :
RPC_i(Lobby)
{
}

GRPC_t::~GRPC_t()
{
}

RPC_i::RPC_i(Lobby_t& Lobby) :
m_Lobby(Lobby)
{

}

WAMP_t::WAMP_t(Lobby_t& Lobby) :
RPC_i(Lobby)
{
}

WAMP_t::~WAMP_t()
{
}

/*! \brief Add a new user to the lobby.
 * 
 *  Schedules a work item that makes the lobby add a new user.
 *  This is a blocking call that will return when the user was added or the lobby
 *  failed to add the new user.
 * 
 *  \param UserName The name of the user that should be added
 * 
 *  \return OK if user added, failure if user not added.
 * 
 */
Core::WorkOrderResult_t RPC_i::AddUserImpl(std::string const& UserName)
{
  return m_Lobby.AddUser(UserName).get();
}



/*! \brief Google RPC wrapper for AddUserImpl.
 * 
 *  Decodes the received message using the GRPC framework and then calls 'AddUserImpl'.
 * 
 *  \param Context Not used
 *  \param User User to be added in protocol buffer format (see .proto file)
 *  \param Response Response to be sent back to client. Not applicable for this RPC.
 * 
 *  \return OK if user added, failure if user not added.
 * 
 */
::grpc::Status GRPC_t::AddUser(::grpc::ServerContext *Context, const ::comm::User *User, ::google::protobuf::Empty *Response)
{
  //Core::WorkOrderResult_t Result = AddUserImpl(UserName);
  return ::grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "OOPS MY CD JUST SKIPPED");
}

/*! \brief Start the google RPC server on both interfaces.
 * 
 *  The RPC server will listen on both interfaces. It is intended for one interface
 *  to be 'local' and not accessible from outside. This interface should be used
 *  for scripting and dumping useful information.
 *  The remote interface should be accessible from outside and is used be clients to communicate with.
 * 
 *  \param LocalInterface Local Ip and port combination. e.g. 0.0.0.0:6666.
 *  \param RemoteInterface Remote Ip and port combination. e.g. 0.0.0.0:6667.
 * 
 * 
 */
void GRPC_t::StartListeningOnInterfaces(std::string&& LocalInterface, std::string&& RemoteInterface)
{
  // Listen on the given address without any authentication mechanism.
  auto ServerBuilder = ::grpc::ServerBuilder();
  ServerBuilder.AddListeningPort(LocalInterface, grpc::InsecureServerCredentials());
  ServerBuilder.AddListeningPort(RemoteInterface, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  ServerBuilder.RegisterService(this);
  // Finally assemble the server.
  auto server = std::unique_ptr<::grpc::Server>(ServerBuilder.BuildAndStart());

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

/*! \brief Connect to the WAMP server
 * 
 *  Connect to the WAMP router and register all remote procedure calls that
 *  clients may call.
 * 
 *  \param LocalInterface Not used
 *  \param WAMP_ServerEndpoint Port and IP onto which the WAMP router is listening.
 * 
 * 
 */
void WAMP_t::StartListeningOnInterfaces(std::string&& LocalInterface, std::string&& WAMP_ServerEndpoint)
{
}