#include "Test.hpp"

unsigned int runnedTests = 0, failedTests = 0, skippedTests = 0;
unsigned int* SKIP = &skippedTests;

namespace pandatest {
	UnitTest::UnitTest(void (*test)(void), std::string description) : testFunction(test), description(description) {
		unitTests.push_back(this);//Adding itself to the list of tests.
	};
	void UnitTest::operator() () {
		std::cout << description << ":" << std::endl;
		testFunction();
		std::cout << "\n" << std::endl;
	};

	DisplayFail::DisplayFail() : skip(false) {};
	DisplayFail::DisplayFail(unsigned int* val) : skip(true) {
		val++;//Makes the compiler feel better.
	};
	std::ostream& DisplayFail::display (std::string description) {
		if (skip)
			/*Returns the message "Failled" in yellow.*/
			return std::cout << "\t" << std::string(description) << ": " << std::string(YEL) << "Failed" << std::string(RESET);
		/*Returns the message "Failled" in red.*/
		return std::cout << "\t" << description << ": " << RED << "Failed" << RESET;
	};

	void Fail() { failedTests++; }
	void Fail(unsigned int* count) { (*count)++; }
	void TestCount() { runnedTests += 1; }
}

int main() {
	for (auto* i : pandatest::unitTests) {//Executes all the unit tests that were added.
		try {//Protection against failures.
			(*i)();
		}
		catch (...) {continue;}
	}
	std::cout << "Total number of tests executed: " << runnedTests << "." << std::endl;//Displaying the total number of tests runned.
	std::cout << "Total number of tests failed: " << ((failedTests) ? RED : GRN) << failedTests << RESET << "." << std::endl;//Displaying the number of tests failled.
	std::cout << "Total number of tests skipped: " << ((skippedTests) ? YEL : GRN) << (skippedTests) << RESET << "." << std::endl;//Displaying the number of tests skipped.
	return failedTests;
}
