#ifndef STC_RPC_H
#define STC_RPC_H
//#include <grpc++/grpc++.h>
//#include <extcomm.grpc.pb.h>
//#include <intcomm.grpc.pb.h>
//class CServerStub
//{
//public:
//    CServerStub(std::shared_ptr<::grpc::Channel> channel): m_Stub(::comm::InternalCommunication::NewStub(channel)) {}
//
//    ::grpc::Status ConnectToServer(const std::string& user)
//    {
//      std::cout << "Internal Connect To Server" << std::endl;
//      ::comm::ClientIdentification ClientIdentification;
//      ::grpc::ClientContext ClientContext;
//      ::google::protobuf::Empty Empty;
//      return m_Stub->ConnectToServer(&ClientContext, ClientIdentification, &Empty);
//    }
//    ::grpc::Status GetGames()
//    {
//      ::grpc::ClientContext ClientContext;
//      ::google::protobuf::Empty Empty;
//      ::comm::GameList Games;
//      return m_Stub->GetGames(&ClientContext, Empty, &Games);
//    }
//    ::grpc::Status AddGame(std::string const & Name)
//    {
//      ::grpc::ClientContext ClientContext;
//      ::comm::Game Game;
//      Game.set_m_name(Name);
//      ::google::protobuf::Empty Empty;
//      return m_Stub->AddGame(&ClientContext,  Game, &Empty);
//    }
//
//private:
//    std::unique_ptr<::comm::InternalCommunication::Stub> m_Stub;
//};
//
///*! \brief Implementation of Client side RPC calls
// *
// */
//class CClientRPC_Impl final : public comm::ExternalCommunication::Service
//{
//    std::unique_ptr<CServerStub> m_ServerStub; ///< Server stub which has its lifetime managed by the RPC object its lifetime.
//    std::mutex m_Mutex; ///< Mutex to lock resources from all different IO threads.
//    CServerStub* ReturnServerIfExists(); ///< Helper function that returns ptr to ServerStub if it exists (connection established).
//public:
//    static void StartLocalRPC_Client(unsigned short LocalPort);
//    ::grpc::Status ConnectToServer(::grpc::ServerContext *context, const ::comm::EndPoint *EndPoint, ::google::protobuf::Empty *response) override;
//};

#endif //STC_RPC_H
