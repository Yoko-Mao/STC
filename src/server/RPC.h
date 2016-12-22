#ifndef RPC_H
#define RPC_H
#define BOOST_RESULT_OF_USE_DECLTYPE
#include "Lobby.h"
#include <grpc++/grpc++.h>
#include <server.grpc.pb.h>
#include <autobahn/autobahn.hpp>
#include <boost/asio.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <set>

/*! \brief Abstract representation of a server side RPC implementation
 *
 * This interface decouples the used RPC framework (e.g. google RPC) and the
 * server side code. Using another framework should be as simple as inheriting 
 * from this class and implementing the pure virtual functions.
 * 
 * For each RPC there is a "<RPC NAME>_Implementation" function that should be called by 
 * the implementation class. These functions hook into the lobby and perform
 * the required function calls. The lobby object stores the state of the server.
 * 
 * The signature of the _Implementation functions is MANDATORY defined as follows:
 * Core::WorkOrderResult_t <NAME>(INPUT_PARAMS, OUTPUT_PARAMS)
 * With input params the params that are external to the executable
 * and received via the autobahn WAMP implementation (params of the RPC if you will). 
 * OUTPUT_PARAMS are the return values sent back to the WAMP router as reply tot the RPC call.
 * Additionally:
 * INPUT_PARAMS can be passed in by value or const lvalue-references. CONST IS REQUIRED.
 * OUTPUT_PARAMS must be lvalue-references and cannot be const qualified
 * The reasons for the above requirement can be found at method 'ParseAndExecuteWAMP_RPC'.
 * 
 */
class RPC_i
{
public:
  RPC_i();
  RPC_i& operator=(RPC_i const&) = delete;
  RPC_i& operator=(RPC_i&&) = delete;
  RPC_i(RPC_i const&) =delete;
  RPC_i(RPC_i&&) = delete;
  
  virtual ~RPC_i() { }
  virtual Core::WorkOrderResult_t AddUser_Implementation(std::string const&  UserName); ///< Add new user to the lobby.
  Core::WorkOrderResult_t GetUsers_Implementation(std::set<User_t>& Users); ///< Get users currently in lobby.
  virtual void StartListeningOnInterface(std::string&& LocalInterface, uint16_t Port) = 0;
private:
  Lobby_t m_Lobby;
};

/*! \brief Google RPC implementation of RPC_i
 *
 */
class GRPC_t : public RPC_i, public comm::Server::Service
{
public:
  GRPC_t();
  virtual ~GRPC_t();
  ::grpc::Status AddUser(::grpc::ServerContext*, const ::comm::User*, ::google::protobuf::Empty*) override;
  virtual void StartListeningOnInterface(std::string&&, uint16_t) override;
};

/*! \brief Web Application Messaging Protocol implementation of RPC_i
 *
 *  In addition to RPC, WAMP also supports pub&sub messages.
 *  clients can subscribe to a topic (STC.Server.*) and receive all publications
 *  of the STC_Server;
 *  This avoids polling STC_Server over and over again for updates and avoids complex code
 *  to deal with various different update replies.
 * 
 *  Thanks WAMP!
 * 
 */
class WAMP_t : public RPC_i
{
public:
  WAMP_t();
  WAMP_t(WAMP_t&&) = delete;
  WAMP_t(WAMP_t const&) = delete;
  WAMP_t operator= (WAMP_t const&) = delete;
  WAMP_t operator= (WAMP_t&&) = delete;
  virtual ~WAMP_t();
  
  //RPC_i:
  virtual void StartListeningOnInterface(std::string&&, uint16_t) override;
  virtual Core::WorkOrderResult_t AddUser_Implementation(std::string const&  UserName) override; ///< Add new user to the lobby.
    
  // WAMP specific only; publish to subscribers; Not supported by RPC
  // 
  // MAY ONLY BE CALLED FROM WAMP_t MEMBER FUNCTIONS.
  //
  void PublishUser(bool Added, std::string const& UserName); ///< Publish to clients that a new user joined.
 
  
  inline std::shared_ptr<autobahn::wamp_session> GetSession(){ return m_Session;}
  inline boost::asio::io_service& GetIO(){return m_IO;}
private:
  std::shared_ptr<autobahn::wamp_session> m_Session; ///< Has to be shared ptr ; passed as parameter into autobahn framework.
  boost::asio::io_service m_IO;

};
#endif /* RPC_H */

