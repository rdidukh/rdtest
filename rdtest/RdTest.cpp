#include "RdTest.h"

#include <string>
#include <vector>
#include <map>

#ifdef _WIN32
#define RED_COLOR       ""
#define GREEN_COLOR	    ""
#define NO_COLOR	    ""
#else
#define RED_COLOR       "\033[0;31m"
#define GREEN_COLOR     "\033[0;32m"
#define NO_COLOR	    "\033[0m"
#endif

namespace rdns
{

typedef std::map<std::string, RdBigTest> RdBigTestMap;

static int bigTestsCounter;
static std::map<std::string, RdBigTest> * bigTests;

RdBigTest & getBigTest(const std::string & name)
{
    if(bigTests->find(name) == bigTests->end())
        bigTests->insert(make_pair(name, RdBigTest(name)));
    return bigTests->find(name)->second;
}

RdLittleTest::RdLittleTest(const std::string & className, const std::string & testName):name(testName)
{
    getBigTest(className).addLittleTest(this);
}

RdBigTest::RdBigTest(const std::string & className):name(className)
{
}

void RdBigTest::addLittleTest(RdLittleTest * littleTest)
{
    littleTests.push_back(littleTest);
}

static const char * plural(int x, bool upper = false)
{
    if(x == 1) return "";
    return upper ? "S" : "s";
}

int RdBigTest::runAll()
{
    int big = bigTests->size();
    int little = 0;

    for(std::map<std::string, RdBigTest>::iterator it = bigTests->begin(); it != bigTests->end(); ++it)
    {
        little += it->second.littleTests.size();
    }

    std::cout << GREEN_COLOR << "[==========]" << NO_COLOR;
    std::cout << " Running " << little << " test" << plural(little) << " from " << big << " test case" << plural(big) << ".\n";
    std::cout << GREEN_COLOR << "[----------]" << NO_COLOR << " Global test environment set-up.\n";

    int failed = 0;

    for(std::map<std::string, RdBigTest>::iterator it = bigTests->begin(); it != bigTests->end(); ++it)
    {
        failed += it->second.run();
    }   

    std::cout << GREEN_COLOR << "[----------]" << NO_COLOR << " Global test environment tear-down\n";
    std::cout << GREEN_COLOR << "[==========] " << NO_COLOR;
    std::cout << little << " test" << plural(little) << " from " << big << " test case" << plural(big) << " ran. (n/a ms total)\n";
    std::cout << GREEN_COLOR <<  "[  PASSED  ] " << NO_COLOR << (little-failed) << " test" << plural(little-failed) << ".\n";

    if(failed > 0)
    {
        std::cout << RED_COLOR << "[  FAILED  ] " << NO_COLOR << failed << " test" << plural(failed) << ", listed below:\n";
        for(int i = 0; i < failed; i++)
            std::cout << RED_COLOR << "[  FAILED  ]" << NO_COLOR << " N/A\n";
        std::cout << '\n';
        std::cout << failed << " FAILED TEST" << plural(failed, true) << "\n";
    }

    return failed;
}

int RdBigTest::run()
{
    int testCount = littleTests.size();
    std::cout << GREEN_COLOR << "[----------] " << NO_COLOR << testCount << " tests from " << name << '\n';

    int fail = 0;
    for(std::vector<RdLittleTest *>::iterator it = littleTests.begin(); it != littleTests.end(); ++it)
    {
        RdLittleTest & littleTest = *(*it);

        std::cout << GREEN_COLOR << "[ RUN      ] " << NO_COLOR << name << '.' << littleTest.name << '\n';

        littleTest.ok = true;
        littleTest.run();
        if(littleTest.ok)
        {
            std::cout << GREEN_COLOR << "[       OK ] " << NO_COLOR;
        }
        else
        {
            fail++;
            std::cout << RED_COLOR << "[  FAILED  ] " << NO_COLOR;
        }

        std::cout << name << '.' << littleTest.name << " (n/a ms)\n";

    }
    
    std::cout << GREEN_COLOR << "[----------] " << NO_COLOR;
    std::cout << testCount << " test" << plural(testCount) << " from " << name << " (n/a ms total)\n\n";

    return fail;
}

RdBigTestsInit::RdBigTestsInit()
{
	if(bigTestsCounter++ == 0)
		bigTests = new RdBigTestMap();
}

RdBigTestsInit::~RdBigTestsInit()
{
	if(--bigTestsCounter == 0)
		delete bigTests;
}

} // namespace rdns
