#ifndef RPC_H
#define RPC_H

#include "Lobby.h"
#include <grpc++/grpc++.h>
#include <server.grpc.pb.h>
#include <iostream>

/*! \brief Abstract representation of a server side RCP implementation
 *
 * This interface decouples the used RPC framework (e.g. google RPC) and the
 * server side code. Using another framework should be as simple as inheriting 
 * from this class and implementing the pure virtual functions.
 * 
 * For each RPC there is a "<RPC NAME>Impl" function that should be called by 
 * the implementation class. These functions hook into the lobby and perform
 * the required function calls. The lobby object stores the state of the server.
 */
class RPC_i
{
public:
  RPC_i(Lobby_t& Lobby);
  RPC_i& operator=(RPC_i const&) = delete;
  RPC_i& operator=(RPC_i&&) = delete;
  RPC_i(RPC_i const&) =delete;
  RPC_i(RPC_i&&) = delete;
  virtual void StartListeningOnInterfaces(std::string&& LocalInterface, std::string&& RemoteInterface) = 0;

  virtual ~RPC_i() { }
protected:
  Core::WorkOrderResult_t AddUserImpl(std::string const&  UserName); ///< Add new user to the lobby.
private:
  Lobby_t& m_Lobby; ///< Reference to the lobby. Does not own or is resposibility for its lifetime.
};

/*! \brief Google RPC implementation of RPC_i
 *
 */
class GRPC_t : public RPC_i, public comm::Server::Service
{
public:
  GRPC_t(Lobby_t& Lobby);
  virtual ~GRPC_t();
  ::grpc::Status AddUser(::grpc::ServerContext*, const ::comm::User*, ::google::protobuf::Empty*) override;
  virtual void StartListeningOnInterfaces(std::string&&, std::string&&) override;
};

/*! \brief Web Application Messaging Protocol implementation of RPC_i
 *
 */
class WAMP_t : public RPC_i
{
public:
  WAMP_t(Lobby_t& Lobby);
  virtual ~WAMP_t();
  virtual void StartListeningOnInterfaces(std::string&&, std::string&&) override;
};
#endif /* RPC_H */

