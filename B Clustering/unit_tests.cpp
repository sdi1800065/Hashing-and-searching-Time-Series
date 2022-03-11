#include <iostream>
#include <vector>
#include "clusterring_functions.hpp"
#include "lsh.hpp"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;
using namespace CppUnit;

class MyFirstTestCase : public TestCase{
    CPPUNIT_TEST_SUITE(MyFirstTestCase);
    CPPUNIT_TEST(test_for_initial_r);
    CPPUNIT_TEST(test_for_distance);
    CPPUNIT_TEST_SUITE_END();

  public:
    void test_for_initial_r();
    void test_for_distance();
};

void MyFirstTestCase::test_for_initial_r()
{
  centers.resize(5);
  for (int i=0; i<5; i++) {
    int b = i*i + 3;
    for (int j = b; j < b + 4; j++)  centers[i].push_back(j);
  }
  int expected_r = 2;
  CPPUNIT_ASSERT(expected_r == min_distance_among_centers());    //Check if the result equals to the expected
}

void MyFirstTestCase::test_for_distance()
{
  vector<double> curve1;
  curve1.resize(6);
  vector<double> curve2;
  curve2.resize(6);
  for (int i = 0; i < curve1.size(); i++) curve1[i] = i*i;
  for (int i = 0; i < curve2.size(); i++) curve2[i] = i*i*i;
  double expected_distance = 100;
  CPPUNIT_ASSERT(expected_distance == _distance(curve1,curve2));
}

CPPUNIT_TEST_SUITE_REGISTRATION(MyFirstTestCase);


int main()
{
  TextUi::TestRunner runner;
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  // Adds the test to the list of test to run
  runner.addTest( suite );
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}
