//
// Created by yoko on 11/13/16.
//
#ifndef STC_COMMANDLINE_H
#define STC_COMMANDLINE_H
#include <iostream>
#include "boost/program_options.hpp"
namespace po = boost::program_options;

/*! \brief Parsed result of command line arguments
 *
 */
namespace Core
{
class CommandLineArgumentsBase_t {
public:

  CommandLineArgumentsBase_t(int Argc, char* Argv[]) : m_Description("Allowed Options"), m_Argc(Argc), m_Argv(Argv) {
  }
  bool Parse(); ///< Parse commandline arguments as passed by Argc and Argv.
  unsigned short m_LocalRPC_Port = 50051; ///< Port on which local RPC clients listens for incomming connections.

protected:
  po::options_description m_Description;
  virtual void SetDescription() = 0; ///< Set Description to allow when option --help is passed
  bool m_ShouldTerminateApplication = false; ///< This flag is toggled as result of wrong commandline input, to indicate early termination
  int m_Argc;
  char** m_Argv;

};
}
#endif //STC_COMMANDLINE_H
