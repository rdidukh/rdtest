#pragma once

#ifndef FILE_RDTEST_H
#define FILE_RDTEST_H

#include <iostream>
#include <vector>
#include <string>

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
        static void runAll();
        void run();
    };

    static struct RdBigTestsInit
    { 
        RdBigTestsInit();
        ~RdBigTestsInit();
    } rdBigTestsInit;
}

#define TEST(className, testName)   \
do {                                \
                                    \
} while(0);

#endif // FILE_RDTEST_H