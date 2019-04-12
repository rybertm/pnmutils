#include "pnm.h"

#include <iostream>
#include <fstream>	//	filestream - input/output operations on files
#include <sstream>	//	stringstream - in
#include <string>

PNM::PNM()
	: m_matrixR(nullptr), m_matrixG(nullptr), m_matrixB(nullptr),
	m_width(0), m_height(0), m_type(0), m_max_grad(0)
{
}

PNM::~PNM()
{
	if(m_matrixR)
		delete[] m_matrixR;
	if (m_matrixG)
		delete[] m_matrixG;
	if (m_matrixB)
		delete[] m_matrixB;
}

int PNM::readFile(const char * file_path)
{
	std::ifstream image_file;
	std::string image_content;
	std::string buff;
	unsigned int index = 0;

	image_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::cerr << "\nLoading image [" << file_path << "]: \n";

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
	
	return 0;
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
		else if (image_content[index] == ' ')
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
	int row, col, tmp, count = 1;
	m_matrixR = new int[m_width * m_height * sizeof(int)];
	m_matrixG = new int[m_width * m_height * sizeof(int)];
	m_matrixB = new int[m_width * m_height * sizeof(int)];

	for (row = 0; row < m_height; row++)
	{
		for (col = 0; col < m_width; col++)
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

			tmp = std::stoi(buff);

			index++;
			switch (m_type)
			{
			case 1:
				if (tmp == 0)
					tmp = 255;
				else
					tmp = 0;

				m_matrixR[row*m_height + col] = tmp;
				m_matrixG[row*m_height + col] = tmp;
				m_matrixB[row*m_height + col] = tmp;
				break;
			case 2:
				m_matrixR[row*m_height + col] = tmp;
				m_matrixG[row*m_height + col] = tmp;
				m_matrixB[row*m_height + col] = tmp;
				break;
			case 3:
				if (count % 3 == 0)
					m_matrixB[row*m_height + col] = tmp;
				if (count % 3 == 1)
					m_matrixR[row*m_height + col] = tmp;
				if (count % 3 == 2)
					m_matrixG[row*m_height + col] = tmp;

				count++;
				break;
			}
			
		}
		count = 1;
	}	
}
