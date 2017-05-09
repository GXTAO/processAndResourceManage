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

PCB* ProcessManage::runningProcess = NULL;
PCB* ProcessManage::initProcess = NULL;

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
	initProcess = pcb;
	std::cout << ">>ϵͳ���� " << queue0->getProcess()->getName() << " �Ѵ�������" << std::endl;
	std::cout << ">>ϵͳ����������������Ӧָ����в���" << std::endl;
}


/************************************************************************/
/* �������̣�
	name���½�������
	priority���½��������ȼ�*/
/************************************************************************/
bool ProcessManage::createProcess(std::string name, int priority) {
	//���ȼ���Ƿ��ظ�������ͬ���ƵĽ���
	PCB* isRepeat = NULL;
	isRepeat = find(name);
	if(isRepeat != NULL) {
		std::cout << ">>��ǰ�������Ѵ��ڣ�����������������" << std::endl;
		return false;
	}

	int rA = -1;
	int rB = -1;

	MyProcess* process = new MyProcess(name, priority);
	process->getResourceNeed(rA, rB);
	PCB* pcb = new PCB(process);

	std::cout << ">>���̣� " << name << " �����ɹ������ȼ�Ϊ��" << priority 
		<< " ��ԴA��B������Ϊ:" << rA << "��" << rB  << std::endl;

	//�����ǰ�����н��̣�����½�����Ϊ�����н��̵��ӽ��̣����û�У����ǳ�ʼ���̵��ӽ���
	if(runningProcess == NULL) {
		initProcess->setSon(pcb);
		pcb->setFather(initProcess);
	}
	else {
		runningProcess->setSon(pcb);
		pcb->setFather(runningProcess);
	}

    //�½����̺���������������
	addReadyQueue(pcb);

	return true;
}

/************************************************************************/
/* ɾ�����̣�
	name��ɾ���Ľ�����*/
/************************************************************************/
bool ProcessManage::deleteProcess(std::string name) {
	//�鿴�Ƿ�����������
	PCB* destroyProcess = find(name);
	if(destroyProcess == NULL) {
		std::cout << ">>ϵͳ���޴˽��̣���������" << std::endl;

		return true;
	}
	else {
		destroy(destroyProcess);
		schedule();
	}

	return true;
}

/************************************************************************/
/* ���ٽ��̣���ͬ���ӽ��̣�
	pcb�����ٵĽ���*/
/************************************************************************/
bool ProcessManage::destroy(PCB* pcb) {
	//���Ȳ鿴�˽����Ƿ����ӽ��̣�������ӽ��̣��������ӽ���
	if(pcb->getSon() != NULL) {
		destroy(pcb->getSon());
	}

	//���ٵ�ǰ����
	//ȥ����Ӧ�ĸ��ӹ�ϵ
	PCB* father = pcb->getFather();
	PCB* son = pcb->getSon();
	if(father != NULL && son == NULL) {
		father->setSon(NULL);
	}

	//ȥ��������Ϣ
	std::string name = pcb->getProcess()->getName();
	//��������Ѱ��
	PCB* current = readyQueue;
	while(current != NULL) {
		if(current->getProcess()->getName() == name) {
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//�����ǰ����λ�ڶ��ף����Ҳ�ֹһ������
			if(fore == NULL && next != NULL) {
				readyQueue = next;
				next->setFore(fore);
				runningProcess = NULL;

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�";
				std::cout << " �������еĽ��̱�ɾ��" << std::endl;
				return true;
			}
			//�����ǰ����λ�ڶ��ף�����ֻ��һ������
			else if(fore == NULL && next == NULL) {
				readyQueue = NULL;
				runningProcess = NULL;

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�";
				std::cout << " �������еĽ��̱�ɾ��" << std::endl;
				return true;
			}
			//�����ǰ����λ�ڶ�β
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�" << std::endl;
				return true;
			}
			//����λ���м�
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�" << std::endl;
				return true;
			}
		}
		else if(current->getNext() != NULL) {
			current = current->getNext();
		}
	}

	//��������Ѱ��
	current = blockQueue;
	while(current != NULL && pcb != NULL) {
		if(current->getProcess()->getName() == name) {
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//�����ǰ����λ�ڶ��ף����Ҳ�ֹһ������
			if(fore == NULL && next != NULL) {
				blockQueue = next;
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�";
				return true;
			}
			//�����ǰ����λ�ڶ��ף�����ֻ��һ������
			else if(fore == NULL && next == NULL) {
				blockQueue = NULL;

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�";
				return true;
			}
			//�����ǰ����λ�ڶ�β
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�" << std::endl;
				return true;
			}
			//����λ���м�
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>���̣� " << name << " ɾ���ɹ�" << std::endl;
				return true;
			}
		}
		else if(current->getNext() != NULL) {
			current = current->getNext();
		}
	}

	return false;
}

