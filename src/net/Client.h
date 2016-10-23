/*
 * Client.h
 *
 *  Created on: Oct 23, 2016
 *      Author: yoko
 */

#ifndef SRC_NET_CLIENT_H_
#define SRC_NET_CLIENT_H_
#include <boost/asio.hpp>

namespace Net {

class CClient {
public:
	CClient(boost::asio::io_service& IO_Service, unsigned short const Port, std::string const& IpAddress);
	virtual ~CClient();
private:
	boost::asio::ip::tcp::socket m_Socket;
};
} /* namespace Net */

#endif /* SRC_NET_CLIENT_H_ */
