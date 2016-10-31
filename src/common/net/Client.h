/*! \file */
/*
 * Client.h
 *
 *  Created on: Oct 23, 2016
 *      Author: yoko
 */

#ifndef SRC_NET_CLIENT_H_
#define SRC_NET_CLIENT_H_
#include <boost/asio.hpp>
#include <string>
#include "IConnectable.h"

namespace Net {

/*! \brief Client like entity which connects to another remote client.
 *
 *	An object of type CClient will connect to the remote client which has a server (hopefully) listening on the specified end address.
 *
 */
class CClient : IConnectable{
public:
	CClient(boost::asio::io_service& IO_Service, unsigned short const Port, std::string&& IpAddress);
	virtual ~CClient();

	//IConnectable interface
	virtual void Open() override;
	virtual void Send() override;
	virtual void Close() override;

private:
	unsigned short m_DestinationPort; ///< Remote server port
	std::string m_DestinationIp; ///< Remote server IP as string (e.g. 1.1.1.1 )
	boost::asio::ip::tcp::socket m_Socket;

	void connect_accepted();
	void connect_refused();



};
} /* namespace Net */

#endif /* SRC_NET_CLIENT_H_ */
