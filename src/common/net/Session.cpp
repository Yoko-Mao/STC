/*
 * Session.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: yoko
 */

#include "Session.h"
#include <iostream>

namespace Net {

CSession::CSession(tcp::socket&& socket):
	m_Socket(std::move(socket))
{
}

CSession::~CSession()
{
}

void CSession::Read()
{
	std::shared_ptr<CSession> self(shared_from_this());
    m_Socket.async_read_some(boost::asio::buffer(m_Buffer),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
//                Msg msg;
//                for (::google::protobuf::Any const& detail : msg.details())
//                {
//                    if (detail.Is<OpenConnection>())
//                    {
//                        OpenConnection OpenConnectionMsg;
//                        detail.UnpackTo(&OpenConnectionMsg);
//                        std::cout<<"Received Open Connection Msg in CSession::Read():"<<OpenConnectionMsg.m_Ip();
//
//                    }
//                }
                Write(length);
            }
        });
}

void CSession::Write(size_t length)
{
	std::shared_ptr<CSession> self(shared_from_this());
    boost::asio::async_write(m_Socket, boost::asio::buffer(m_Buffer, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            Read();
          }
        });
}

void CSession::Start()
{

}

} /* namespace Net */

