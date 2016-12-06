/*! \file */
#include <iostream>
#include "client/RPC.h"
#include "common/core/utilities/CommandLine.h"
#include "common/core/task/Thread.h"
namespace po = boost::program_options;

namespace {

    /*! \brief Client specific Commandline impl.
     *
     */
    class CommandLineArguments_t : public Core::CommandLineArgumentsBase_t {
    public:

        CommandLineArguments_t(int argc, char* argv[]) : Core::CommandLineArgumentsBase_t(argc, argv) {

        }

        void SetDescription() override {
            m_Description.add_options()("help", "produce help message")("local-rpc-port", po::value < unsigned short > (&m_LocalRPC_Port), "Set local RPC port")
                    ("remote-server-ip", po::value<std::string>(&m_ServerIp), "Set remote server ip")
                    ("remote-server-port", po::value<unsigned short> (&m_ServerPort), "Set remote server port");
        }
        unsigned short m_ServerPort = 8000; ///< Port on which remote game server listens for incomming connections
        std::string m_ServerIp = "127.0.0.1"; ///< Ip of remote game server; Default this is the localhost.
    };
}

/*! \brief Application entry point.
 *
 *  \param argc Amount of commandline arguments
 *  \param argv The commandline parameters
 *
 *  \note Enter parameter "--help" to display list of parameters that can be entered.
 */
int main(int argc, char *argv[]) {
    std::cout << "Launching client" << std::endl;
    auto Arguments = CommandLineArguments_t(argc, argv);
    // Error occured while parsing command line arguments. Proper error should have been dumped already.
    if (Arguments.Parse())
        return 1;


    CClientRPC_Impl::StartLocalRPC_Client(Arguments.m_LocalRPC_Port);
    return 0;
}
