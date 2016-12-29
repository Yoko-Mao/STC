#pragma once
#include "server/communication/rpc/RPC.h"
#include "server/communication/database/Database.h"
#include "server/communication/database/DatabaseWAMP.h"
#include "server/communication/CommunicationAbstract.h"


/*! \brief Base class for communication implementations
 *
 *  Communication proxy objects have to inherit from the base class
 *  and implement the pure virtual functions.
 * 
 */
class CommunicationBase_t : public Communication_i
{
public:
  CommunicationBase_t(RPC_i& RPC, Database_i& Database):m_RPC(RPC), m_Database(Database){  m_RPC.StartListeningOnInterface("127.0.0.1",8000);}
  CommunicationBase_t& operator=(CommunicationBase_t const&) = delete;
  CommunicationBase_t& operator=(CommunicationBase_t&&) = delete;
  CommunicationBase_t(CommunicationBase_t const&) =delete;
  CommunicationBase_t(CommunicationBase_t&&) = delete;  
  virtual ~CommunicationBase_t() { }
  
  RPC_i& GetRPC() { return m_RPC;}
  Database_i& GetDatabase(){ return m_Database;}
private:
  RPC_i& m_RPC;
  Database_i& m_Database;
};

/*! \brief WAMP implementation of communication proxy object.
 * 
 */
class CommunicationWAMP_t : public CommunicationBase_t
{
public:
  CommunicationWAMP_t(State_i& StateForCommunication): CommunicationBase_t(m_RPC_WAMP, m_DatabaseWAMP), m_RPC_WAMP(StateForCommunication), m_DatabaseWAMP(m_RPC_WAMP){}
  CommunicationWAMP_t& operator=(CommunicationWAMP_t const&) = delete;
  CommunicationWAMP_t& operator=(CommunicationWAMP_t&&) = delete;
  CommunicationWAMP_t(CommunicationWAMP_t const&) =delete;
  CommunicationWAMP_t(CommunicationWAMP_t&&) = delete;  
  virtual ~CommunicationWAMP_t() { }
  
private:
  WAMP_t m_RPC_WAMP;
  DatabaseWAMP_t m_DatabaseWAMP;
};
