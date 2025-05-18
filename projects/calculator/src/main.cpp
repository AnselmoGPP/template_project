#include <iostream>

#include "calculator.hpp"

int main()
{
    try
    {
        long option, value1, value2;

        Calculator calc("storedData.bin");
	    calc.showOptions();

	    for (;;)
	    {
	    	option = calc.getOption();
	    	if(option == 5) break;
	    	value1 = calc.getValue("first");
	    	value2 = calc.getValue("second");
	    	calc.compute(value1, value2, option);
	    }

	    calc.printRecords();
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    std::cin.get();
    return 0;
}
