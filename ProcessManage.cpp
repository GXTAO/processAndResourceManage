#include <iostream>
#include "ProcessManage.h"
#include "MyProcess.h"
#include "PCB.h"
#include "ResourceManage.h"

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
/* 初始化进程：创建初始进程*/
/************************************************************************/
void ProcessManage::init() {
	MyProcess* process = new MyProcess("initProcess", 1, 0, 0);
	PCB* pcb = new PCB(process);
	pcb->setState(true);                                      //设置状态为就绪
	pcb->getProcess()->setState(1);                           //设置进程状态为运行
	queue0 = pcb;
	initProcess = pcb;
	std::cout << ">>系统进程 " << queue0->getProcess()->getName() << " 已创建运行" << std::endl;
	std::cout << ">>系统已启动，请输入相应指令进行操作" << std::endl;
	std::cout << std::endl;
}


/************************************************************************/
/* 创建进程：
	name：新建进程名
	priority：新建进程优先级*/
/************************************************************************/
bool ProcessManage::createProcess(std::string name, int priority) {
	//首先检查是否重复创建相同名称的进程
	PCB* isRepeat = NULL;
	isRepeat = find(name);
	if(isRepeat != NULL) {
		std::cout << ">>当前进程名已存在，请重新命名进程名" << std::endl;
		return false;
	}

	int rA = -1;
	int rB = -1;

	MyProcess* process = new MyProcess(name, priority);
	process->getResourceNeed(rA, rB);
	PCB* pcb = new PCB(process);

	std::cout << ">>进程： " << name << " 创建成功，优先级为：" << priority 
		<< " 资源A、B需求量为:" << rA << "、" << rB  << std::endl;

	//父子关系建立
	//如果当前有运行进程，则此新建进程为此运行进程的子进程，如果没有，则是初始进程的子进程
	if(runningProcess == NULL) {
		initProcess->setSon(pcb);
		pcb->setFather(initProcess);
	}
	else {
		runningProcess->setSon(pcb);
		pcb->setFather(runningProcess);
	}

    //新建进程后将其添加入就绪队列
	addReadyQueue(pcb);

	return true;
}

/************************************************************************/
/* 删除进程：
	name：删除的进程名*/
/************************************************************************/
bool ProcessManage::deleteProcess(std::string name) {
	//查看是否存在这个进程
	PCB* destroyProcess = find(name);
	if(destroyProcess == NULL) {
		std::cout << ">>系统中无此进程，请检查输入" << std::endl;

		return true;
	}
	else {
		destroy(destroyProcess);
		schedule();
	}

	return true;
}

/************************************************************************/
/* 销毁进程，连同其子进程：
	pcb：销毁的进程*/
