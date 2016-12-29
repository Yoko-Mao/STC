#pragma once
#include "server/state/User.h"
#include <boost/optional.hpp>

/*! \brief Abstract representation of database access.
 *
 *  Database access providers have to implement this interface.
 * 
 */
class Database_i
{
public:
  Database_i() {}
  Database_i& operator=(Database_i const&) = delete;
  Database_i& operator=(Database_i&&) = delete;
  Database_i(Database_i const&) =delete;
  Database_i(Database_i&&) = delete;  
  virtual ~Database_i() { }
  
  virtual boost::optional<User_t> Read(std::string const& UserName) = 0;
  virtual bool Insert(User_t const&) = 0;
  virtual void Update(User_t const&) = 0;
  virtual void Delete(std::string const& UserName) = 0;
  virtual std::set<User_t> ReadAll() = 0;
  
};



