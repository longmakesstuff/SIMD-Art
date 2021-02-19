#ifndef CMAKE_EXAMPLE_TESTEXAMPLE_H
#define CMAKE_EXAMPLE_TESTEXAMPLE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

class TestExample : public CPPUNIT_NS::TestFixture {

CPPUNIT_TEST_SUITE(TestExample);
        CPPUNIT_TEST(agnostic_test);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp() override {
        TestFixture::setUp();
    }

    void tearDown() override {
        TestFixture::tearDown();
    }

    static void agnostic_test();
};


#endif
