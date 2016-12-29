/*! \file */
#include <iostream>
#include "common/core/utilities/CommandLine.h"
#include "server/Server.h"

namespace po = boost::program_options;

namespace
{

/*! \brief Server specific Commandline impl.
 *
 */
class CommandLineArguments_t : public Core::CommandLineArgumentsBase_t
{
public:

  CommandLineArguments_t(int argc, char* argv[]) : Core::CommandLineArgumentsBase_t(argc, argv)
  {

  }

  void SetDescription() override
  {
    m_Description.add_options()("help", "produce help message")
      ("local-rpc-port", po::value < unsigned short > (&m_LocalRPC_Port), "Set local RPC port")
      ("remote-server-port", po::value<unsigned short> (&m_ServerPort), "Set remote server port");
  }
  unsigned short m_ServerPort = 8000; ///< Port on which to listen for incoming client connections.
};
}

/*! \brief Application entry point.
 *
 *	\param argc Amount of commandline arguments
 *	\param argv The commandline parameters
 *
 *	\note Enter parameter "--help" to display list of parameters that can be entered.
 */
int main(int argc, char *argv[])
{
  std::cout << "Launching Server" << std::endl;
  auto Arguments = CommandLineArguments_t(argc, argv);
  // Error occured while parsing command line arguments. Proper error should have been dumped already.
  if (Arguments.Parse())
  {
    return 1;
  }
  ServerWAMP_t Server;
  return 0;
}