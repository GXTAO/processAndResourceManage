#pragma once
/************************************************************************/
/* ϵͳ��Դ����*/
/************************************************************************/

#include <string>

class Resource;
class PCB;
class ResourceManage
{
public:
	ResourceManage(void);
	~ResourceManage(void);

	//Apply for resource
	static bool requestResource();
	static bool apply(const int& countA, const int& countB, bool isGet);
	//Release resource
	static bool releaseResource();
	static bool releaseResource(PCB* pcb);
	//Look resource information
	static bool listResource();

private:
	static Resource* resourceA;                            //��ԴA
	static Resource* resourceB;                            //��ԴB
	static int remainA;                                    //��ԴA��ʣ������
	static int remainB;                                    //��ԴB��ʣ������
};

