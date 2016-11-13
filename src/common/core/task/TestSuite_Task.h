#ifndef TESTSUITE_TASK_H
#define TESTSUITE_TASK_H
#include <boost/test/included/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( MessageQueue )

BOOST_AUTO_TEST_CASE (Pop)
{
  BOOST_ERROR( "some error 1" );
}
BOOST_AUTO_TEST_CASE (Push)
{
  BOOST_ERROR( "some OK" );
}
BOOST_AUTO_TEST_SUITE_END()

#endif /* TESTSUITE_TASK_H */

