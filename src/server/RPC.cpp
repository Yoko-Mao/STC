#include "RPC.h"
#include <tuple>
#include <functional>
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
 *  \return WorkOrderResult that indicates whether or not the work was successfull.
 * 
 */
Core::WorkOrderResult_t RPC_i::AddUser_Implementation(std::string const& UserName)
{
  auto Optional = m_Lobby.AddUser_Threadsafe(UserName);

  return (Optional) ? Optional->get() : Core::WorkOrderResult_t(Core::WorkOrderResult_t::ErrorCode_t::ERROR);
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
 *  The RPC server will listen on the combination Ip+Port. If multiple NICs/IPs
 *  are available the Ip param may be relevant but if hosting the server on lo
 *  this should be 0.0.0.0.
 * 
 *  \param Ip Ip onto which to listen for incoming RPC's; This will probably be 0.0.0.0 if localhost.
 *  \param Port Port onto which to listen for incoming RPC's.
 * 
 * 
 */
void GRPC_t::StartListeningOnInterface(std::string&& Ip, uint16_t Port)
{
  // Listen on the given address without any authentication mechanism.
  auto ServerBuilder = ::grpc::ServerBuilder();
  ServerBuilder.AddListeningPort(Ip+":"+std::to_string(Port), grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  ServerBuilder.RegisterService(this);
  // Finally assemble the server.
  auto server = std::unique_ptr<::grpc::Server>(ServerBuilder.BuildAndStart());

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}
namespace
{ 
  template<typename F, typename T, std::size_t... I>
  decltype(auto) CallFunctionWithTuple_Impl(RPC_i* Obj, F f,T t, std::index_sequence<I...>)
  {
    return (Obj->*f)(std::get<I>(t)...);
  }

  template<typename F, typename... T>
  decltype(auto) CallFunctionWithTuple(RPC_i* Obj, F f,const std::tuple<T...>& a)
  {
    auto Indices = std::make_index_sequence<sizeof...(T)>();
    return CallFunctionWithTuple_Impl(Obj, f, a, Indices);
  }

  /*! \brief Extract parameters from Autobahn interface and call required implementation function.
   * 
   *  The point of this function is to easily implement new WAMP RPCs.
   *  ParseAndExecuteWAMP_RPC should be called from the entry point of the RPC that is automatically 
   *  called by autobahn when a call is received.
   *  The required implementation function is supplied from the RPC entry point as parameter "Function".
   *  This function is executed with the parameters passed by the RPC (extracted from the Invocation).
   *  
   *  The key idea is that the implementation function its definition defines the data that 
   *  should've been sent with the RPC. For each parameter of that function a data member should be present
   *  in the Invocation.
   * 
   *  This will:
   *    - Send an Error result back if not all required arguments are present in the Invocation.
   *    - Send the result of the implementation back after it was handled.
   * 
   *  This is not a member function WAMP_t because I wanted not to bloat the header file.
   * 
   *  \note The fact that the member is passed as templated pointer basically disables overloads 
   *  for that member function (to avoid disambiguation). 
   *  
   *  \note This function blocks until the scheduled work is performed.
   * 
   *  \param Invocation Interface object to Autobahn library.
   *  \param Obj Object that owns the RPC all to execute.
   *  \param Function Implementation of RPC to execute.
   * 
   * 
   */
  template<typename... Args_t>
  void ParseAndExecuteWAMP_RPC(autobahn::wamp_invocation Invocation, RPC_i* Obj, Core::WorkOrderResult_t(RPC_i::*Function)(Args_t...))
  {
    // std::decay is required because the member function params may use references or other qualifiers in its function definition.
    std::tuple<std::decay_t<Args_t>...> tuple; 
  
    // Extract parameters that were passed by the RPC and put them in the tuple
    try
    {
      Invocation->get_arguments(tuple);
    }
    catch (const std::bad_cast& e)
    {
      Invocation->result(std::make_tuple(static_cast<uint32_t>(Core::WorkOrderResult_t::ErrorCode_t::ERROR)));
      return;
    }
    
    //Call implementation function with params
    Core::WorkOrderResult_t Result = CallFunctionWithTuple(Obj, Function, tuple);
    
    Invocation->result(std::make_tuple(static_cast<uint32_t>(Result.GetErrorCode())));
    
  }
}

/*! \brief WAMP RPC wrapper for AddUserImpl.
 * 
 *  After parsing arguments passed by Autobahn; calls 'AddUser_Implementation' with correct arguments.
 *  
 *  \param Invocation Interface object to Autobahn library.
 * 
 * 
 */
void WAMP_t::AddUser(autobahn::wamp_invocation Invocation)
{  
  ParseAndExecuteWAMP_RPC(Invocation, this, &RPC_i::AddUser_Implementation);
}

/*! \brief Connect to the WAMP server
 * 
 *  Connect to the WAMP router and register all remote procedure calls that
 *  clients may call.
 * 
 *  \param Ip Ip onto which to listen for incoming RPC's; Probably 127.0.0.1.
 *  \param Port Port onto which to listen for incoming RPC's.
 * 
 * 
 */
void WAMP_t::StartListeningOnInterface(std::string&& Ip, uint16_t Port)
{

  try
  {
    // ASIO service object
    //
    boost::asio::io_service io;


    const std::string realm = "realm1";
    bool debug = false;
    const boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(Ip), Port);
    auto transport = std::make_shared<autobahn::wamp_tcp_transport>(io, endpoint, debug);
    auto session = std::make_shared<autobahn::wamp_session>(io, debug);
    transport->attach(std::static_pointer_cast<autobahn::wamp_transport_handler>(session));

    // Make sure the continuation futures we use do not run out of scope prematurely.
    // Since we are only using one thread here this can cause the io service to block
    // as a future generated by a continuation will block waiting for its promise to be
    // fulfilled when it goes out of scope. This would prevent the session from receiving
    // responses from the router.
    boost::future<void> connect_future;
    boost::future<void> start_future;
    boost::future<void> join_future;
    boost::future<void> provide_future_add;
    boost::future<void> provide_future_longop;

    connect_future = transport->connect().then([&](boost::future<void> connected)
    {
      try
      {
        connected.get();
      } catch (const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        io.stop();
        return;
      }

      std::cerr << "transport connected" << std::endl;

      start_future = session->start().then([&](boost::future<void> started)
      {
        try
        {
          started.get();
        } catch (const std::exception& e)
        {
          std::cerr << e.what() << std::endl;
          io.stop();
          return;
        }

        std::cerr << "session started" << std::endl;

        join_future = session->join(realm).then([&](boost::future<uint64_t> joined)
        {
          try
          {
            std::cerr << "joined realm: " << joined.get() << std::endl;
          } catch (const std::exception& e)
          {
            std::cerr << e.what() << std::endl;
            io.stop();
            return;
          }

          provide_future_add = session->provide("com.server.adduser", [&](autobahn::wamp_invocation e)
          {
            this->AddUser(e);
          }).then(
            [&](boost::future<autobahn::wamp_registration> registration)
            {
              try
              {
                std::cerr << "registered procedure:" << registration.get().id() << std::endl;
              } catch (const std::exception& e)
              {
                std::cerr << e.what() << std::endl;
                io.stop();
                return;
              }
            });
        });
      });
    });

    std::cerr << "starting io service" << std::endl;
    io.run();
    std::cerr << "stopped io service" << std::endl;
  } catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
//          // start the WAMP session on the transport that has been connected
//          //
//          start_future = session->start().then([&](boost::future<bool> started)
//          {
//            if (started.get())
//            {
//              std::cerr << "session started" << std::endl;
//
//              // join a realm with the WAMP session
//              //
//              join_future = session->join(realm).then([&](boost::future<uint64_t> s)
//              {
//                std::cerr << "joined realm: " << s.get() << std::endl;
//
//                // SUBSCRIBE to a topic and receive events
//                //
//                session->subscribe("com.example.onhello",
//                  [](const autobahn::wamp_event & event)
//                  {
//                    std::cerr << "event for 'onhello' received: " << event.argument<std::string>(0) << std::endl;
//                  }
//                );
//
//                // REGISTER a procedure for remote calling
//                //
//                session->provide("com.example.add2", &add2);
//              } catch (std::exception& e)
//              {
//                std::cerr << e.what() << std::endl;
//              }
//
//            }