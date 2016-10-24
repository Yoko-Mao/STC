/*! \file */

/*
 * Server.h
 *
 *  Created on: Oct 17, 2016
 *      Author: yoko
 */

#ifndef SRC_NET_SERVER_H_
#define SRC_NET_SERVER_H_
#include <boost/asio.hpp>
#include "common/net/IConnectable.h"

using boost::asio::ip::tcp;
namespace Net {

/*! \brief Server like entity which listens for incoming connections.
 *
 *	An object of type CServer will listen on some port passed to the constructor and accept connections
 *	asynchronously.
 *
 */
class CServer: public IConnectable
{
public:
	CServer(boost::asio::io_service& IO_Service, unsigned short const Port);
	virtual ~CServer();
	virtual void Close(){}
	virtual void Open(){}
	virtual void Send(){}
private:

	tcp::acceptor m_Acceptor;
	tcp::socket m_Socket;

	void Accept();

};

} /* namespace Net */

#endif /* SRC_NET_SERVER_H_ */
