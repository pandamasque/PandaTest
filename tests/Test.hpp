#include <list>
#include <string>
#include <iostream>
#include <stdexcept>
#include <chrono>

/*Color code macros*/
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

/*Namespace of the test utility.*/
namespace pandatest {
	/*This object stores a test function to be called by the test runner.
	It adds itself to the list of test during its construction.*/
	class UnitTest {
	public:
		UnitTest(void (*)(void), std::string);
		void operator() ();
	private:
		void (*testFunction)(void);
		std::string description;
	};

	class DisplayFail {
	public:
		DisplayFail();
		DisplayFail(unsigned int*);
		std::ostream& display(std::string);
	private:
		const bool skip;
	};

	/*Increments the number of failed Tests.*/
	void Fail();
	/*Increments an external variable (meant for SKIP).*/
	void Fail(unsigned int*);
	/*Count the number of test runned.*/
	void TestCount();

	/*global list of the tests to run.*/
	inline std::list<UnitTest*> unitTests;
	/*The following variables need to be define here to prevent their redefinition in another file.*/
	inline unsigned int TEST_ITERATOR = 0;
	inline bool TEST_PASS = 0;
	inline std::chrono::high_resolution_clock::time_point START;
	inline std::chrono::duration<float, std::milli> DURATION;
}
extern unsigned int* SKIP;

/*Displays that the test was successful.*/
#define SUCCESS(description) std::cout << "\t" << description << ": " << GRN << "OK" << RESET << std::endl;

/*This maccro creates a new UnitTest object and associate to it the code in bracket following it.
The tests added this way will be executed in order.*/
#define TEST(testName, description) \
void testName(void); \
pandatest::UnitTest testName ## _obj(&testName, description);\
void testName(void)

/*This maccro allows to add a unit test that checks if the code raises when it shouldn't for a fixed number of executions.*/
#define ASSERT_PASS_RPT(assertion, description, repetition, ...)\
try {\
	for (int i=0;i<repetition;++i)\
	{assertion;}\
	SUCCESS(description)}\
catch (...){\
	/*The Fail and DisplayFail functions are overloaded to change the behaviour of the test if SKIP is added.*/ \
	pandatest::Fail(__VA_ARGS__);\
	auto disfail = pandatest::DisplayFail(__VA_ARGS__);\
	disfail.display(std::string(description)) << std::endl;}\
pandatest::TestCount();

/*This maccro allows to add a unit test that checks if the code raises the right exception  for a fixed number of executions.*/
#define ASSERT_THROW_RPT(assertion, exceptionType, description, repetition, ...)\
pandatest::TEST_ITERATOR=0;\
pandatest::TEST_PASS = true;\
while (pandatest::TEST_PASS && (pandatest::TEST_ITERATOR < repetition)){\
	try {\
		assertion;\
		pandatest::TEST_PASS = false;\
		pandatest::Fail(__VA_ARGS__); \
		auto disfail = pandatest::DisplayFail(__VA_ARGS__);\
		disfail.display(std::string(description)) << std::endl;}\
	catch (const exceptionType&) {;}\
	catch (...) {\
		pandatest::TEST_PASS = false;\
		/*The Fail and DisplayFail functions are overloaded to change the behaviour of the test if SKIP is added.*/ \
		pandatest::Fail(__VA_ARGS__);  \
		auto disfail = pandatest::DisplayFail(__VA_ARGS__);\
		disfail.display(std::string(description)) << std::endl;}\
	pandatest::TEST_ITERATOR++;}\
SUCCESS(description)\
pandatest::TestCount();

/*This maccro allows to add a unit test that checks the performance of a given function against a given time for a fixed number of executions.*/
#define ASSERT_PERF(functionCall, numberExecution, averageSecondsTime, description, ...)\
pandatest::START = std::chrono::high_resolution_clock::now();\
for (unsigned long int i = 0; i < numberExecution; ++i){\
	functionCall;}\
pandatest::DURATION = std::chrono::high_resolution_clock::now() - pandatest::START;\
if ( pandatest::DURATION.count() > averageSecondsTime*numberExecution) {\
	/*The Fail and DisplayFail functions are overloaded to change the behaviour of the test if SKIP is added.*/ \
	pandatest::Fail(__VA_ARGS__);\
	auto disfail = pandatest::DisplayFail(__VA_ARGS__);\
	disfail.display(std::string(description)) << ", expected: " << averageSecondsTime << "s, had:" << pandatest::DURATION.count()/numberExecution << "s" << std::endl;} \
else {SUCCESS(description)}\
pandatest::TestCount();

/*This maccro allows to add a unit test that checks if the condition given is fullfilled for a fixed number of executions.*/
#define ASSERT_BOOL_RPT(condition, description, repetition, ...) ASSERT_THROW_RPT(if (condition) throw std::exception();, std::exception, description, repetition, __VA_ARGS__)

/*This maccro allows to add a unit test that checks if the two values are equal for a fixed number of executions.*/
#define ASSERT_EQUAL_RPT(a, b, description, repetition, ...) ASSERT_BOOL_RPT(a == b, description, repetition, __VA_ARGS__)

/*This maccro allows to add a unit test that checks if the code raises when it shouldn't.*/
#define ASSERT_PASS(assertion, description, ...) ASSERT_PASS_RPT(assertion, description, 1, __VA_ARGS__)

/*This maccro allows to add a unit test that checks if the code raises the right exception.*/
#define ASSERT_THROW(assertion, exceptionType, description, ...) ASSERT_THROW_RPT(assertion, exceptionType, description, 1, __VA_ARGS__)

/*This maccro allows to add a unit test that checks if the condition given is fullfilled.*/
#define ASSERT_BOOL(condition, description, ...) ASSERT_BOOL_RPT(condition, description, 1, __VA_ARGS__)

/*This maccro allows to add a unit test that checks if the two values are equal.*/
#define ASSERT_EQUAL(a, b, description, ...) ASSERT_EQUAL_RPT(a, b, description, 1, __VA_ARGS__)
