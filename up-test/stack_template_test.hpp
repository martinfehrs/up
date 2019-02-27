#pragma once

#include <algorithm>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "up/stacks.hpp"

/*

constexpr std::size_t MAX_STACK_SIZE = 10;

// This allocator is only suitable for construction and destruction testing
struct DummyAllocator
{
    using pointer = std::uint8_t*;


    pointer allocate(std::size_t n)
    {
       allocation_size += n;
       return mem;
    }

    void deallocate(pointer p, std::size_t n)
    {
        deallocated_pointer = p;
        allocation_size -= n;
    }

    pointer deallocated_pointer;
    std::size_t allocation_size = 0;
    std::uint8_t mem[MAX_STACK_SIZE];
};


using test_stack_t = StackTemplate<TypeSet<std::int8_t, std::int16_t>, DummyAllocator>;


class StackTemplateConstructionFixture : public CppUnit::TestFixture
{
  public:

    static CppUnit::Test* suite()
    {
        auto p_suite = new CppUnit::TestSuite;

        p_suite->addTest(new CppUnit::TestCaller<StackTemplateConstructionFixture>{
            "construction", &StackTemplateConstructionFixture::construction_test
        });

        return p_suite;
    }

  private:

    void construction_test()
    {
        using namespace std;

        p_test_stack = new(stack_mem) test_stack_t{ MAX_STACK_SIZE };

        CPPUNIT_ASSERT(p_test_stack->_rbegin != nullptr);
        CPPUNIT_ASSERT(p_test_stack->_rend == p_test_stack->_rbegin);
        CPPUNIT_ASSERT(all_of(p_test_stack->_rbegin, p_test_stack->_rbegin + MAX_STACK_SIZE, [](uint8_t val){ return val == 0; }));
    }

    test_stack_t* p_test_stack = nullptr;
    std::uint8_t stack_mem[sizeof(test_stack_t)];
};

class StackTemplateDestructionFixture : public CppUnit::TestFixture
{
  public:

    static CppUnit::Test* suite()
    {
        auto p_suite = new CppUnit::TestSuite;

        p_suite->addTest(new CppUnit::TestCaller<StackTemplateDestructionFixture>{
            "destruction", &StackTemplateDestructionFixture::destruction_test
        });

        return p_suite;
    }

    virtual void setUp() override
    {
        p_test_stack = reinterpret_cast<test_stack_t*>(stack_mem);
        p_test_stack->_rbegin = p_test_stack->allocator.allocate(MAX_STACK_SIZE);
        p_test_stack->_rend = p_test_stack->_rbegin;
    }

  private:

    void destruction_test()
    {
        p_test_stack->~StackTemplate();

        CPPUNIT_ASSERT(p_test_stack->allocator.allocation_size == 0);
        CPPUNIT_ASSERT(p_test_stack->_rbegin == p_test_stack->allocator.deallocated_pointer);
    }

    test_stack_t* p_test_stack = nullptr;
    std::uint8_t stack_mem[sizeof(test_stack_t)];
};

class StackTemplateManipulationTests : public CppUnit::TestFixture
{
  public:

    static CppUnit::Test* suite()
    {
        auto p_suite = new CppUnit::TestSuite;

        p_suite->addTest(new CppUnit::TestCaller<StackTemplateManipulationTests>(
            "push", &StackTemplateManipulationTests::push_test
        ));

        p_suite->addTest(new CppUnit::TestCaller<StackTemplateManipulationTests>(
            "drop", &StackTemplateManipulationTests::drop_test
        ));

        p_suite->addTest(new CppUnit::TestCaller<StackTemplateManipulationTests>(
            "size", &StackTemplateManipulationTests::size_test
        ));

        return p_suite;
    }

    virtual void setUp() override;
    virtual void tearDown() override;

  protected:

     // tests for methods of template struct StackValuePack
    //void calculate_byte_size_test();
    //void calculate_count_test();

     // tests for methods of template class StackTemplate

    //void byte_size_test();
    //void byte_resize_test();
    //void cleanup_test();
    //void increase_byte_size_test();
    //void reduce_byte_size_test();

    //void construction_test();
    //void destruction_test();
    void push_test();
    void drop_test();
    void size_test();

  private:

    static constexpr std::size_t MAX_STACK_SIZE = 10;

};


class StackTemplateTestSuite : public CppUnit::TestSuite
{
  public:

    using CppUnit::TestSuite::TestSuite;

    static CppUnit::Test* suite()
    {
        auto p_suite = new StackTemplateTestSuite{ "Stack template tests" };

        p_suite->addTest(StackTemplateConstructionFixture::suite());
        p_suite->addTest(StackTemplateDestructionFixture::suite());
        p_suite->addTest(StackTemplateManipulationTests::suite());

        return p_suite;
    }
};

*/
