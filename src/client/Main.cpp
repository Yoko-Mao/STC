/*! \file */
#include <iostream>
#include "boost/program_options.hpp"
#include <grpc++/grpc++.h>
#include "extcomm.grpc.pb.h"

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


// Logic and data behind the server's behavior.
class ExtCommImpl final : public ::comm::ManagementComm::Service
{
	//Status SayHello(ServerContext* context, const HelloRequest* request,
	//				HelloReply* reply) override {
	//	std::string prefix("Hello ");
	//	reply->set_message(prefix + request->name());
	//	return Status::OK;
	//}
};

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


	std::string server_address("0.0.0.0:50051");
	ExtCommImpl service;

	::grpc::ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();


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

