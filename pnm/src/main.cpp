#include "pnm.h"
#include "visualizer.h"

#include <GL/freeglut.h>

#include <iostream>

int main(int argc, char** argv)
{
	if (argv[1])
	{
		PNM parser;

		parser.readFile(argv[1]);
		parser.readFile(argv[1]);

		parser.getRGB(&visu::matrixR, &visu::matrixG, &visu::matrixB);

		visu::init(argc, argv);
		visu::createWindow(parser.getWidth(), parser.getHeight(), parser.getGrad(), argv[1]);

		glutMainLoop();
		visu::reset();
	}
	else
	{
		std::cerr << "No PNM file provided. Aborting\n";
		std::cin.get();
		return -1;
	}
	return 0;
}