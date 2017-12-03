#include "stdafx.h"
#include <iostream>

// TODO: remove that ugly thing
using namespace nope::dts::parser;

int main(int ac, char **av)
{
	try
	{
		for (int i = 1; i < ac; ++i)
		{
			Parser parser(av[i]);

			parser.parse();

			std::cout << "OK" << std::flush;

			/*std::ofstream os("output.xml");

			os << parser.ast().xml();*/
		}
	}
	catch (nope::dts::parser::error::Syntax const &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}