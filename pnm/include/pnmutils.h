#ifndef	PNMUTILS_H_INCLUDED
#define	PNMUTILS_H_INCLUDED

#include <string>
#include <functional>

class PNM
{
public:
	 PNM();
	~PNM();

	//	Parser
	int		loadFile(const char* file_path);

	//	Utility
	int		getWidth();
	int		getHeight();
	int		getGrad();
	int		at(char mat, int index);
	void	reset();

	//	Visualizer
	void init(int argc, char** argv);
	void drawImage(void(*processFunc)());

private:
	bool initialized;

	std::string	m_title;
	int			m_type;
	int			m_width;
	int			m_height;
	int			m_max_grad;
	int*		m_matrixR;
	int*		m_matrixG;
	int*		m_matrixB;

	//	Parser
	void	readHeader(const std::string& image_content, std::string& buff, unsigned int& index);
	void	readContent(const std::string& image_content, std::string& buff, unsigned int& index);
};

#endif	//PNMUTILS_H_INCLUDED