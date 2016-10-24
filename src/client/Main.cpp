/*! \file */
#include <iostream>
#include "boost/program_options.hpp"

#include "common/net/Client.h"
#include "../common/net/Client.h"
#include "../common/net/Server.h"
#include <common/net/Server.h>

namespace po = boost::program_options;

/*! \brief Parsed result of command line arguments
 *
 */
struct CCommandLineArguments
{
	bool ShouldTerminateApplication; ///< This flag is toggled as result of wrong commandline input, to indicate early termination
	unsigned short ServerPort = 8000; ///< Port on which the client is listening for incoming connection on localhost.
	unsigned short ClientPort = 8000; ///< Port on which a remote client is listening for incoming connections (remote client its serverport).
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
	CCommandLineArguments Arguments = HandleCommandLineArguments(argc, argv);

	// Error occured while parsing command line arguments. Proper error should've been dumped already.
	if (Arguments.ShouldTerminateApplication)
		return 1;

	boost::asio::io_service IO_Service;
	Net::CServer Server(IO_Service, Arguments.ServerPort);
	Net::CClient Client(IO_Service, Arguments.ClientPort, "127.0.0.1");
	IO_Service.run();

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
		desc.add_options()
			("help", "produce help message")
			("serverport,sp", po::value<unsigned short>(&CommandLineArguments.ServerPort), "Set local listening port")
			("clientport,cp", po::value<unsigned short>(&CommandLineArguments.ClientPort), "Set remote client port");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			CommandLineArguments.ShouldTerminateApplication = true;
			return CommandLineArguments;
		}

		po::notify(vm);

		CommandLineArguments.ShouldTerminateApplication = false;
		return CommandLineArguments;
	}
	catch(std::exception& e)
	{
		std::cerr << "Error occured while parsing commandline input."<< std::endl <<e.what() << "\n";
		CommandLineArguments.ShouldTerminateApplication = true;
		return CommandLineArguments;
	}

}

