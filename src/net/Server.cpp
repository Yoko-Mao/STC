/*
 * Adapter.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: yoko
 */

#include <memory>
#include <utility>

#include "Server.h"
#include "Session.h"
namespace Net {

CServer::CServer(boost::asio::io_service& IO_Service, unsigned short const Port):
		m_Acceptor(IO_Service, tcp::endpoint(tcp::v4(), Port)),
		m_Socket(IO_Service)
{
	Accept();
}

CServer::~CServer()
{
	// TODO Auto-generated destructor stub
}

void CServer::Accept()
{
  m_Acceptor.async_accept(m_Socket,
      [this](boost::system::error_code ec)
      {
        if (!ec)
        {
        	std::make_shared<CSession>(std::move(m_Socket))->Start();
        }

        Accept();
      });
}



} /* namespace Net */
