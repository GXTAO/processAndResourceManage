#pragma once
/************************************************************************/
/* �����ࣺ*/
/************************************************************************/
#include <string>

class MyProcess
{
public:
	MyProcess(const std::string& theName, const int& thePriority, const int& resourceA,
		const int& resourceB);
	MyProcess(const std::string& theName, const int& thePriority);
	~MyProcess(void);

	std::string getName();
	void getResourceNeed(int& resource1, int& resource2);
	int getPriority();
	int getState();
	bool setState(const int& state);

private:
	std::string name;//������
	int resourceA, resourceB;//������Դ����
	int priority;//�������ȼ�
	int state;//����״̬
};

