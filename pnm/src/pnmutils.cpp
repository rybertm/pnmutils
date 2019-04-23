#include "pnmutils.h"

#include <iostream>
#include <fstream>	//	filestream - input/output operations on files streams
#include <sstream>	//	stringstream - input/output operations on string streams
#include <string>	//	stoi - string to integer

PNM::PNM()
	: m_width(0), m_height(0), m_type(0), m_max_val(255)
{
}

PNM::~PNM()
{
}

unsigned char* PNM::loadFile(const char* file_path)
{
	unsigned char* data = nullptr;

	free(data);

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
		data = readContent(image_content, buff, index);
	}
	catch (const std::ifstream::failure & e)
	{
		std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_READ(" << e.what() << ")\n" << e.code();
		return nullptr;
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << "ERROR::INVALID_ARGUMENT(" << e.what() << ")\n";
		return nullptr;
	}

	std::cerr << "Loaded\n";

	return data;
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

void PNM::free(unsigned char* data)
{
	if (data)
		delete[] data;

	m_type = 0;
	m_width = 0;
	m_height = 0;
	m_max_val = 255;
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

unsigned char* PNM::readContent(const std::string& image_content, std::string& buff, unsigned int& index)
{
	try
	{
		int row, col;
		unsigned char tmp[3] = { 0, 0, 0 };

		unsigned char* data = new unsigned char[m_width * m_height * 3];

		int offset = m_width * m_height;

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
					data[row * m_width + col] = tmp[0];
					data[row * m_width + col + offset] = tmp[0];
					data[row * m_width + col + (2 * offset)] = tmp[0];
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

					data[row * m_width + col] = tmp[0];
					data[row * m_width + col + offset] = tmp[0];
					data[row * m_width + col + (2 * offset)] = tmp[0];
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

					data[row * m_width + col] = tmp[0];
					data[row * m_width + col + offset] = tmp[1];
					data[row * m_width + col + (2 * offset)] = tmp[2];
					break;
				}

			}
		}

		return data;
	}
	catch (const std::invalid_argument & e)
	{
		throw;
	}
}