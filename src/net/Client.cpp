/*
 * Client.cpp
 *
 *  Created on: Oct 23, 2016
 *      Author: yoko
 */

#include "Client.h"

namespace Net {
void connect_handler(const boost::system::error_code& error)
{
  if (!error)
  {
    // Connect succeeded.
  }
}

CClient::CClient(boost::asio::io_service& IO_Service, unsigned short const Port, std::string const & IpAddress): m_Socket(IO_Service)
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(IpAddress), Port);
	m_Socket.async_connect(endpoint, connect_handler);
}

CClient::~CClient()
{
	// TODO Auto-generated destructor stub
}

} /* namespace Net */