/************************************************************************/
bool ProcessManage::destroy(PCB* pcb) {
	//首先查看此进程是否有子进程，如果有子进程，先销毁子进程
	if(pcb->getSon() != NULL) {
		destroy(pcb->getSon());
	}

	//释放删除进程的资源
	ResourceManage::releaseResource(pcb);

	//销毁当前进程
	//去除相应的父子关系
	PCB* father = pcb->getFather();
	PCB* son = pcb->getSon();
	if(father != NULL && son == NULL) {
		father->setSon(NULL);
	}

	//去除队列信息
	std::string name = pcb->getProcess()->getName();
	//就绪队列寻找
	PCB* current = readyQueue;
	while(current != NULL) {
		if(current->getProcess()->getName() == name) {
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//如果当前进程位于队首，并且不止一个进程
			if(fore == NULL && next != NULL) {
				readyQueue = next;
				next->setFore(fore);
				runningProcess = NULL;

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功";
				std::cout << " 正在运行的进程被删除" << std::endl;
				return true;
			}
			//如果当前进程位于队首，并且只有一个进程
			else if(fore == NULL && next == NULL) {
				readyQueue = NULL;
				runningProcess = NULL;

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功";
				std::cout << " 正在运行的进程被删除" << std::endl;
				return true;
			}
			//如果当前进程位于队尾
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
			//进程位于中间
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
		}

		current = current->getNext();
	}

	//阻塞队列寻找
	current = blockQueue;
	while(current != NULL && pcb != NULL) {
		if(current->getProcess()->getName() == name) {
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//如果当前进程位于队首，并且不止一个进程
			if(fore == NULL && next != NULL) {
				blockQueue = next;
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
			//如果当前进程位于队首，并且只有一个进程
			else if(fore == NULL && next == NULL) {
				blockQueue = NULL;

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
			//如果当前进程位于队尾
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
			//进程位于中间
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				delete(pcb);
				std::cout << ">>进程： " << name << " 删除成功" << std::endl;
				return true;
			}
		}

		current = current->getNext();
	}

	return false;
}

/************************************************************************/
/* 查找进程：
	name：所查找的进程名*/
/************************************************************************/
PCB* ProcessManage::find(std::string name) {
	PCB* findProcess = NULL;
	//先冲就绪队列开始查找
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

	//再查找阻塞队列
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
/* 查看进程列表：*/
/************************************************************************/
bool ProcessManage::listProcess() {
	std::cout << ">>进程列表如下：" << std::endl;

	if(runningProcess == NULL) {
		std::cout << ">>当前无运行进程" << std::endl;
	}
	else {
		std::cout << ">>当前运行进程为：" << runningProcess->getProcess()->getName() << std::endl;
	}

	PCB* current = readyQueue;
	if(current == NULL) {
		std::cout << ">>就绪队列为空，没有就绪进程" << std::endl;
	}
	else {
		std::cout << ">>就绪队列有如下进程：";
		while(current != NULL) {
			std::cout << current->getProcess()->getName() << " ";
			current = current->getNext();
		}
		std::cout << std::endl;
	}

	current = blockQueue;
	if(current == NULL) {
		std::cout << ">>阻塞队列为空，没有阻塞进程" << std::endl;
	}
	else {
		std::cout << ">>阻塞队列有如下进程：";
		while(current != NULL) {
			std::cout << current->getProcess()->getName() << " ";
			current = current->getNext();
		}
		std::cout << std::endl;
	}

	return true;
}

/************************************************************************/
/* 时钟中断：*/
/************************************************************************/
bool ProcessManage::timeInt() {
	if(runningProcess == NULL) {
		std::cout << ">>当前无运行进程，不需时钟中断" << std::endl;
		return false;
	}

	std::cout << ">>进程：" << runningProcess->getProcess()->getName() << " 时钟中断" << std::endl;
	//如果就绪进程就只有一个，只设置不是运行状态就可
	if(readyQueue->getNext() == NULL) {
		runningProcess->getProcess()->setState(0);
	}
	//否则将运行进程停止，并插入就绪队列最后方
	else {
		//设置状态
		runningProcess->getProcess()->setState(0);
		//从队列中移除
		readyQueue = runningProcess->getNext();
		runningProcess->getNext()->setFore(NULL);
		//插入队尾
		PCB* current = runningProcess;
		readyLast->setNext(current);
		current->setFore(readyLast);
		current->setNext(NULL);
		readyLast = current;
	}

	runningProcess = NULL;
	schedule();

	return true;
}

/************************************************************************/
/* 进程调度：*/
/************************************************************************/
void ProcessManage::schedule() {
	//首先检查阻塞队列是否可以获得资源而激活
	PCB* current = blockQueue;
	while(current != NULL) {
		//如果资源充足，进程进入就绪队列
		if(current->applyForResource(false)) {
			//从阻塞队列中移除
			std::string name = current->getProcess()->getName();
			PCB* fore = current->getFore();
			PCB* next = current->getNext();
			//如果当前进程位于队首，并且不止一个进程
			if(fore == NULL && next != NULL) {
				blockQueue = next;
				next->setFore(fore);

				std::cout << ">>进程： " << name << "进入就绪队列" << std::endl;
				addReadyQueue(current);
				return;
			}
			//如果当前进程位于队首，并且只有一个进程
			else if(fore == NULL && next == NULL) {
				blockQueue = NULL;

				std::cout << ">>进程： " << name << "进入就绪队列" << std::endl;
				addReadyQueue(current);
				return;
			}
			//如果当前进程位于队尾
			else if(fore != NULL && next == NULL) {
				fore->setSon(NULL);

				std::cout << ">>进程： " << name << "进入就绪队列" << std::endl;
				addReadyQueue(current);
				return;
			}
			//进程位于中间
			else if(fore != NULL && next != NULL) {
				fore->setNext(next);
				next->setFore(fore);

				std::cout << ">>进程： " << name << "进入就绪队列" << std::endl;
				addReadyQueue(current);
				return;
			}
		}

		current = current->getNext();
	}

	//检查就绪队列，如果有进程存在，但无运行进程则申请资源
	if(readyQueue == NULL) {
		std::cout << ">>无就绪进程，无进程可调度运行" << std::endl;
	}
	if(readyQueue != NULL && runningProcess == NULL) {
		//对队首进程进行检测，如果成功则运行，失败进入阻塞队列
		std::string name = readyQueue->getProcess()->getName();
		//如果资源不足则进入阻塞队列
		if(!readyQueue->getState() && !readyQueue->applyForResource(false)) {
			//首先从就绪独立移除
			PCB* current = readyQueue;
			current->setNext(NULL);
			readyQueue = current->getNext();
			if(readyQueue == NULL) {
				runningProcess = NULL;
				readyLast = NULL;
			}
			else {
				readyQueue->setFore(NULL);
			}
			//加入阻塞队列
			addBlockQueue(current);
			std::string name = readyQueue->getProcess()->getName();
			std::cout << ">>进程： " << name << "进入阻塞队列" << std::endl;

			//无运行进程，再调度进程调度，查看就绪队列下一个进程
			schedule();
		}
		else {
			readyQueue->getProcess()->setState(1);
			runningProcess = readyQueue;
			//如果是第一次申请资源成功，则设置成功占用资源
			if(!readyQueue->getState()) {
				readyQueue->setState(true);
			}
			std::cout << ">>进程： " << name << "资源申请成功，进入运行状态" << std::endl;
			return;
		}
	}
}

/************************************************************************/
/* 添加进程进入阻塞队列：
	pcb：添加的进程*/
/************************************************************************/
bool ProcessManage::addBlockQueue(PCB* pcb) {
	if(blockQueue == NULL) {
		blockQueue = pcb;
		blockLast = pcb;
		pcb->setNext(NULL);
	}
	else {
		blockLast->setNext(pcb);
		pcb->setFore(blockLast);
		blockLast = pcb;
		pcb->setNext(NULL);
	}

	return true;
}

/************************************************************************/
/* 添加进程进入就绪队列：
	pcb：添加的进程*/
/************************************************************************/
bool ProcessManage::addReadyQueue(PCB* pcb) {
	if(readyQueue == NULL) {                          //如果当前就绪队列中没有进程
		if(pcb->applyForResource(true)) {                 //如果进程的资源申请成功则进入就绪队列运行
			readyQueue = pcb;                         //进入就绪队列
			readyLast = pcb;                          //同时它也是队尾进程
			pcb->getProcess()->setState(1);           //设置进程为运行状态
			pcb->setState(true);                      //表示已占用资源
			runningProcess = pcb;

			std::cout << ">>当前无运行进程，进程分配资源成功，直接运行进程" << std::endl;
		}
		else {                                        //如果资源不足则进程阻塞进入阻塞队列
			addBlockQueue(pcb);
			std::cout << ">>当前无运行进程，进程分配资源失败，进程阻塞队列" << std::endl;
		}
	}
	//如果就绪队列不为空，对新加入的进程做一次优先级检查，如果进程优先级高则取代当前运行进程
	else {
		if(readyQueue->getProcess()->getPriority() > pcb->getProcess()->getPriority()) {

			std::cout << ">>当前进程：" << pcb->getProcess()->getName() << " 比当前运行进程 "
				<< readyQueue->getProcess()->getName() << " 高" << std::endl;

			if(pcb->applyForResource(true)) {                        //如果进程的资源申请成功则插队运行
				PCB* currentProcess = readyQueue;                //获取当前运行进程
				currentProcess->setFore(pcb);
				currentProcess->getProcess()->setState(0);       //设置当前运行进程为就绪不运行
				readyQueue = pcb;                                //将新进程插入队首
				pcb->setNext(currentProcess);                    //队首进程指向下一个进程（被插队的进程）
				pcb->getProcess()->setState(1);                  //设置进程为运行状态
				pcb->setState(true);                             //表示已占用资源
				runningProcess = pcb;

				std::cout << ">>当前进程分配资源成功，直接运行进程" << std::endl;
			}
			else {                                               //如果资源不足则进程阻塞进入阻塞队列
				addBlockQueue(pcb);

				std::cout << ">>当前进程分配资源失败，进程阻塞队列" << std::endl;
			}
		}
		//如果进程优先级没有当前运行进程高，则插入就绪进程队尾
		else {
			PCB* lastProcess = readyLast;                        //获取就绪队列最尾部进程
			lastProcess->setNext(pcb);                           //插入队尾
			pcb->setFore(lastProcess);
			readyLast = pcb;                                     //使就绪队列队尾指针指向这个新加入队尾的进程

			std::cout << ">>当前进程：" << pcb->getProcess()->getName() << " 比当前运行进程 "
				<< readyQueue->getProcess()->getName() << " 低,加入就绪队列" << std::endl;
		}
	}

	return true;
}

void ProcessManage::getRunningResource(int& amountA, int& amountB) {
	if(runningProcess == NULL) {
		amountA = 0;
		amountB = 0;
		std::cout << ">>当前无运行进程，无资源释放" << std::endl;
		return;
	}

	runningProcess->getProcess()->getResourceNeed(amountA, amountB);

	//释放资源以后，相当于这个进程已经运行完成，从队列中删除
	PCB* current = runningProcess;
	std::cout << ">>进程：" << runningProcess->getProcess()->getName() << " 运行完成，资源释放" << std::endl;
	//如果就绪进程就只有一个，就绪队列相关参数设置为空
	if(readyQueue->getNext() == NULL) {
		runningProcess = NULL;
		readyQueue = NULL;
		readyLast = NULL;
	}
	//将运行进程移除即可
	else {
		//从队列中移除
		readyQueue = runningProcess->getNext();
		runningProcess->getNext()->setFore(NULL);
		runningProcess = NULL;
	}

	//删除进程
	delete(current);
}