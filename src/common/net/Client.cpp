/*! \file */
/*
 * Client.cpp
 *
 *  Created on: Oct 23, 2016
 *      Author: yoko
 */

#include "Client.h"
#include "common/net/Session.h"
#include <iostream>
namespace Net
{
/*! \brief Client constructor
 *
 *	\param IO_Service
 *	\param Port Remote server port
 *	\param IpAddress Remote server IP as string (e.g. 1.1.1.1 )
 *
 */
CClient::CClient(boost::asio::io_service& IO_Service, unsigned short const Port, std::string&& IpAddress):
		m_Socket(IO_Service),
		m_DestinationPort(Port),
		m_DestinationIp(std::forward<std::string>(IpAddress))
{

}

CClient::~CClient()
{
	// TODO Auto-generated destructor stub
}

/*! \brief Async open Connect to server
 *
 *	\note Will call callback connect_refused when connection was refused by remote host, else connect_accepted();
 *
 */
void CClient::Open()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_DestinationIp), m_DestinationPort);
	m_Socket.async_connect(endpoint,
		[&](const boost::system::error_code& error)
		{

			if(error)
			{
				connect_refused();
			}
			else
			{
				connect_accepted();
			}
		}
	);
}
void CClient::Send()
{
}
void CClient::Close()
{
}

/*! \brief Called when attempted connection was successful
 *
 *
 */
void CClient::connect_accepted()
{
	std::make_shared<CSession>(std::move(m_Socket))->Start();
}

/*! \brief Called when attempted connection was not successful
 *
 *
 */
void CClient::connect_refused()
{
	std::cout << "Connection refused"<< std::endl;
}

} /* namespace Net */
