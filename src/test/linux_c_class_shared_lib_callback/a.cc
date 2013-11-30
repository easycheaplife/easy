#include "a.h"
#include<iostream>
A::A()
{
	std::cout << " A construct call" << std::endl;
}

A::~A()
{
	std::cout << " A deconstruct call" << std::endl;
}

void A::Output()
{
	std::cout << " A! hello" << std::endl;
}
