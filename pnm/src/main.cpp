#include "pnm.h"

#include <iostream>

int main()
{
	PNM parser;

	parser.readFile("samples/lena.ppm");
	std::cout << "Hello World!\n";
}