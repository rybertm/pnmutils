#include "pnmutils.h"

#include <GL/freeglut.h>

#include <iostream>
#include <chrono>

void desenha_original();
void desenha_negativo();

PNM parser;

using namespace std::chrono;

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		auto start = high_resolution_clock::now();
		parser.loadFile(argv[1]);
		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(stop - start);

		std::cout << "Time taken by loadFile(): "
			<< duration.count() << " microseconds\n";

		parser.init(argc, argv);
		parser.drawImage(desenha_original);
		parser.setWindowPos(700, 100);
		parser.drawImage(desenha_negativo, "negativo");

		glutMainLoop();
	}
	else
	{
		std::cout << "Correct usage: pnmutils_test.exe < [pnm_image] ";
		std::cin.get();
	}

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
			R = parser.at('R', r * width + c);
			G = parser.at('G', r * width + c);
			B = parser.at('B', r * width + c);

			glColor3ub(R, G, B);
			glVertex2i(col, row);
		}
	}
	glEnd();
	glFlush();
} // Fim: desenha_original

void desenha_negativo()
{
	int col, row, r, c;
	int R, G, B;

	int width, height, grade;

	width = parser.getWidth();
	height = parser.getHeight();
	grade = parser.getMaxVal();

	glClearColor(1.0, 1.0, 1.0, 0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (row = height - 1, r = 0; row >= 0; row--, r++)
	{
		for (col = 0, c = 0; col < width; col++, c++)
		{
			R = parser.at('R', r * width + c);
			G = parser.at('G', r * width + c);
			B = parser.at('B', r * width + c);

			R = grade - R;
			G = grade - G;
			B = grade - B;

			glColor3ub(R, G, B);
			glVertex2i(col, row);
		}
	}
	glEnd();
	glFlush();
} // Fim: desenha_original