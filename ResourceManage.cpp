#include <iostream>
#include "ResourceManage.h"


ResourceManage::ResourceManage(void)
{
}


ResourceManage::~ResourceManage(void)
{
}

/************************************************************************/
/* ����ϵͳ��Դ��
	name:��Դ��
	count��������Դ����*/
/************************************************************************/
bool ResourceManage::requestResource(std::string name, int count) {
	std::cout << ">>������Դ��" << name << "," << count << "��" << std::endl;

	return true;
}

/************************************************************************/
/* �ͷŶ�Ӧ��Դ��
	name���ͷŵ���Դ��*/
/************************************************************************/
bool ResourceManage::releaseResource(std::string name) {
	std::cout << ">>�ͷ���Դ��" << name << std::endl;

	return true;
}

/************************************************************************/
/* �鿴ϵͳ��Դ��Ϣ��*/
/************************************************************************/
bool ResourceManage::listResource() {
	std::cout << ">>ϵͳ��Դ��Ϣ���£�" << std::endl;

	return true;
}