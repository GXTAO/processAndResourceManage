#include "Tool.h"
#include <iostream>
#include <ctime>

bool Tool::rotate = false;

Tool::Tool(void)
{
}


Tool::~Tool(void)
{
}

/************************************************************************/
/* �ַ����ָ
	s:Դ�ַ���
	delim:�ָ��
	ret:�����ָ�󷵻صĶ���ַ���*/
/************************************************************************/
void Tool::split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
{  
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		ret->push_back(s.substr(last,index-last));  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index-last>0)  
	{  
		ret->push_back(s.substr(last,index-last));  
	}  
}  

/************************************************************************/
/* ������������½������������Դ������*/
/************************************************************************/
int Tool::resourceRandom() {
	if(!rotate) {
		srand((unsigned)time(NULL)+10);
		rotate = true;
	}
	else {
		srand((unsigned)time(NULL)+100);
		rotate = false;
	}
	return rand() % 11;
}