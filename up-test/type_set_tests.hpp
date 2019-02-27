#pragma once


/*
#include "type_set.hpp"


class TypeSetTestSuite : public CppUnit::TestFixture
{
  public:

    using CppUnit::TestFixture::TestFixture;

    static CppUnit::Test* suite()
    {
        auto p_suite = new CppUnit::TestSuite{ "Type set tests" };

        p_suite->addTest(new CppUnit::TestCaller<TypeSetTestSuite>(
            "ins", &TypeSetTestSuite::instantiation_test
        ));

        return p_suite;
    }

  private:

    void instantiation_test()
    {
        using namespace std;


        // check emtpy type list
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<>>) == true);

        // check single type
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<std::uint8_t>>) == true);

        // check type pair


        // check type list
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<std::uint8_t, std::uint16_t, std::uint32_t>>) == true);
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<std::uint8_t, std::uint16_t, std::uint16_t>>) == false);
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<std::uint8_t, std::uint8_t, std::uint32_t>>) == false);
        //CPPUNIT_ASSERT((is_type_set_instantiable<TypeSet<std::uint8_t, std::uint16_t, std::uint8_t>>) == false);
    }
};
*/

