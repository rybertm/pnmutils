#include "pnmutils.h"

#include <GL/freeglut.h>

#include <iostream>
#include <chrono>

void desenha_original();

PNM parser;

using namespace std::chrono;

int main(int argc, char** argv)
{
	auto start = high_resolution_clock::now();
	parser.loadFile(argv[1]);
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken by function: "
		<< duration.count() << " microseconds\n";

	parser.init(argc, argv);
	parser.drawImage(desenha_original);

	glutMainLoop();

	return 0;
}

void desenha_original()
{
	int col, row, r, c;
	int R, G, B;

	int width, height;

	width = parser.getWidth();
	height = parser.getHeight();
	
	glClearColor(1.0, 1.0, 1.0, 0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (row = height - 1, r = 0; row >= 0; row--, r++)
	{
		for (col = 0, c = 0; col < width; col++, c++)
		{
			R = parser.at('R', r * height + c);
			G = parser.at('G', r * height + c);
			B = parser.at('B', r * height + c);

			glColor3ub(R, G, B);
			glVertex2i(col, row);
		}
	}
	glEnd();
	glFlush();
} // Fim: desenha_original