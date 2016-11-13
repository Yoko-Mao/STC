#include "CommandLine.h"

/*! \brief Parse commandline arguments.
 *  The CCommandLineArguments contains the parsed parameters.
 *  \returns True if an error occured during parsing, else returns false.
 *
 */
bool Core::CommandLineArgumentsBase_t::Parse() {
    SetDescription();
    try {
        auto vm = po::variables_map();
        po::store(po::parse_command_line(m_Argc, m_Argv, m_Description), vm);

        if (vm.count("help")) {
            std::cout << m_Description << "\n";
            m_ShouldTerminateApplication = true;
            return m_ShouldTerminateApplication;
        }

        po::notify(vm);

        m_ShouldTerminateApplication = false;
        return m_ShouldTerminateApplication;
    } catch (std::exception& e) {
        std::cerr << "Error occured while parsing commandline input." << std::endl << e.what() << "\n";
        m_ShouldTerminateApplication = true;
        return m_ShouldTerminateApplication;
    }
}
