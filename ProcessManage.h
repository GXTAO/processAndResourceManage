#pragma once
/************************************************************************/
/* ���̵��ȹ���*/
/************************************************************************/

#include <string>

class PCB;
class ProcessManage
{
public:
	ProcessManage(void);
	~ProcessManage(void);

	//Create init process
	static void init();
	//Create corresponding process
	static bool createProcess(std::string name, int priority);
	//Delete corresponding process
	static bool deleteProcess(std::string name);
	//Look process information
	static bool listProcess();
	//Clock interrupt	
	static bool timeInt();
	//Process schedule
	static void schedule();
	static void schedule(PCB* pcb);
	//��ӽ��̽�����������
	static bool addBlockQueue(PCB* pcb);
	//��ӽ��̽����������
	static bool addReadyQueue(PCB* pcb);

private:
	//���ȼ�����
	static PCB* queue0;
	static PCB* queue1;
	static PCB* queue2;
	//��������
	static PCB* blockQueue;
	static PCB* blockFirst;//����������
	static PCB* blockLast;//��������β
	//��������
	static PCB* readyQueue;
	static PCB* readyFirst;//����������
	static PCB* readyLast;//��������β
};

