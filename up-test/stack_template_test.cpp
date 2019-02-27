#include <algorithm>

#include "stack_template_test.hpp"


/*
void StackTemplateManipulationTests::setUp()
{

}

void StackTemplateManipulationTests::tearDown()
{

}
*/

//void StackTemplateTest::calculate_byte_size_test()
//{
//    using namespace std;
//
//    using stack_value_pack_t = StackValuePack<int8_t, int16_t>;
//
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_byte_size(1) == sizeof(int8_t));
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_byte_size(2) == sizeof(int8_t) + sizeof(int16_t));
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_byte_size(3) == sizeof(int8_t) + 2 * sizeof(int16_t));
//}
//
//void StackTemplateTest::calculate_count_test()
//{
//    using namespace std;
//
//    using stack_value_pack_t = StackValuePack<int8_t, int16_t>;
//
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_count(sizeof(int8_t)) == 1);
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_count(sizeof(int16_t)) == 1);
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_count(sizeof(int8_t) + sizeof(int16_t)) == 2);
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_count(2 * sizeof(int8_t) + sizeof(int16_t)) == 2);
//    CPPUNIT_ASSERT(stack_value_pack_t::calculate_count(sizeof(int8_t) + 2 * sizeof(int16_t)) == 3);
//}

//void StackTemplateTest::byte_size_test()
//{
//    StackTemplate<int> stack{ MAX_STACK_SIZE };
//
//    stack._rend += MAX_STACK_SIZE;
//    CPPUNIT_ASSERT(stack.byte_size() == MAX_STACK_SIZE);
//}

//void StackTemplateTest::byte_resize_test()
//{
//    StackTemplate<int> stack{ MAX_STACK_SIZE };
//
//    stack.byte_resize(MAX_STACK_SIZE);
//
//    CPPUNIT_ASSERT((stack._rend - stack._rbegin) == MAX_STACK_SIZE);
//}

//void StackTemplateTest::cleanup_test()
//{
//    using namespace std;
//
//    StackTemplate<int> stack{ MAX_STACK_SIZE };
//
//    stack._rend += MAX_STACK_SIZE;
//    fill_n(stack._rbegin, MAX_STACK_SIZE, 1);
//    stack.cleanup(MAX_STACK_SIZE);
//
//    CPPUNIT_ASSERT(all_of(stack._rbegin, stack._rbegin + MAX_STACK_SIZE, [](uint8_t val){ return val == 0; }));
//}

//void StackTemplateTest::increase_byte_size_test()
//{
//    StackTemplate<int> stack{ MAX_STACK_SIZE };
//
//    stack.increase_byte_size(MAX_STACK_SIZE);
//
//    CPPUNIT_ASSERT((stack._rend - stack._rbegin) == MAX_STACK_SIZE);
//}

//void StackTemplateTest::reduce_byte_size_test()
//{
//    using namespace std;
//
//    StackTemplate<int> stack{ MAX_STACK_SIZE };
//
//    stack._rend += MAX_STACK_SIZE;
//    fill_n(stack._rbegin, MAX_STACK_SIZE, 1);
//    stack.reduce_byte_size(MAX_STACK_SIZE);
//
//    CPPUNIT_ASSERT((stack._rend - stack._rbegin) == 0);
//    CPPUNIT_ASSERT(all_of(stack._rbegin, stack._rbegin + MAX_STACK_SIZE, [](uint8_t val){ return val == 0; }));
//}

/*
void StackTemplateManipulationTests::push_test()
{
    using namespace std;

    StackTemplate<TypeSet<int8_t, int16_t>> stack{ MAX_STACK_SIZE };
    auto publicationCounter = 0U;

    stack.subscribers.push_back([&](auto&) {
        publicationCounter++;
    });

    stack.push(int8_t{1});
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == sizeof(int8_t) and publicationCounter == 1);
    stack.push(int16_t{2});
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == sizeof(int8_t) + sizeof(int16_t) and publicationCounter == 2);
    stack.push(int16_t{3}, int8_t{4});
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == 2 * sizeof(int8_t) + 2 * sizeof(int16_t) and publicationCounter == 3);
}

void StackTemplateManipulationTests::drop_test()
{
    using namespace std;

    StackTemplate<TypeSet<int8_t, int16_t>> stack{ MAX_STACK_SIZE };

    auto publicationCounter = 0U;

    stack.subscribers.push_back([&](auto&) {
        publicationCounter++;
    });

    stack._rend += MAX_STACK_SIZE;

    stack.drop();
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == MAX_STACK_SIZE - sizeof(int8_t) and publicationCounter == 1);
    stack.drop<int16_t>();
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == MAX_STACK_SIZE - sizeof(int8_t) - sizeof(int16_t) and publicationCounter == 2);
    stack.drop<int8_t, int16_t>(2);
    CPPUNIT_ASSERT(stack._rend - stack._rbegin == MAX_STACK_SIZE - 2 * sizeof(int8_t) - 2 * sizeof(int16_t) and publicationCounter == 3);
}

void StackTemplateManipulationTests::size_test()
{
    using namespace std;

    StackTemplate<TypeSet<int8_t, int16_t>> stack{ MAX_STACK_SIZE };

    CPPUNIT_ASSERT(stack.size() == 0);

    stack._rend += MAX_STACK_SIZE;

    CPPUNIT_ASSERT(stack.size() == MAX_STACK_SIZE);
    CPPUNIT_ASSERT(stack.size<int8_t>() == MAX_STACK_SIZE);
    CPPUNIT_ASSERT(stack.size<int16_t>() == MAX_STACK_SIZE / 2);

    CPPUNIT_ASSERT((stack.size<int8_t, int16_t>() == MAX_STACK_SIZE / 2));
    CPPUNIT_ASSERT((stack.size<int8_t, int8_t, int16_t>() == MAX_STACK_SIZE / 2 + 1));
    CPPUNIT_ASSERT((stack.size<int16_t, int8_t>() == MAX_STACK_SIZE - 1));
    CPPUNIT_ASSERT((stack.size<int16_t, int16_t, int8_t>() == MAX_STACK_SIZE - 2));
}
*/
