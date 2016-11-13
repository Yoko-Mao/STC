#include "client/RPC.h"
#include <thread>

/*! \brief Connect to central server;
 *
 * \param EndPoint Address of the server
 * \returns OK if connection succeeded; failure if connection with server could not be established.
 */
::grpc::Status CClientRPC_Impl::ConnectToServer(::grpc::ServerContext* context, const ::comm::EndPoint* EndPoint, ::google::protobuf::Empty* Response)
{
  std::cout<<"Received server connection: "<<EndPoint->m_ip()<<std::endl;
  auto EndPointStr = EndPoint->m_ip() + ":" + std::to_string(EndPoint->m_port());
  auto TmpUniquePtr = std::unique_ptr<CServerStub>(new CServerStub(grpc::CreateChannel(EndPointStr, grpc::InsecureChannelCredentials())));
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_ServerStub= std::move(TmpUniquePtr);
  }
  return m_ServerStub->ConnectToServer("william");
}

/*! \brief Returns ptr to server stub if created.
 *
 *  \returns Returns null if server not created and logs error, else returns server obj
 */
CServerStub* CClientRPC_Impl::ReturnServerIfExists()
{
  if (!m_ServerStub) {
    //Log error here
  }
  return m_ServerStub.get();
}

/*! \brief Starts the local RPC server.
 *
 *  \param LocalPort port on which the RPC client listens on the local machine
 *  \note This blocks the calling thread!!
 */
void CClientRPC_Impl::StartLocalRPC_Client(unsigned short LocalPort)
{
  CClientRPC_Impl ClientRPC_Impl;

  // Listen on the given address without any authentication mechanism.
  auto ServerBuilder = ::grpc::ServerBuilder();
  ServerBuilder.AddListeningPort("0.0.0.0:"+std::to_string(LocalPort), grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  ServerBuilder.RegisterService(&ClientRPC_Impl);
  // Finally assemble the server.
  auto server = std::unique_ptr<::grpc::Server>(ServerBuilder.BuildAndStart());

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

