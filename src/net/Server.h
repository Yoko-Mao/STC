/*
 * Adapter.h
 *
 *  Created on: Oct 17, 2016
 *      Author: yoko
 */

#ifndef SRC_NET_SERVER_H_
#define SRC_NET_SERVER_H_
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
namespace Net {

// Server listening for incoming connections of external clients
// Need atleast 1 server running on each client to re
class CServer {
public:
	CServer(boost::asio::io_service& IO_Service, unsigned short const Port);
	virtual ~CServer();
private:

	tcp::acceptor m_Acceptor;
	tcp::socket m_Socket;

	void Accept();

};

} /* namespace Net */

#endif /* SRC_NET_SERVER_H_ */
