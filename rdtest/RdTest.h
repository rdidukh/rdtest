#pragma once

#ifndef FILE_RDTEST_H
#define FILE_RDTEST_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

namespace rdns
{
    struct RdLittleTest
    {
        RdLittleTest(const std::string & className, const std::string & testName);
        virtual void run()=0;
        std::string name;
        bool ok;
    };

    class RdBigTest
    {
        std::vector<RdLittleTest *> littleTests;
        std::string name;
    public:
        RdBigTest(const std::string & className);
        void addLittleTest(RdLittleTest * littleTest);
        static int runAll();
        int run();
    };

    static struct RdBigTestsInit
    { 
        RdBigTestsInit();
        ~RdBigTestsInit();
    } rdBigTestsInit;
    
    struct VoidReturner
    {
        void operator=(std::ostream & os) {}
    };
    
    struct Cmp_Eq
    {
        template<class U, class V>
        bool operator()(const U& u, const V& v)
        {
            return u == v;
        }
    };
    
    template<typename U, typename V, typename Compare>
    bool expect_common(const U & expected, const V & actual, const char * expStr, const char * actStr, const char * file, unsigned line, Compare comp, const char * cmpName)
    {
        bool res = comp(actual, expected);

        if(!res)
        {
            std::string cmpNameStr = std::string("To be ") + cmpName + ": ";
            int width = cmpNameStr.length() < 10 ? 10 : cmpNameStr.length();
            std::cout << file << ":" << line << ": Failure" << '\n';
            std::cout << std::setw(width) << "Expected: " << expStr << '\n';
            std::cout << std::setw(width) << "Which is: " << expected << '\n';
            std::cout << std::setw(width) << cmpNameStr << actStr << '\n';
            std::cout << std::setw(width) << "Which is: " << actual << '\n';        
            return false;
        }

        return true;
    }
    
}

#define CALL_EXPECT_COMMON(expected, actual, comp, cmpName) \
(this->ok = rdns::expect_common(expected, actual, #expected, #actual, __FILE__, __LINE__, comp, cmpName))

#define EXPECT_COMMON(expected, actual, comp, cmpName) \
CALL_EXPECT_COMMON(expected, actual, comp, cmpName) || (this->ok = false) || std::cout

#define ASSERT_COMMON(expected, actual, comp, cmpName) \
CALL_EXPECT_COMMON(expected, actual, comp, cmpName) || (this->ok = false) || return VoidReturner() = std::cout

#define EXPECT_EQ(expected, actual) EXPECT_COMMON(expected, actual, rdns::Cmp_Eq(), "equal to")

#define ASSERT_EQ(expected, actual) ASSERT_COMMON(expected, actual, rdns::Cmp_Eq(), "equal to")

#define TEST(className, testName)                                                                   \
namespace rdns {                                                                                    \
struct className##_##testName##_Test : RdLittleTest                                                 \
{                                                                                                   \
    className##_##testName##_Test(const char * a, const char * b):RdLittleTest(a, b) {}             \
    void run();                                                                                     \
}; }                                                                                                \
static rdns::className##_##testName##_Test className##_##testName##_TestObj(#className, #testName); \
void rdns::className##_##testName##_Test::run()

#define RUN_ALL_TESTS() rdns::RdBigTest::runAll();

#endif // FILE_RDTEST_H