#pragma once
/************************************************************************/
/* �����ࣺ�ṩͨ�õĺ�������*/
/************************************************************************/

#include <string>
#include <vector>

class Tool
{
public:
	Tool(void);
	~Tool(void);

	//�ַ����ָ�
	static void split(std::string& s, std::string& delim, std::vector<std::string>* ret);
	//�½�������Դ�������������
	static int resourceRandom();

private:
	static bool rotate;//�������������ʱӦ�ò�ͬ�����������
};

