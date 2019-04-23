#ifndef	PNMUTILS_H_INCLUDED
#define	PNMUTILS_H_INCLUDED

#include <string>

class PNM
{
public:
	 PNM();
	~PNM();

	//	Parser
	unsigned char*	loadFile(const char* file_path);

	//	Utility
	int		getWidth();
	int		getHeight();
	int		getMaxVal();
	void	free(unsigned char* data);

private:
	int			m_type;
	int			m_width;
	int			m_height;
	int			m_max_val;

	//	Parser
	void			readHeader(const std::string& image_content, std::string& buff, unsigned int& index);
	unsigned char*	readContent(const std::string& image_content, std::string& buff, unsigned int& index);
};

#endif	//PNMUTILS_H_INCLUDED