/*
 * IConnectable.h
 *
 *  Created on: Oct 26, 2016
 *      Author: yoko
 */

#ifndef SRC_COMMON_NET_ICONNECTABLE_H_
#define SRC_COMMON_NET_ICONNECTABLE_H_

namespace Net {

class IConnectable
{
public:
	virtual void Send() = 0;  ///< Send data to all (if plural possible) connected endpoints.
	virtual void Open() = 0;  ///< Depending implementation this may open a connection or start listening for connections.
	virtual void Close() = 0; ///< Close connection to any connected endpoints and halt open().

};

} /* namespace Net */

#endif /* SRC_COMMON_NET_ICONNECTABLE_H_ */
