#pragma once
/************************************************************************/
/* ϵͳ��Դ����*/
/************************************************************************/

#include <string>

class ResourceManage
{
public:
	ResourceManage(void);
	~ResourceManage(void);

	//Apply for resource
	static bool requestResource(std::string name, int count);
	//Release resource
	static bool releaseResource(std::string name);
	//Look resource information
	static bool listResource();
};

