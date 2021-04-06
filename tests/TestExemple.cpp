#include "Test.hpp"

TEST(NameOfTheFirstTest, "This text indicates what's tested")
{
	ASSERT_THROW(throw std::exception(), std::exception, "This test checks that an exception gets raised");
	ASSERT_PASS(; , "This test checks that no exception gets raised");
};


TEST(NameOfTheSecondTest, "The tests can be splitted in multiples sections")
{
	ASSERT_BOOL(true, "This checks that the given expression is true");
	ASSERT_EQUAL(2, 2., "This checks the equality between 2 expressions");
	ASSERT_EQUAL(2, 3, "All tests can be made skipable by adding the 'SKIP' keyword", SKIP);
};


TEST(NameOfTheThirdTest, "Each section will display its own message")
{
	ASSERT_PASS_RPT(;, "Each test can be repeated a given number of time to prevent randomness", 5000);
	ASSERT_THROW_RPT(; , std::exception, "Even repeatable tests can be made skipable", 5000, SKIP);
	ASSERT_PERF(for (int i = 0; i < 6500; i++) { ; }, 1000, 0.01, "This checks that an action doesn't take more time than it should on average");
};