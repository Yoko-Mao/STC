#pragma once

#include "server/communication/database/Database.h"
#include "server/communication/rpc/RPC.h"

/*! \brief Database access provider using WAMP
 *
 *  The Database calls are forwarded to the database service which does the
 *  the actual queries on the database. This service is running in a separate
 *  process also connected to the WAMP router.
 * 
 */
class DatabaseWAMP_t:public Database_i
{
public:
  DatabaseWAMP_t(WAMP_t& WAMP):m_WAMP(WAMP){}
  DatabaseWAMP_t& operator=(DatabaseWAMP_t const&) = delete;
  DatabaseWAMP_t& operator=(DatabaseWAMP_t&&) = delete;
  DatabaseWAMP_t(DatabaseWAMP_t const&) =delete;
  DatabaseWAMP_t(DatabaseWAMP_t&&) = delete;  
  virtual ~DatabaseWAMP_t() { }
  
  virtual boost::optional<User_t> Read(std::string const& UserName) override;
  virtual bool Insert(User_t const&);
  virtual void Update(User_t const&) {}
  virtual void Delete(std::string const& UserName) {}
  virtual std::set<User_t> ReadAll() { return std::set<User_t>();}
  
  WAMP_t& m_WAMP; ///< Reference to some WAMP object used to execute DB calls.
};

