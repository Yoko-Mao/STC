/*
 * ConnectionManager.h
 *
 *  Created on: Oct 26, 2016
 *      Author: yoko
 */

#ifndef SRC_COMMON_NET_CONNECTIONMANAGER_H_
#define SRC_COMMON_NET_CONNECTIONMANAGER_H_
#include <vector>
#include <memory>
#include "common/net/IConnectable.h"
namespace Net {

class CConnectionManager {
public:
	CConnectionManager();
	virtual ~CConnectionManager();

private:
	std::vector<std::unique_ptr<IConnectable>> m_Connectables;
};

} /* namespace Net */

#endif /* SRC_COMMON_NET_CONNECTIONMANAGER_H_ */
