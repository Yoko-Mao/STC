/*
 * Session.h
 *
 *  Created on: Oct 17, 2016
 *      Author: yoko
 */

#ifndef SRC_NET_SESSION_H_
#define SRC_NET_SESSION_H_
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Net {

//Represents a communication
class CSession : public std::enable_shared_from_this<CSession> {
public:
	CSession(tcp::socket&& socket);
	virtual ~CSession();
	void Start();
private:
	void Read();
	void Write(size_t length);
	tcp::socket m_Socket;
	static const uint32_t MAX_LENGTH = 1024;
	char m_Data[CSession::MAX_LENGTH];
};

} /* namespace Net */

#endif /* SRC_NET_SESSION_H_ */
