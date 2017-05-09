#pragma once
/************************************************************************/
/* ���̿��ƿ飺*/
/************************************************************************/

class MyProcess;
class PCB
{
public:
	PCB(MyProcess* myProcess);
	~PCB(void);

	//�������������Դ
	bool applyForResource();

	MyProcess* getProcess();
	PCB* getFather();
	bool setFather(PCB* pcb);
	PCB* getSon();
	bool setSon(PCB* pcb);
	PCB* getFore();
	bool setFore(PCB* const pcb);
	PCB* getNext();
	bool setNext(PCB* const pcb);
	bool getState();
	bool setState(bool state);

private:
	MyProcess* theProcess;//��ǰ����
	PCB* father;//������
	PCB* son;//�ӽ���
	PCB* fore;//�����ڶ����е���һ������
	PCB* next;//�����ڶ����е���һ������
	bool isReady;//�����Ƿ�����Դ���Ծ���
};

