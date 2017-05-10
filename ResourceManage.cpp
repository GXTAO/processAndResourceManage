#include <iostream>
#include "ResourceManage.h"
#include "Resource.h"
#include "ResourceA.h"
#include "ResourceB.h"
#include "ProcessManage.h"
#include "PCB.h"
#include "MyProcess.h"

Resource* ResourceManage::resourceA = new ResourceA();
Resource* ResourceManage::resourceB = new ResourceB();
int ResourceManage::remainA = resourceA->getAmount();
int ResourceManage::remainB = resourceB->getAmount();

ResourceManage::ResourceManage(void)
{
}


ResourceManage::~ResourceManage(void)
{
}

/************************************************************************/
/* ����ϵͳ��Դ��
/************************************************************************/
bool ResourceManage::requestResource() {
	//std::cout << ">>������Դ��" << name << std::endl;
	ProcessManage::schedule();

	return true;
}

bool ResourceManage::requestResource(std::string name) {
	//std::cout << ">>������Դ��" << name << std::endl;
	ProcessManage::schedule();

	return true;
}

bool ResourceManage::requestResource(std::string name, int count) {
	std::cout << ">>������Դ��" << name << "," << count << "��" << std::endl;

	return true;
}

/************************************************************************/
/* ������Դ���룬����ɹ�����Դ����������Ӧ�ı仯:
	countA:��ԴA����������
	countB:��ԴB����������
	isGet:�Ƿ�����Դ����������Ӧ�ı仯*/
/************************************************************************/
bool ResourceManage::apply(const int& countA, const int& countB, bool isGet) {
	if(countA <= remainA && countB <= remainB) {
		if(isGet) {
			remainA -= countA;
			remainB -= countB;
		}
		return true;
	}

	return false;
}

/************************************************************************/
/* �ͷŶ�Ӧ��Դ��
/************************************************************************/
bool ResourceManage::releaseResource() {
	//��ȡ��ǰ���н��̵���Դ��Ŀ,���ͷ�
	int runA = -1;
	int runB = -1;
	ProcessManage::getRunningResource(runA, runB);
	remainA += runA;
	remainB += runB;

	//���µ��Ƚ���
	ProcessManage::schedule();

	return true;
}

/************************************************************************/
/* �ͷŶ�Ӧ��Դ��
	pcb:Ҫ�ͷ���Դ�Ľ���*/
/************************************************************************/
bool ResourceManage::releaseResource(PCB* pcb) {
	//���˽����Ƿ���ռ����Դ,û����Դռ��ֱ�ӷ���
	if(!pcb->getState()) {
		return false;
	}

	//��ȡ��ǰ���н��̵���Դ��Ŀ,���ͷ�
	int runA = -1;
	int runB = -1;
	pcb->getProcess()->getResourceNeed(runA, runB);
	remainA += runA;
	remainB += runB;

	std::cout << ">>���� " << pcb->getProcess()->getName() << " ��Դ�ͷųɹ�" << std::endl;

	return true;
}

/************************************************************************/
/* �鿴ϵͳ��Դ��Ϣ��*/
/************************************************************************/
bool ResourceManage::listResource() {
	std::cout << ">>ϵͳ��Դ��Ϣ���£�" << std::endl;
	std::cout << "ResourceA:amount=" << resourceA->getAmount() << " remain=" << remainA << std::endl;
	std::cout << "ResourceB:amount=" << resourceB->getAmount() << " remain=" << remainB << std::endl;

	return true;
}