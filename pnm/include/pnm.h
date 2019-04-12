#ifndef	PNM_H_INCLUDED
#define	PNM_H_INCLUDED

#include <fstream>	//	filestream - input/output operations on files

class PNM
{
public:
	 PNM();
	~PNM();

	int		readFile(const char* file_path);

private:
	int		m_type;
	int		m_width;
	int		m_height;
	int		m_max_grad;
	int*	m_matrixR;
	int*	m_matrixG;
	int*	m_matrixB;


	void	readHeader(const std::string& image_content, std::string& buff, unsigned int& index);
	void	readContent(const std::string& image_content, std::string& buff, unsigned int& index);
};

#endif	//PNM_H_INCLUDED