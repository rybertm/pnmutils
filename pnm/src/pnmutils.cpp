#include "pnmutils.h"

#include <GL/freeglut.h>

#include <iostream>
#include <fstream>	//	filestream - input/output operations on files streams
#include <sstream>	//	stringstream - input/output operations on string streams
#include <string>	//	stoi - string to integer
#include <functional>

PNM::PNM()
	: m_matrixR(nullptr), m_matrixG(nullptr), m_matrixB(nullptr),
	m_width(0), m_height(0), m_type(0), m_max_val(255), initialized(false),
	m_xPos(15), m_yPos(100)
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
		m_file_name = file_path;

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

			readHeader(image_content, buff, index);
			readContent(image_content, buff, index);
		}
		catch (const std::ifstream::failure& e)
		{
			std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_READ(" << e.what() << ")\n" << e.code();
			return -1;
		}
		catch (const std::invalid_argument & e)
		{
			std::cerr << "ERROR::INVALID_ARGUMENT(" << e.what() << ")\n";
			return -1;
		}

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

int PNM::getMaxVal()
{
	return m_max_val;
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

void PNM::drawImage(void(*processFunc)(), std::string windowTitle)
{
	if (initialized)
	{
		std::string title;
		if (windowTitle.find("\\0") == std::string::npos)
			title = m_file_name + " (" + windowTitle + ')';
		else
			title = m_file_name;

		glutInitWindowPosition(m_xPos, m_yPos);

		glutInitWindowSize(m_width, m_height);
		glutCreateWindow(title.data());

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
	m_max_val	=	0;
	m_file_name.clear();
}

void PNM::setWindowPos(const int xPos, const int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
}

void PNM::readHeader(const std::string& image_content, std::string& buff, unsigned int& index)
{
	try
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
			else if (image_content[index] <= ' ')
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
				while (image_content[index] <= ' ')
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
					while (image_content[index] <= ' ')
						index++;

					buff.clear();
					while (image_content[index] != ' ' && image_content[index] != '\n')
					{
						buff.push_back(image_content[index++]);
					}

					m_max_val = std::stoi(buff);

					index++;
				}

				break;
			}
		}
	}
	catch (const std::invalid_argument & e)
	{
		throw;
	}
}

void PNM::readContent(const std::string& image_content, std::string& buff, unsigned int& index)
{
	try
	{
		//	row - row, col - col
		int row, col, tmp[3] = { -1, -1, -1 };

		m_matrixR = new int[m_width * m_height * sizeof(int)];
		m_matrixG = new int[m_width * m_height * sizeof(int)];
		m_matrixB = new int[m_width * m_height * sizeof(int)];

		for (row = 0; row < m_height; row++)
		{
			for (col = 0; col < m_width; col++)
			{
				switch (m_type)
				{
				case 1:
					// ---START: READ NUMBER---
					buff.clear();

					// ignore whitespaces or new line before a number, if any
					while (image_content[index] <= ' ')
						index++;

					buff.push_back(image_content[index]);
					
					tmp[0] = std::stoi(buff);	//	convert string to integer

					index++;
					// ---END: READ NUMBER---

					if (tmp[0] == 0)
						tmp[0] = 255;
					else
						tmp[0] = 0;

					/*
						Given matrix[row][col] and matrix[row*col], they have row*col elements each.
						Matrix[row][col] has col in width.

						Assuming they were filled with the same elements.

						index = row * width + col;

						row = 0, col = 0; --- index = 0 --- first element, matrix[0][0] = matrix[0]
						row = 0, col = 1; --- index = 1 --- second element, matrix[0][1] = matrix[1]
						row = 0, col = 5; --- index = 5 --- sixth element, matrix[0][5] = matrix[5]

						row = 1, col = 0; --- index = 6 --- seventh element, matrix[1][0] = matrix[6]
						row = 1, col = 2; --- index = 8 --- ninth element, matrix[1][2] = matrix[8]
						row = 1, col = 4; --- index = 10 ---  eleventh element, matrix[1][4] = matrix[10]

						row = 3, col = 3; --- index = 21 --- twenty-second element, matrix[3][3] = matrix[21]

						row = 4, col = 5; --- index = 29 --- thirtieth element, matrix[4][5] = matrix[29]
						.
						.
						.
						row = r, col = c; --- index = N --- (N+1)th element, matrix[r][c] = matrix[N]
					*/
					m_matrixR[row * m_width + col] = tmp[0];
					m_matrixG[row * m_width + col] = tmp[0];
					m_matrixB[row * m_width + col] = tmp[0];
					break;
				case 2:
					// ---START: READ NUMBER---
					buff.clear();

					// ignore whitespaces or new line before a number, if any
					while (image_content[index] <= ' ')
						index++;

					//	-	if the number has more than 1 digit keep adding the digit(s) to the string
					while (image_content[index] != ' ' && image_content[index] != '\n' &&
						index != image_content.length())
					{
						buff.push_back(image_content[index++]);
					}

					tmp[0] = std::stoi(buff);	//	convert string to integer

					// convert to match the maximum color value
					if (m_max_val != 255)
						tmp[0] = (double)tmp[0] / m_max_val * 255;

					index++;
					// ---END: READ NUMBER---

					m_matrixR[row * m_width + col] = tmp[0];
					m_matrixG[row * m_width + col] = tmp[0];
					m_matrixB[row * m_width + col] = tmp[0];
					break;
				case 3:
					// ---START: READ NUMBER---
					for (int i = 0; i < m_type; i++)
					{
						buff.clear();

						// ignore whitespaces or new line before a number, if any
						while (image_content[index] <= ' ')
							index++;

						//	-	if the number has more than 1 digit keep adding the digit(s) to the string
						while (image_content[index] != ' ' && image_content[index] != '\n' &&
							index != image_content.length())
						{
							buff.push_back(image_content[index++]);
						}

						tmp[i] = std::stoi(buff);	//	convert string to integer

						// convert to match the maximum color value
						if (m_max_val != 255)
							tmp[i] = (double)tmp[i] / m_max_val * 255;

						index++;
					}
					// ---END: READ NUMBER---

					m_matrixR[row * m_width + col] = tmp[0];
					m_matrixG[row * m_width + col] = tmp[1];
					m_matrixB[row * m_width + col] = tmp[2];
					break;
				}

			}
		}
	}
	catch (const std::invalid_argument& e)
	{
		throw;
	}
}