#include "InputCallback.h"
#include <iostream>

class Test : public InputCallback
{
	public:
	virtual void RegisterInput() override;
};

void Test::RegisterInput()
{
	std::cout << "test";
}
