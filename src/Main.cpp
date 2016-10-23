#include <iostream>
#include "boost/program_options.hpp"

#include "net/Client.h"
#include "net/Server.h"

namespace po = boost::program_options;


struct CCommandLineArguments
{
	bool ShouldTerminate;
	unsigned short ServerPort = 8000;
	unsigned short ClientPort = 8001;
};
CCommandLineArguments HandleCommandLineArguments(int, char*[]);

int main(int argc, char *argv[])
{
	CCommandLineArguments Arguments = HandleCommandLineArguments(argc, argv);

	// Error occured while parsing command line arguments. Proper error should've been dumped already.
	if (Arguments.ShouldTerminate)
		return 1;

	boost::asio::io_service IO_Service;
	Net::CServer Server(IO_Service, Arguments.ServerPort);
	Net::CClient Client(IO_Service, Arguments.ClientPort, "127.0.0.1");
	IO_Service.run();

	return 0;
}

CCommandLineArguments HandleCommandLineArguments(int argc, char* argv[])
{
	CCommandLineArguments CommandLineArguments;

	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "produce help message")
	    ("serverport,sp", po::value<unsigned short>(&CommandLineArguments.ServerPort), "Set local listening port")
		("clientport,cp", po::value<unsigned short>(&CommandLineArguments.ClientPort), "Set remote client port");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
	    std::cout << desc << "\n";
	    CommandLineArguments.ShouldTerminate = true;
	    return CommandLineArguments;
	}

	po::notify(vm);

	CommandLineArguments.ShouldTerminate = false;
	return CommandLineArguments;
}

