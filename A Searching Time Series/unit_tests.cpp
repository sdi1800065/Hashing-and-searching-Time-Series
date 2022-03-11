#include <iostream>
#include <vector>
#include "curves.hpp"
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
    CPPUNIT_TEST(test_for_filtering);
    CPPUNIT_TEST(test_for_distance);
    CPPUNIT_TEST_SUITE_END();

  public:
    //void testShouldFail();
    void test_for_filtering();
    void test_for_distance();
};

void MyFirstTestCase::test_for_filtering()
{
  vector <double> initial_curve;
  initial_curve.push_back(5.14);
  initial_curve.push_back(5.6);
  initial_curve.push_back(5.7);
  initial_curve.push_back(6.8);
  initial_curve.push_back(7);
  initial_curve.push_back(7.4);

  vector <double> expected_curve;                       //Expected curve after filtering
  expected_curve.push_back(5.14);
  expected_curve.push_back(5.7);
  expected_curve.push_back(6.8);
  expected_curve.push_back(7.4);
  CPPUNIT_ASSERT(expected_curve == filter(initial_curve));    //Check if the result equals to the expected
}

void MyFirstTestCase::test_for_distance()
{
  vector<double> curve1;
  curve1.resize(6);
  vector<double> curve2;
  curve2.resize(6);
  for (int i = 0; i < curve1.size(); i++) curve1[i] = i;
  for (int i = 0; i < curve2.size(); i++) curve2[i] = i*i;
  double expected_distance = 20;
  CPPUNIT_ASSERT(expected_distance == distance(curve1,curve2));
}

CPPUNIT_TEST_SUITE_REGISTRATION(MyFirstTestCase);


int main()
{
  TextUi::TestRunner runner;
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  // Adds the test to the list of test to run
  runner.addTest( suite );
  //runner.run();
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}
