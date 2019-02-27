#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>


#include "stack_template_test.hpp"
#include "type_traits_tests.hpp"
#include "type_set_tests.hpp"


class MyProgressListener : public CppUnit::BriefTestProgressListener
{
  public:

    virtual void startSuite(CppUnit::Test* test)
    {
        //BriefTestProgressListener::startSuite(test);

        if(not test->getName().empty())
        {
            std::cout << prefix << "* suite: " << test->getName() << std::endl << std::endl;

            prefix.push_back(' ');
            prefix.push_back(' ');
        }
    }

    virtual void endSuite(CppUnit::Test* test)
    {
        if(not test->getName().empty())
        {
            prefix.pop_back();
            prefix.pop_back();

            std::cout << std::endl;
        }
    }

    virtual void startTest(CppUnit::Test* test)
    {
        std::cout << prefix << "- " << test->getName();
    }

    std::string prefix;
};


int main()
{
    CppUnit::TestResult testresult;

    CppUnit::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    MyProgressListener progress;
    testresult.addListener (&progress);

    CppUnit::TestRunner testrunner;
    //testrunner.addTest(StackTemplateTestSuite::suite());
    //testrunner.addTest(TypeTraitsTestSuite::suite());
    //testrunner.addTest(TypeSetTestSuite::suite());

    testrunner.run(testresult);

    CppUnit::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write();

    return collectedresults.wasSuccessful () ? 0 : 1;
}

