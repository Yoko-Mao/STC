#include "RPC.h"
#include <tuple>
#include <functional>

GRPC_t::GRPC_t()
{
}

GRPC_t::~GRPC_t()
{
}

RPC_i::RPC_i() :
m_Lobby()
{

}

WAMP_t::WAMP_t()
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
Core::WorkOrderResult_t RPC_i::GetUsers_Implementation(std::set<User_t>& Users)
{
  auto Optional = m_Lobby.GetUsers_Threadsafe(Users);

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
  return ::grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "POGO STICK");
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
  ServerBuilder.AddListeningPort(Ip + ":" + std::to_string(Port), grpc::InsecureServerCredentials());
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
template <typename T>
struct is_lvalue_reference_and_not_const : std::integral_constant<bool, std::is_lvalue_reference<T>::value && !std::is_const<typename std::remove_reference<T>::type>::value>
{
};

template <typename T>
using is_output_type = is_lvalue_reference_and_not_const<T>;

template <typename T>
using is_input_type = std::integral_constant<bool, !is_output_type<T>::value>;

template <template <typename> typename FilterFunction, typename TypesToFilter, typename TypesLeftAfterFiltering = std::tuple<>>
struct Filter;

template <template <typename> typename FilterFunction, typename... TypesLeftAfterFiltering> 
struct Filter<FilterFunction, std::tuple<>, std::tuple<TypesLeftAfterFiltering...>>
{
  // std::decay is required because the member function params may use references or other qualifiers in its function definition.
  using Tuple = std::tuple<typename std::decay<TypesLeftAfterFiltering>::type...>;
};
template <template <typename> typename FilterFunction, typename Argument, typename... TypesToFilter, typename... TypesLeftAfterFiltering>
struct Filter <FilterFunction, std::tuple<Argument, TypesToFilter...>, std::tuple<TypesLeftAfterFiltering...> > : 
	Filter<
    FilterFunction,
    std::tuple<TypesToFilter...>,
    std::conditional_t<
        FilterFunction<Argument>::value,
        std::tuple<TypesLeftAfterFiltering..., Argument>,
        std::tuple<TypesLeftAfterFiltering...>
      >
    >
{
};

template<typename F, 
  typename InputTuple_t, 
  typename OutputTuple_t,
  std::size_t... InputTupleIndices, 
  std::size_t... OutputTupleIndices>
decltype(auto) CallFunctionWithTuple_Impl(RPC_i& Obj, F f, InputTuple_t& InputTuple, OutputTuple_t& OutputTuple, std::index_sequence<InputTupleIndices...>, std::index_sequence<OutputTupleIndices...>)
{
  return (Obj.*f)(std::get<InputTupleIndices>(InputTuple)..., std::get<OutputTupleIndices>(OutputTuple)...);
}

template<typename F, 
  typename... InputTupleTypes, 
  typename... OutputTupleTypes>
decltype(auto) CallFunctionWithTuple(RPC_i& Obj, F f, std::tuple<InputTupleTypes...>& InputTuple, std::tuple<OutputTupleTypes...>& OutputTuple)
{
  auto IndicesInputTuple = std::make_index_sequence<sizeof...(InputTupleTypes)>();
  auto IndicesOutputTuple = std::make_index_sequence<sizeof...(OutputTupleTypes)>();
  
  return CallFunctionWithTuple_Impl(Obj, f, InputTuple, OutputTuple, IndicesInputTuple, IndicesOutputTuple);
}

/*! \brief Extract parameters from Autobahn interface and call required implementation function.
 * 
 *  The point of this function is to easily implement new WAMP RPCs.
 *  ParseAndExecuteWAMP_RPC should be called from the entry point of the RPC.
 *  The entrypoint is the interface point between autobahn and STC. 
 *  It is automatically called by autobahn when a RPC has been decoded. (see 'RegisterRPC').
 * 
 *  The function 'Function' is executed with the parameters extracted from the Invocation.
 *  
 *  IMPORTANT:
 *  The key idea is that the implementation function its definition defines the data that 
 *  should've been sent with the RPC. For each input parameter of that function a data member should be present
 *  in the Invocation. Error will be returned if not exactly the correct arguments have been passed.
 *  For each output parameter a data member is sent back to the WAMP router.
 *  In order to be able determine what are input and what are output params, the signature of the 
 *  _Implementation functions is MANDATORY defined as follows: Core::WorkOrderResult_t <NAME>(INPUT_PARAMS, OUTPUT_PARAMS).
 *  Additionally:
 *  INPUT_PARAMS can be passed in by value or const lvalue-references. CONST IS REQUIRED (see is_input_type).
 *  OUTPUT_PARAMS must be lvalue-references and cannot be const qualified (see is_output_type).

 * 
 *  This will:
 *    - Send an Error result back if not all required arguments are present in the Invocation.
 *    - Send the result of the implementation back after it was handled.
 * 
 *  This is not a member function of WAMP_t to avoid header bloat.
 * 
 *  \note The fact that the member is passed as templated pointer basically disables overloads 
 *  for that function (to avoid disambiguation). 
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
void ParseAndExecuteWAMP_RPC(autobahn::wamp_invocation Invocation, RPC_i& Obj, Core::WorkOrderResult_t(RPC_i::*Function)(Args_t...))
{
  typename Filter<is_input_type, std::tuple<Args_t...> >::Tuple InputTuple;
  typename Filter<is_output_type, std::tuple<Args_t...> >::Tuple OutputTuple;

  // Extract parameters that were passed by the RPC and put them in the tuple
  try
  {
    Invocation->get_arguments(InputTuple);
  } catch (const std::bad_cast& e)
  {
    std::cout << "Error occured while parsing params: "<< e.what()<<std::endl;
    std::cout << "INFO: Expected " << std::tuple_size<decltype(InputTuple)>::value << " argument(s) got "<<Invocation->number_of_arguments()<<" from RPC."<<std::endl;
    Invocation->result(std::make_tuple(static_cast<uint32_t> (Core::WorkOrderResult_t::ErrorCode_t::ERROR)));
    return;
  }

  //Call implementation function with params
  Core::WorkOrderResult_t Result = CallFunctionWithTuple(Obj, Function, InputTuple, OutputTuple);
  auto ResultTuple = std::make_tuple(static_cast<uint32_t> (Result.GetErrorCode()));
  auto ResultTupleFinal = std::tuple_cat(ResultTuple, OutputTuple);
  
  Invocation->result(ResultTupleFinal);

}

/*! \brief Register a WAMP RPC
 * 
 *  Helper function to register a RPC at the WAMP router.
 * 
 *  \param Name Name of RPC to register (e.g. STC.Lobby.AddUser)
 *  \param Function Function that should be called when the RPC is received.
 *  \param WAMP WAMP instance for which the RPC is registered.
 * 
 *  \note Not a member function of WAMP_t to keep the header file clean.
 * 
 */
