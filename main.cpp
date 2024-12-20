#include "HuffmanTree.hpp"
#include <iostream>
#include <string>

int main()
{
	HuffmanTree h;
	bool active = true;
	std::string input;
	std::string compressed;
	std::string serialized;
	std::string decoded;

	while (active)
	{
		std::cout << "\nOperation Codes:\n"
			<< "1: Encode string.\n"
			<< "2: Decode string.\n"
			<< "3: Exit program.\n"
			<< "\nEnter desired operation code: ";
		int code;
		std::cin >> code;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		switch (code)
		{
		case 1:
			std::cout << "\nEnter string for encoding: ";
			std::getline(std::cin, input);

			compressed = h.compress(input);
			serialized = h.serializeTree();

			std::cout << "\nCompressed string: " << compressed << std::endl;
			std::cout << "\nSerialized tree: " << serialized << std::endl;

			break;

		case 2:
			if (compressed.empty() || serialized.empty())
			{
				std::cout << "Error: No encoded string or serialized tree." << std::endl;
			}
			else
			{
				decoded = h.decompress(compressed, serialized);
				std::cout << "Decoded string: " << decoded << std::endl;
			}

			break;

		case 3:
			active = false;
			std::cout << "Program exited." << std::endl;

			break;

		default:
			std::cout << "Error: Invalid operation code." << std::endl;
		}
	}
	
	return 0;
}