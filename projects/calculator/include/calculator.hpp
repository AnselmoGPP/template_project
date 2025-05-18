#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <vector>
#include <string>

/// Parity of a number (even or odd).
enum Parity {any = 0, odd = 1, even = 2};

/// Data for each number, either entered number or resulting number.
struct NumberInfo
{
	NumberInfo(long value = 0, size_t order = 0, std::string description = "");
	void clear();

	long value;
	int parity;   // 0 (any), 1 (odd), 2 (even)
	size_t order;
	std::string description;
};

/**
	@class Calculator
	@brief Gets operands and operators from the user and computes results.

	Calculator asks for different input from the user (one operator and two operands).
	It stores all the operands and the result of the operation together with some 
	number information. When exiting the application, this data is printed to screen.
*/
class Calculator
{
	std::vector<NumberInfo> numbersInfo;
	std::string filePath;
	size_t count;

	long getInputNumber() const;
	std::string getExtension(const std::string& filename) const;

public:
	Calculator(std::string filePath = "");
	~Calculator();

	void showOptions() const;
	long getOption() const;
	long getValue(std::string adjective = "an");
	void compute(long value1, long value2, long symbol);
	void printRecords(Parity parity = Parity::any) const;
	void printRecord(size_t orderPos) const;
	void clear();
};

#endif