template<typename... Args_t>
void RegisterRPC(std::string const& Name, Core::WorkOrderResult_t(RPC_i::*Function)(Args_t...), WAMP_t& WAMP)
{
  boost::future<void> future = WAMP.GetSession()->provide(Name, [&, Function](autobahn::wamp_invocation Invocation)
  {
    ParseAndExecuteWAMP_RPC(Invocation, WAMP, Function);
  }).then(
    [&](boost::future<autobahn::wamp_registration> registration)
    {
      try
      {
        registration.get();
      } catch (const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        WAMP.GetIO().stop();
        return;
      }
    });
}

}

/*! \brief Connect to the WAMP server
 * 
 *  Connect to the WAMP router and register all RPCs that clients may call.
 * 
 *  \param Ip Ip onto which to listen for incoming RPC's; Probably 127.0.0.1.
 *  \param Port Port onto which to listen for incoming RPC's.
 * 
 *  \note Blocking call.
 */
void WAMP_t::StartListeningOnInterface(std::string&& Ip, uint16_t Port)
{
  try
  {
    const std::string realm = "realm1";
    bool debug = false;
    const boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(Ip), Port);
    auto transport = std::make_shared<autobahn::wamp_tcp_transport>(m_IO, endpoint, debug);
    
    //TODO:check with valgrind that no memory is leaked; Issue logged @ github under 'valgrind'.
    m_Session = std::make_shared<autobahn::wamp_session>(m_IO, debug);
    transport->attach(std::static_pointer_cast<autobahn::wamp_transport_handler>(m_Session));

    // Make sure the continuation futures we use do not run out of scope prematurely.
    // Since we are only using one thread here this can cause the io service to block
    // as a future generated by a continuation will block waiting for its promise to be
    // fulfilled when it goes out of scope. This would prevent the session from receiving
    // responses from the router.
    boost::future<void> connect_future;
    boost::future<void> start_future;
    boost::future<void> join_future;

    connect_future = transport->connect().then([&](boost::future<void> connected)
    {
      try
      {
        connected.get();
      } catch (const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        m_IO.stop();
        return;
      }

      std::cout << "Transport connected" << std::endl;

      start_future = m_Session->start().then([&](boost::future<void> started)
      {
        try
        {
          started.get();
        } catch (const std::exception& e)
        {
          std::cerr << e.what() << std::endl;
          m_IO.stop();
          return;
        }
        std::cout << "Session started" << std::endl;
        
        join_future = m_Session->join(realm).then([&](boost::future<uint64_t> joined)
        {
          try
          {
            std::cerr << "joined realm: " << joined.get() << std::endl;
          } catch (const std::exception& e)
          {
            std::cerr << e.what() << std::endl;
            m_IO.stop();
            return;
          }
          RegisterRPC("STC.Lobby.Users.Add", &RPC_i::AddUser_Implementation, *this);
          RegisterRPC("STC.Lobby.Users.Get", &RPC_i::GetUsers_Implementation, *this);

          std::cout << "RPCs registered" <<std::endl;
        });
      });
    });

    std::cerr << "Starting io service." << std::endl;
    m_IO.run();
    std::cerr << "Stopped io service." << std::endl;
  } catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

/*! \brief Publish user list update to subscribers
 * 
 *  Connect to the WAMP router and register all RPCs that clients may call.
 *  Connection to the WAMP router should be active; no checks are performed.
 *  Session should be dereferencable.
 * 
 *  \param Added Did the user leave or join the lobby?
 *  \param user User that joined or left the lobby.
 * 
 */
void WAMP_t::PublishUser(bool Added, std::string const& UserName)
{
  m_Session->publish("STC.Lobby.Users.Update", std::make_tuple(Added, UserName));
}

/*! \brief Add a new user to the lobby.
 * 
 *  Calls base implementation and if user could be added, the user add is published.
 * 
 *  \param UserName The name of the user that should be added
 * 
 *  \return WorkOrderResult that indicates whether or not the work was successfull.
 * 
 */
Core::WorkOrderResult_t WAMP_t::AddUser_Implementation(std::string const& UserName)
{
  Core::WorkOrderResult_t Result = RPC_i::AddUser_Implementation(UserName);
  
  if (Result.IsSuccessfull())
  {
    PublishUser(true, UserName);
  }
  
  return Result;
  
}