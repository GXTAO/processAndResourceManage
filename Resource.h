#pragma once
class Resource
{
public:
	Resource(const int& amount);
	virtual ~Resource(void);

	int getAmount();

private:
	//��Դ������
	int amount;
};

