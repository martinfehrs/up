#pragma once


/*
class TypeTraitsTestSuite : public CppUnit::TestFixture
{
  public:

    using CppUnit::TestFixture::TestFixture;

    static CppUnit::Test* suite()
    {
        auto p_suite = new CppUnit::TestSuite{ "Type traits tests" };

        p_suite->addTest(new CppUnit::TestCaller<TypeTraitsTestSuite>(
            "all_different_types", &TypeTraitsTestSuite::all_different_types_test
        ));

        return p_suite;
    }

  private:

    void all_different_types_test()
    {
        using namespace std;

        // check for emtpy type list
        CPPUNIT_ASSERT((all_different_types<>) == true);

        // check type pair
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint16_t>) == true);
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint8_t>) == false);

        // check type list
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint16_t, std::uint32_t>) == true);
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint16_t, std::uint16_t>) == false);
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint8_t, std::uint32_t>) == false);
        CPPUNIT_ASSERT((all_different_types<std::uint8_t, std::uint16_t, std::uint8_t>) == false);
    }
};
*/