/************************************************************************/
/* ���ҽ��̣�
	name�������ҵĽ�����*/
/************************************************************************/
PCB* ProcessManage::find(std::string name) {
	PCB* findProcess = NULL;
	//�ȳ�������п�ʼ����
	findProcess = readyQueue;
	while(findProcess != NULL) {
		if(findProcess->getProcess()->getName() == name) {
			return findProcess;
		}
		else {
			if(findProcess->getNext() != NULL) {
				findProcess = findProcess->getNext();
			}
			else {
				break;
			}
		}
	}

	//�ٲ�����������
	findProcess = blockQueue;
	while(findProcess != NULL) {
		if(findProcess->getProcess()->getName() == name) {
			return findProcess;
		}
		else {
			if(findProcess->getNext() != NULL) {
				findProcess = findProcess->getNext();
			}
			else {
				break;
			}
		}
	}

	return NULL;
}

/************************************************************************/
/* �鿴�����б�*/
/************************************************************************/
bool ProcessManage::listProcess() {
	std::cout << ">>�����б����£�" << std::endl;

	if(runningProcess == NULL) {
		std::cout << ">>��ǰ�����н���" << std::endl;
	}
	else {
		std::cout << ">>��ǰ���н���Ϊ��" << runningProcess->getProcess()->getName() << std::endl;
	}

	PCB* current = readyQueue;
	if(current == NULL) {
		std::cout << ">>��������Ϊ�գ�û�о�������" << std::endl;
	}
	else {
		std::cout << ">>�������������½��̣�";
		while(current != NULL) {
			std::cout << current->getProcess()->getName() << " " << std::endl;
			current = current->getNext();
		}
	}

	current = blockQueue;
	if(current == NULL) {
		std::cout << ">>��������Ϊ�գ�û����������" << std::endl;
	}
	else {
		std::cout << ">>�������������½��̣�";
		while(current != NULL) {
			std::cout << current->getProcess()->getName() << " " << std::endl;
			current = current->getNext();
		}
	}

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
	//���ȼ�����������Ƿ���Ի����Դ������
	PCB* current = blockQueue;
	while(current != NULL) {
		//�����Դ���㣬���̽����������
		if(current->applyForResource()) {
			addReadyQueue(current);
			//�������������Ƴ�
			std::string name = current->getProcess()->getName();
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//�����ǰ����λ�ڶ��ף����Ҳ�ֹһ������
			if(fore == NULL && next != NULL) {
				blockQueue = next;
				next->setFore(fore);

				std::cout << ">>���̣� " << name << "�����������";
				return;
			}
			//�����ǰ����λ�ڶ��ף�����ֻ��һ������
			else if(fore == NULL && next == NULL) {
				blockQueue = NULL;

				std::cout << ">>���̣� " << name << "�����������";
				return;
			}
			//�����ǰ����λ�ڶ�β
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				std::cout << ">>���̣� " << name << "�����������" << std::endl;
				return;
			}
			//����λ���м�
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				std::cout << ">>���̣� " << name << "�����������" << std::endl;
				return;
			}
		}

		current = current->getNext();
	}

	//���������У�����н��̴��ڣ��������н�����������Դ
	if(readyQueue != NULL && runningProcess == NULL) {
		//�Զ��׽��̽�����Դ���룬����ɹ������У�ʧ�ܽ�����������
		while(readyQueue != NULL) {
			std::string name = readyQueue->getProcess()->getName();
			if(!readyQueue->applyForResource()) {
				//������������
				addBlockQueue(readyQueue);
				std::string name = readyQueue->getProcess()->getName();
				std::cout << ">>���̣� " << name << "������������" << std::endl;

				readyQueue = readyQueue->getNext();
			}
			else {
				readyQueue->getProcess()->setState(1);
				runningProcess = readyQueue;
				std::cout << ">>���̣� " << name << "��Դ����ɹ�����������״̬" << std::endl;
				return;
			}
		}
	}
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
			runningProcess = pcb;

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
				currentProcess->setFore(pcb);
				currentProcess->getProcess()->setState(0);       //���õ�ǰ���н���Ϊ����������
				readyQueue = pcb;                                //���½��̲������
				pcb->setNext(currentProcess);                    //���׽���ָ����һ�����̣�����ӵĽ��̣�
				pcb->getProcess()->setState(1);                  //���ý���Ϊ����״̬
				runningProcess = pcb;

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
			pcb->setFore(lastProcess);
			readyLast = pcb;                                     //ʹ�������ж�βָ��ָ������¼����β�Ľ���

			std::cout << ">>��ǰ���̣�" << pcb->getProcess()->getName() << " �ȵ�ǰ���н��� "
				<< readyQueue->getProcess()->getName() << " ��,�����������" << std::endl;
		}
	}

	return true;
}