#ifndef SERVER_H
#define SERVER_H

#include "server/communication/rpc/RPC.h"
#include "server/communication/database/DatabaseWAMP.h"
#include "server/state/State.h"
#include "server/communication/Communication.h"

class Server_i
{
public:
  Server_i(CommunicationBase_t& Communication):m_Communication(Communication), m_Lobby(m_Communication){}
  Server_i& operator=(Server_i const&) = delete;
  Server_i& operator=(Server_i&&) = delete;
  Server_i(Server_i const&) =delete;
  Server_i(Server_i&&) = delete;
protected:
  CommunicationBase_t& m_Communication;
  State_t m_Lobby;
};


class ServerWAMP_t : public Server_i
{
public:
  ServerWAMP_t(): Server_i(m_CommunicationWAMP), m_CommunicationWAMP(m_Lobby){}
  ServerWAMP_t& operator=(Server_i const&) = delete;
  ServerWAMP_t& operator=(Server_i&&) = delete;
  ServerWAMP_t(Server_i const&) =delete;
  ServerWAMP_t(Server_i&&) = delete;
private:
  CommunicationWAMP_t m_CommunicationWAMP;
};

#endif /* SERVER_H */

