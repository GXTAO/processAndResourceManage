#include "Shell.h"

#include <iostream>
#include "Tool.h"
#include "ProcessManage.h"
#include "ResourceManage.h"


Shell::Shell(void)
{
	ProcessManage::init();
	instruck = NULL;
}


Shell::~Shell(void)
{
}

/************************************************************************/
/* ��ȡ�û�ָ�������Ӧ������
	input���û������ָ��*/
/************************************************************************/
bool Shell::read(std::string s) {
	int input = 0;

	//�������ָ����зָ����
	instruck = new std::vector<std::string>;
	std::string delim = " ";//�ָ���Ϊ�ո�
	Tool::split(s, delim, instruck);

	//��ȡ����
	std::string command = instruck->at(0);

	//ƥ���ͬʱ����ָ�������⣨�Ƿ���ϱ�׼������Ƿ�ƥ�䣩
	//�˳�ϵͳ
	if(command == "q") {
		if(instruck->size() == 1) {
			return false;
		}
	}
	//��������
	else if(command == "cr") {
		//�ж��Ƿ����Ϊ������һ������ ID,һ���������ȼ�
		if(instruck->size() != 3) {
			std::cout << ">>�����������ṩ�����������ȼ��������������" << std::endl;
			std::cout << ">>";
		}
		else {
			std::string name = instruck->at(1);
			int priority = atoi(instruck->at(2).c_str());
			ProcessManage::createProcess(name, priority);
		}
	}
	//ɾ������
	else if(command == "de") {
		//�жϲ����Ƿ�Ϊһ����ɾ��������
		if(instruck->size() != 2) {
			std::cout << ">>ɾ���������ṩ�������������������" << std::endl;
			std::cout << ">>";
		}
		else {
			std::string name = instruck->at(1);
			ProcessManage::deleteProcess(name);
		}
	}
	//������Դ
	else if(command == "req") {
		//�жϲ����Ƿ�Ϊ��������Դ����������Դ����
		if(instruck->size() != 3) {
			std::cout << ">>�������ṩ��Դ���������������������" << std::endl;
			std::cout << ">>";
		}
		else {
			std::string name = instruck->at(1);
			int count = atoi(instruck->at(2).c_str());
			ResourceManage::requestResource();
		}
	}
	//�ͷ���Դ
	else if(command == "rel") {
		//�жϲ����Ƿ�û��
		if(instruck->size() != 1) {
			std::cout << ">>�������룬help�鿴ָ���������" << std::endl;
			std::cout << ">>";
		}
		else {
			ResourceManage::releaseResource();
		}
	}
	//ʱ���ж�
	else if(command == "to") {
		if(instruck->size() != 1) {
			std::cout << ">>�������룬help�鿴ָ���������" << std::endl;
			std::cout << ">>";
		}
		else {
			ProcessManage::timeInt();
		}
	}
	//�鿴������Ϣ
	else if(command == "list_p") {
		if(instruck->size() != 1) {
			std::cout << ">>�������룬help�鿴ָ���������" << std::endl;
			std::cout << ">>";
		}
		else {
			ProcessManage::listProcess();
		}
	}
	//�鿴ϵͳ��Դ��Ϣ
	else if(command == "list_r") {
		if(instruck->size() != 1) {
			std::cout << ">>�������룬help�鿴ָ���������" << std::endl;
			std::cout << ">>";
		}
		else {
			ResourceManage::listResource();
		}
	}
	//�������
	else {
		std::cout << ">>�������룬help�鿴ָ���������" << std::endl;
		std::cout << ">>";
	}

	//�ͷ���Ӧָ����Դ
	delete(instruck);

	return true;
}
