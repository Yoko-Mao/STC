#include "server/communication/database/DatabaseWAMP.h"
#include <iostream>

bool DatabaseWAMP_t::Insert(User_t const& User)
{
  bool RetValue = true;
  auto c1 = m_WAMP.GetSession()->call("com.example.mul2", std::tuple<int>(3))
      .then([&](boost::future<autobahn::wamp_call_result> result) {
          try {
              uint64_t product = result.get().argument<uint64_t>(0);
              std::cerr << "call succeeded with result: " << product << std::endl;
          } catch (const std::exception& e) {
              std::cerr << "call failed: " << e.what() << std::endl;
          }
      }
  );

  return RetValue;
}

boost::optional<User_t> DatabaseWAMP_t::Read(std::string const& UserName)
{ 
  boost::optional<User_t> OptionalUser(boost::none);
  auto c1 = m_WAMP.GetSession()->call("stc.database.users.read", std::tuple<std::string>(UserName))
      .then([&](boost::future<autobahn::wamp_call_result> result) {
          try {
              OptionalUser = result.get().argument<User_t>(0);
          } catch (const std::exception& e) {
              std::cerr << "call failed: " << e.what() << std::endl;
          }
      }
  );

  return OptionalUser;
}