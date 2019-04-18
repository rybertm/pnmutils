#include "pnmutils.h"

#include <GL/freeglut.h>

#include <iostream>
#include <fstream>	//	filestream - input/output operations on files streams
#include <sstream>	//	stringstream - input/output operations on string streams
#include <string>
#include <functional>

PNM::PNM()
	: m_matrixR(nullptr), m_matrixG(nullptr), m_matrixB(nullptr),
	m_width(0), m_height(0), m_type(0), m_max_grad(0), initialized(false)
{
}

PNM::~PNM()
{
	reset();
}

int PNM::loadFile(const char * file_path)
{
	// If there is a file already in memory then it resets everything and read the new file
	if (!m_type)
	{
		m_title = file_path;

		std::ifstream image_file;
		std::string image_content;
		std::string buff;
		unsigned int index = 0;

		image_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::cerr << "\nLoading image [" << file_path << "]: ";

		try
		{
			std::ostringstream image_stream;

			image_file.open(file_path, std::ios_base::in);

			image_stream << image_file.rdbuf();

			image_file.close();

			image_content = image_stream.str();
		}
		catch (const std::ifstream::failure& e)
		{
			std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_READ" << "\n";
			return -1;
		}

		readHeader(image_content, buff, index);
		readContent(image_content, buff, index);

		std::cerr << "Loaded\n";
	}
	else
	{
		reset();
		loadFile(file_path);
	}

	return 0;
}

int PNM::getWidth()
{
	return m_width;
}

int PNM::getHeight()
{
	return m_height;
}

int PNM::getGrad()
{
	return m_max_grad;
}

int PNM::at(char mat, int index)
{
	if (m_matrixR)
	{
		if (mat == 'R' || mat == 'r')
			return m_matrixR[index];
		else if (mat == 'G' || mat == 'g')
			return m_matrixG[index];
		else if (mat == 'B' || mat == 'b')
			return m_matrixB[index];
		else
			return -1;
	}
	else
		return -1;
}

void PNM::init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	initialized = true;
}

void PNM::drawImage(void(*processFunc)())
{
	if (initialized)
	{
		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 640) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 480) / 2);

		glutInitWindowSize(m_width, m_height);
		glutCreateWindow(m_title.data());

		glutDisplayFunc(processFunc);

		gluOrtho2D(0, m_width, 0, m_height);
	}
	else
	{
		std::cout << "\nMust run init() before drawing.\nAborting...\n";
		return;
	}
}

void PNM::reset()
{
	if (m_matrixR)
		delete[] m_matrixR;
	if (m_matrixG)
		delete[] m_matrixG;
	if (m_matrixB)
		delete[] m_matrixB;

	m_matrixR = nullptr;
	m_matrixG = nullptr;
	m_matrixB = nullptr;

	m_type		=	0;
	m_width		=	0;
	m_height	=	0;
	m_max_grad	=	0;
	m_title.clear();
}

void PNM::readHeader(const std::string& image_content, std::string& buff, unsigned int& index)
{
	int pos = 0;

	buff.push_back(image_content[1]);

	m_type = std::stoi(buff);

	index += 3;

	while (index < image_content.length())
	{
		if (image_content[index] == '#')
		{
			pos = image_content.find('\n', index);

			index += pos - index + 1;
		}
		else if (image_content[index] == ' ' || image_content[index] == '\n')
		{
			index++;
			continue;
		}
		else
		{
			buff.clear();
			
			while (image_content[index] != ' ' && image_content[index] != '\n')
				{
					buff.push_back(image_content[index++]);
				}
			m_width = std::stoi(buff);

			index++;

			// ignore whitespaces or new line before a number, if any
			while (image_content[index] == ' ' || image_content[index] == '\n')
				index++;

			buff.clear();
			while (image_content[index] != ' ' && image_content[index] != '\n')
			{
				buff.push_back(image_content[index++]);
			}

			m_height = std::stoi(buff);

			index++;
			
			if (m_type != 1)
			{
				// ignore whitespaces or new line before a number, if any
				while (image_content[index] == ' ' || image_content[index] == '\n')
					index++;

				buff.clear();
				while (image_content[index] != ' ' && image_content[index] != '\n')
				{
					buff.push_back(image_content[index++]);
				}

				m_max_grad = std::stoi(buff);

				index++;
			}

			break;
		}
	}
}

void PNM::readContent(const std::string& image_content, std::string& buff, unsigned int& index)
{
	int row, col, tmp[3] = { -1, -1, -1 };

	m_matrixR = new int[m_width * m_height * sizeof(int)];
	m_matrixG = new int[m_width * m_height * sizeof(int)];
	m_matrixB = new int[m_width * m_height * sizeof(int)];

	for (row = 0; row < m_height; row++)
	{
		for (col = 0; col < m_width; col++)
		{
			// if the pnm file is type 1 or 2 it uses the same number for every matrix
			if (m_type != 3)
			{
				buff.clear();

				// ignore whitespaces or new line before a number, if any
				while (image_content[index] == ' ' || image_content[index] == '\n')
					index++;

				while (image_content[index] != ' ' && image_content[index] != '\n' &&
					index != image_content.length())
				{
					buff.push_back(image_content[index++]);
				}

				tmp[0] = std::stoi(buff);

				index++;
			}
			// if the pnm file is type 3 it uses a different number for each matrix
			else
			{
				for (int i = 0; i < m_type; i++)
				{
					buff.clear();

					// ignore whitespaces or new line before a number, if any
					while (image_content[index] == ' ' || image_content[index] == '\n')
						index++;

					while (image_content[index] != ' ' && image_content[index] != '\n' &&
						index != image_content.length())
					{
						buff.push_back(image_content[index++]);
					}

					tmp[i] = std::stoi(buff);

					index++;
				}
			}

			switch (m_type)
			{
			case 1:
				if (tmp[0] == 0)
					tmp[0] = 255;
				else
					tmp[0] = 0;

				m_matrixR[row*m_height + col] = tmp[0];
				m_matrixG[row*m_height + col] = tmp[0];
				m_matrixB[row*m_height + col] = tmp[0];
				break;
			case 2:
				m_matrixR[row*m_height + col] = tmp[0];
				m_matrixG[row*m_height + col] = tmp[0];
				m_matrixB[row*m_height + col] = tmp[0];
				break;
			case 3:
				m_matrixR[row*m_height + col] = tmp[0];
				m_matrixG[row*m_height + col] = tmp[1];
				m_matrixB[row*m_height + col] = tmp[2];
				break;
			}
			
		}
	}	
}

//void PNM::originalImage()
//{
//	int col, row, r, c;
//	int R, G, B;
//	glClearColor(1.0, 1.0, 1.0, 0); //Especifica um cor para o fundo
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_POINTS);
//	for (row = m_height - 1, r = 0; row >= 0; row--, r++)
//	{
//		for (col = 0, c = 0; col < m_width; col++, c++)
//		{
//			R = m_matrixR[r * m_height + c];
//			G = m_matrixG[r * m_height + c];
//			B = m_matrixB[r * m_height + c];
//
//			if (m_max_grad != 255)
//			{
//				R = (double)R / m_max_grad * 255;
//				G = (double)G / m_max_grad * 255;
//				B = (double)B / m_max_grad * 255;
//			}
//
//			glColor3ub(R, G, B);
//			glVertex2i(col, row);
//		}
//	}
//	glEnd();
//	glFlush();
//}
