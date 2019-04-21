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
	int		getMaxVal();
	int		at(char mat, int index);
	void	reset();

	void setWindowPos(const int xPos, const int yPos);

	//	Visualizer
	void init(int argc, char** argv);
	void drawImage(void(*processFunc)(), std::string windowTitle = "\\0");

private:
	bool initialized;

	std::string	m_file_name;
	int			m_type;
	int			m_width;
	int			m_height;
	int			m_max_val;
	int*		m_matrixR;
	int*		m_matrixG;
	int*		m_matrixB;

	// Visualization
	int m_xPos;
	int m_yPos;

	//	Parser
	void	readHeader(const std::string& image_content, std::string& buff, unsigned int& index);
	void	readContent(const std::string& image_content, std::string& buff, unsigned int& index);
};

#endif	//PNMUTILS_H_INCLUDED