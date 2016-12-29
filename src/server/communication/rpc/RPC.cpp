#include "RPC.h"
#include "server/state/StateAbstract.h"
#include <tuple>
#include <vector>
#include <map>
#include <functional>

RPC_i::RPC_i(State_i& State) :
m_State(State)
{

}

WAMP_t::WAMP_t(State_i& State): 
RPC_i(State)
{
}

WAMP_t::~WAMP_t()
{
}

/*! \brief Remove a user from the server.
 * 
 *  Schedules a work item that makes the lobby remove an existing user.
 *  This is a blocking call that will return when the user was removed or the lobby
 *  failed to remove the user. The user did not exist.
 * 
 *  \param UserName The name of the existing user that should be removed
 * 
 *  \return WorkOrderResult that indicates whether or not the work was successfull.
 * 
 */
Core::WorkOrderResult_t RPC_i::RemoveUser_Implementation(std::string const& UserName)
{
  return m_State.RemoveUser(UserName);
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
  return m_State.AddUser(UserName);
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
  return m_State.GetUsers(Users);
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
 *  This is for incomming RPCs only.
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
          RegisterRPC("STC.Lobby.Users.Remove", &RPC_i::RemoveUser_Implementation, *this);
          RegisterRPC("STC.Lobby.Users.Get", &RPC_i::GetUsers_Implementation, *this);
          
          boost::future<void> future = m_Session->provide("stc.lobby.users.authenticate", [&](autobahn::wamp_invocation Invocation)
          {
            std::cerr << "Authenticate"<<std::endl;
            
            std::map<std::string, std::string> Map;
            Map.insert(std::pair<std::string, std::string>("secret", "dev"));
            Map.insert(std::pair<std::string, std::string>("role", "dev"));
            Invocation->result(Map);
            //Invocation->result()
            //Invocation->
          }).then(
            [&](boost::future<autobahn::wamp_registration> registration)
            {
              try
              {
                registration.get();
              } catch (const std::exception& e)
              {
                std::cerr << e.what() << std::endl;
                m_IO.stop();
                return;
              }
            });

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

/*! \brief Remove existing user from the lobby.
 * 
 *  Calls base implementation and if user was removed the user removal is published.
 * 
 *  \param UserName The name of the user that should be removed.
 * 
 *  \return WorkOrderResult that indicates whether or not the work was successfull.
 * 
 */
Core::WorkOrderResult_t WAMP_t::RemoveUser_Implementation(std::string const& UserName)
{
  Core::WorkOrderResult_t Result = RPC_i::RemoveUser_Implementation(UserName);
  
  if (Result.IsSuccessfull())
  {
    PublishUser(false, UserName);
  }
  
  return Result;
  
}