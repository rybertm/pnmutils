#ifndef	PNM_H_INCLUDED
#define	PNM_H_INCLUDED

#include <string>

class PNM
{
public:
	 PNM();
	~PNM();

	int		loadFile(const char* file_path);
	int		getWidth();
	int		getHeight();
	int		getGrad();
	void	getRGB(int** matrixR, int** matrixG, int** matrixB);
	void	reset();

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