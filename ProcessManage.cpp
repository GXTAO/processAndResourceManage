#include <iostream>
#include "ProcessManage.h"
#include "MyProcess.h"
#include "PCB.h"

PCB* ProcessManage::queue0 = NULL;
PCB* ProcessManage::queue1 = NULL;
PCB* ProcessManage::queue2 = NULL;

PCB* ProcessManage::blockQueue = NULL;
PCB* ProcessManage::blockFirst = NULL;
PCB* ProcessManage::blockLast = NULL;

PCB* ProcessManage::readyQueue = NULL;
PCB* ProcessManage::readyFirst = NULL;
PCB* ProcessManage::readyLast = NULL;

ProcessManage::ProcessManage(void)
{
}


ProcessManage::~ProcessManage(void)
{
}


/************************************************************************/
/* ��ʼ�����̣�������ʼ����*/
/************************************************************************/
void ProcessManage::init() {
	MyProcess* process = new MyProcess("initProcess", 1, 0, 0);
	PCB* pcb = new PCB(process);
	pcb->setState(true);                                      //����״̬Ϊ����
	pcb->getProcess()->setState(1);                           //���ý���״̬Ϊ����
	queue0 = pcb;
	std::cout << ">>ϵͳ���� " << queue0->getProcess()->getName() << " �Ѵ�������" << std::endl;
	std::cout << ">>ϵͳ����������������Ӧָ����в���" << std::endl;
	std::cout << ">>";
}


/************************************************************************/
/* �������̣�
	name���½�������
	priority���½��������ȼ�*/
/************************************************************************/
bool ProcessManage::createProcess(std::string name, int priority) {
	int rA = -1;
	int rB = -1;

	MyProcess* process = new MyProcess(name, priority);
	process->getResourceNeed(rA, rB);
	PCB* pcb = new PCB(process);

	std::cout << ">>���̣� " << name << " �����ɹ������ȼ�Ϊ��" << priority 
		<< " ��ԴA��B������Ϊ:" << rA << "��" << rB  << std::endl;

	addReadyQueue(pcb);                                  //�½����̺���������������

	std::cout << ">>";

	return true;
}

/************************************************************************/
/* ɾ�����̣�
	name��ɾ���Ľ�����*/
/************************************************************************/
bool ProcessManage::deleteProcess(std::string name) {
	std::cout << ">>���̣� " << name << " ɾ���ɹ�" << std::endl;
	std::cout << ">>";

	return true;
}

/************************************************************************/
/* �鿴�����б�*/
/************************************************************************/
bool ProcessManage::listProcess() {
	std::cout << ">>�����б����£�" << std::endl;

	return true;
}

/************************************************************************/
/* ʱ���жϣ�*/
/************************************************************************/
bool ProcessManage::timeInt() {
	std::cout << ">>ʱ���ж�" << std::endl;

	return true;
}

/************************************************************************/
/* ���̵��ȣ�*/
/************************************************************************/
void ProcessManage::schedule() {

}

void ProcessManage::schedule(PCB* pcb) {
	//���ȶ���Ҫ���ȵĽ��̵���Դ��������
	pcb->applyForResource();

	//�����������Դ׼�������������������
	if(!pcb->getState()) {
		addBlockQueue(pcb);
	}
	//���������Դ�ѻ��������������
	else {
		addReadyQueue(pcb);
	}
}

bool ProcessManage::addBlockQueue(PCB* pcb) {
	return true;
}

/************************************************************************/
/* ��ӽ��̽���������У�
	pcb����ӵĽ���*/
/************************************************************************/
bool ProcessManage::addReadyQueue(PCB* pcb) {
	if(readyQueue == NULL) {                          //�����ǰ����������û�н���
		if(pcb->applyForResource()) {                 //������̵���Դ����ɹ�����������������
			readyQueue = pcb;                         //�����������
			readyLast = pcb;                          //ͬʱ��Ҳ�Ƕ�β����
			pcb->getProcess()->setState(1);           //���ý���Ϊ����״̬

			std::cout << ">>��ǰ�����н��̣����̷�����Դ�ɹ���ֱ�����н���" << std::endl;
		}
		else {                                        //�����Դ�������������������������
			addBlockQueue(pcb);
			std::cout << ">>��ǰ�����н��̣����̷�����Դʧ�ܣ�������������" << std::endl;
		}
	}
	//����������в�Ϊ�գ����¼���Ľ�����һ�����ȼ���飬����������ȼ�����ȡ����ǰ���н���
	else {
		if(readyQueue->getProcess()->getPriority() > pcb->getProcess()->getPriority()) {

			std::cout << ">>��ǰ���̣�" << pcb->getProcess()->getName() << " �ȵ�ǰ���н��� "
				<< readyQueue->getProcess()->getName() << " ��" << std::endl;

			if(pcb->applyForResource()) {                        //������̵���Դ����ɹ���������
				PCB* currentProcess = readyQueue;                //��ȡ��ǰ���н���
				currentProcess->getProcess()->setState(0);       //���õ�ǰ���н���Ϊ����������
				readyQueue = pcb;                                //���½��̲������
				pcb->setNext(currentProcess);                    //���׽���ָ����һ�����̣�����ӵĽ��̣�
				pcb->getProcess()->setState(1);                  //���ý���Ϊ����״̬

				std::cout << ">>��ǰ���̷�����Դ�ɹ���ֱ�����н���" << std::endl;
			}
			else {                                               //�����Դ�������������������������
				addBlockQueue(pcb);

				std::cout << ">>��ǰ���̷�����Դʧ�ܣ�������������" << std::endl;
			}
		}
		//����������ȼ�û�е�ǰ���н��̸ߣ������������̶�β
		else {
			PCB* lastProcess = readyLast;                        //��ȡ����������β������
			lastProcess->setNext(pcb);                           //�����β
			readyLast = pcb;                                     //ʹ�������ж�βָ��ָ������¼����β�Ľ���

			std::cout << ">>��ǰ���̣�" << pcb->getProcess()->getName() << " �ȵ�ǰ���н��� "
				<< readyQueue->getProcess()->getName() << " ��,�����������" << std::endl;
		}
	}

	return true;
}