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

int RdBigTest::runAll()
{
    int result = 0;
    for(std::map<std::string, RdBigTest>::iterator it = bigTests->begin(); it != bigTests->end(); ++it)
        result += it->second.run();
    return result;
}

int RdBigTest::run()
{
    unsigned ok = 0, fail = 0;
    for(std::vector<RdLittleTest *>::iterator it = littleTests.begin(); it != littleTests.end(); ++it)
    {
        RdLittleTest & littleTest = *(*it);
        littleTest.ok = true;
        littleTest.run();
        if(littleTest.ok)
        {
            std::cout << GREEN_COLOR << name << "::" << littleTest.name << "   OK" << NO_COLOR << '\n';
            ok++;
        }
        else
        {
            std::cout << RED_COLOR << name << "::" << littleTest.name << "   FAILED" << NO_COLOR << '\n';
            fail++;
        }
    }
    if(fail > 0)
    {
        std::cout << RED_COLOR << name << ": " << fail << " out of " << (ok+fail) << "FAILED" << NO_COLOR << '\n';
    }
    else
    {
        std::cout << GREEN_COLOR << name << ": " << ok << " out of " << ok << " OK" << NO_COLOR << '\n';
    }
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
