/*! \file */
#include <iostream>
#include "boost/program_options.hpp"

namespace po = boost::program_options;

/*! \brief Parsed result of command line arguments
 *
 */
struct CommandLineArguments_t
{
	bool ShouldTerminateApplication; ///< This flag is toggled as result of wrong commandline input, to indicate early termination
};
CommandLineArguments_t HandleCommandLineArguments(int, char*[]);

/*! \brief Application entry point.
 *
 *	\param argc Amount of commandline arguments
 *	\param argv The commandline parameters
 *
 *	\note Enter parameter "--help" to display list of parameters that can be entered.
 */
int main(int argc, char *argv[])
{
	   CommandLineArguments_t Arguments = HandleCommandLineArguments(argc, argv);

	// Error occured while parsing command line arguments. Proper error should've been dumped already.
	if (Arguments.ShouldTerminateApplication)
		return 1;

	return 0;
}

/*! \brief Parse commandline arguments.
 *
 *	\param argc Amount of commandline arguments
 *	\param argv The commandline parameters
 *	\returns CommandLine string arguments parsed into an element of type CCommandLineArguments.
 *
 */
CommandLineArguments_t HandleCommandLineArguments(int argc, char* argv[])
{
	   CommandLineArguments_t CommandLineArguments;
	try
	{
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

