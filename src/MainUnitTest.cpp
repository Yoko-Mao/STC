#include "common/core/task/TestSuite_Task.h"

boost::unit_test::test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
  boost::unit_test::framework::master_test_suite().p_name.value = "my master test suite name";
  return 0;
}
