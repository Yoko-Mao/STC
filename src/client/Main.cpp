/*! \file */
#include <iostream>
#include "boost/program_options.hpp"
#include "client/RPC.h"
namespace po = boost::program_options;

/*! \brief Parsed result of command line arguments
 *
 */
struct CCommandLineArguments
{
	bool m_ShouldTerminateApplication; ///< This flag is toggled as result of wrong commandline input, to indicate early termination
    unsigned short m_LocalRPC_Port = 50051; ///< Port on which local RPC clients listens for incommng connections.
	unsigned short m_ServerPort = 8000; ///< Port on wich game server listens for incomming connections
	std::string m_ServerIp = "127.0.0.1"; ///< Ip of game server;
};
CCommandLineArguments HandleCommandLineArguments(int, char*[]);

/*! \brief Application entry point.
 *
 *	\param argc Amount of commandline arguments
 *	\param argv The commandline parameters
 *
 *	\note Enter parameter "--help" to display list of parameters that can be entered.
 */
int main(int argc, char *argv[])
{
	std::cout << "Launching client" << std::endl;
 	CCommandLineArguments Arguments = HandleCommandLineArguments(argc, argv);

	// Error occured while parsing command line arguments. Proper error should've been dumped already.
	if (Arguments.m_ShouldTerminateApplication)
		return 1;

	CClientRPC_Impl::StartLocalRPC_Client(Arguments.m_LocalRPC_Port);
	return 0;
}

/*! \brief Parse commandline arguments.
 *
 *	\param argc Amount of commandline arguments
 *	\param argv The commandline parameters
 *	\returns CommandLine string arguments parsed into an element of type CCommandLineArguments.
 *
 */
CCommandLineArguments HandleCommandLineArguments(int argc, char* argv[])
{
	CCommandLineArguments CommandLineArguments;
	try
  {
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message"), ("local-rpc-port", po::value < unsigned short > (&CommandLineArguments.m_LocalRPC_Port), "Set local RPC port"),
    ("remote-server-ip", po::value<std::string>(&CommandLineArguments.m_ServerIp), "Set remote server ip"),
    ("remote-server-port", po::value<unsigned short> (&CommandLineArguments.m_ServerPort), "Set remote server port");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
		{
      std::cout << desc << "\n";
      CommandLineArguments.m_ShouldTerminateApplication = true;
      return CommandLineArguments;
    }

		po::notify(vm);

		CommandLineArguments.m_ShouldTerminateApplication = false;
		return CommandLineArguments;
	}
	catch(std::exception& e)
	{
		std::cerr << "Error occured while parsing commandline input."<< std::endl <<e.what() << "\n";
		CommandLineArguments.m_ShouldTerminateApplication = true;
		return CommandLineArguments;
	}
}

