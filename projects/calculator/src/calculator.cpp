#include "calculator.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

NumberInfo::NumberInfo(long value, size_t order, std::string description)
	: value(value), order(order), description(description)
{
	if(value % 2)
		parity = Parity::odd;
	else
		parity = Parity::even;
}

void NumberInfo::clear()
{
	value = 0;
	order = 0;
	parity = Parity::even;
	description = "";
}

Calculator::Calculator(std::string filePath)
	: count(0), filePath(filePath)
{
	if (!filePath.size()) return;

	std::ifstream iFile;
	std::string ext = getExtension(filePath);

	if (ext == ".txt")
	{
		iFile.open(filePath);
		if(iFile.is_open())
		{
			std::string line;
			NumberInfo entry;
			while (std::getline(iFile, line))
			{
				std::istringstream iss(line);
				iss >> entry.value >> entry.parity >> entry.order;
				std::getline(iss >> std::ws, entry.description);

				numbersInfo.push_back(entry);
				entry.clear();
			}
		}
	}
	else if (ext == ".bin")
	{
		iFile.open(filePath, std::ios_base::binary);
		if(iFile.is_open())
		{
			NumberInfo entry;
			size_t strLength;
			for(;;)
			{
				if(!iFile.read(reinterpret_cast<char*>(&entry.value), sizeof(entry.value))) break;
				iFile.read(reinterpret_cast<char*>(&entry.parity), sizeof(entry.parity));
				iFile.read(reinterpret_cast<char*>(&entry.order), sizeof(entry.order));
				iFile.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
				entry.description.resize(strLength);
				iFile.read(&entry.description[0], strLength);

				numbersInfo.push_back(entry);
				entry.clear();
			}
		}
	}

	if (iFile.is_open()) iFile.close();
	else std::cout << "Input file cannot be opened\n";
	
	count = numbersInfo.size();
}

Calculator::~Calculator()
{
	if (!filePath.size()) return;

	std::ofstream oFile;
	std::string ext = getExtension(filePath);

	if(ext == ".txt")
	{
		oFile.open(filePath);
		if(oFile.is_open())
			for (const auto& record : numbersInfo)
				oFile 
					<< std::to_string(record.value) << ' '
					<< std::to_string(record.parity) << ' '
					<< std::to_string(record.order) << ' '
					<< record.description << '\n';
	}
	else if (ext == ".bin")
	{
		size_t strLength;
		oFile.open(filePath, std::ios_base::binary);
		if(oFile.is_open())
			for (const auto& record : numbersInfo)
			{
				oFile.write(reinterpret_cast<const char*>(&record.value), sizeof(record.value));
				oFile.write(reinterpret_cast<const char*>(&record.parity), sizeof(record.parity));
				oFile.write(reinterpret_cast<const char*>(&record.order), sizeof(record.order));
				strLength = record.description.size();
				oFile.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));
				oFile.write(record.description.c_str(), strLength);                   // then content
			}
	}

	if(oFile.is_open()) oFile.close();
	else std::cout << "Output file cannot be opened\n";
}

long Calculator::getInputNumber() const
{
	std::string line;

	if (!std::getline(std::cin, line))   // Get input line
		throw std::runtime_error("Failure when reading from std::cin.");

	std::istringstream iss(line);
	iss >> line;   // Get first word of the line
	
	return std::stol(line);
}

std::string Calculator::getExtension(const std::string& filename) const
{
	size_t pos = filename.rfind('.');
	if(pos == std::string::npos) return "";

	return filename.substr(pos);
}

long Calculator::getOption() const
{
	long option;
	do
	{
		std::cout << "Select an available option (1-5): ";
		option = getInputNumber();
	} while (option < 1 || option > 5);

	return option;
}

long Calculator::getValue(std::string adjective)
{
	std::cout << "Enter " << adjective << " operand: ";
	long value = getInputNumber();

	numbersInfo.push_back(NumberInfo(value, ++count));
	return value;
}

void Calculator::printRecords(Parity parity) const
{
	std::cout
		<< "All"
		<< (parity == Parity::even ? " even " : parity == Parity::odd ? " odd " : " ")
		<< "recorded entries and results (value, parity, order, description):\n";

	if(!numbersInfo.size()) std::cout << "No records stored\n";

	std::cout << std::left;
	for (size_t i = 0; i < numbersInfo.size(); i++)
		if(parity == any || parity == numbersInfo[i].parity)
			std::cout 
			<< std::setw(8) << numbersInfo[i].value << " " 
			<< std::setw(4) << numbersInfo[i].parity << " " 
			<< std::setw(4) << numbersInfo[i].order << " "
			<< std::setw(20) << numbersInfo[i].description << "\n";
}

void Calculator::printRecord(size_t orderPos) const
{
	std::cout << "Entry number " << orderPos << " (value, parity, order, description):\n";

	if(orderPos >= numbersInfo.size())
		std::cout << "Non-existent record\n";
	else
		std::cout 
			<< std::setw(8) << numbersInfo[orderPos].value << " " 
			<< std::setw(4) << numbersInfo[orderPos].parity << " " 
			<< std::setw(4) << numbersInfo[orderPos].order << " "
			<< std::setw(20) << numbersInfo[orderPos].description << "\n";
}

void Calculator::showOptions() const
{
	std::cout
		<< "Working directory: " << std::filesystem::current_path() << "\n\n"
		<< "Available options:\n"
		<< "  1. Addition (+)\n"
		<< "  2. Subtraction (-)\n"
		<< "  3. Multiplication (*)\n"
		<< "  4. Division (/)\n"
		<< "  5. Exit\n\n";
}

void Calculator::compute(long value1, long value2, long symbol)
{
	long result;
	std::string symbolStr;

	switch (symbol)
	{
		case 1:
			result = value1 + value2;
			symbolStr = " + ";
			break;
		case 2:
			result = value1 - value2;
			symbolStr = " - ";
			break;
		case 3:
			result = value1 * value2;
			symbolStr = " * ";
			break;
		case 4:
			result = value1 / value2;
			symbolStr = " / ";
			break;
		default:
			std::cout << "Invalid operator\n";
			return;
	}

	std::string description(std::to_string(value1) + symbolStr + std::to_string(value2) + " = " + std::to_string(result));
	numbersInfo.push_back(NumberInfo(result, ++count, description));
}

void Calculator::clear()
{
	numbersInfo.clear();
	count = 0;
}