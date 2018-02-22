//****************************************************************************
//  File Name	: Common.h
//	Purpose		: This Header file contains some commonly used node types
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//****************************************************************************
#pragma once

// define header file
#include "Base.h"

#ifndef COMMON
#define COMMON

// -------------------------------------------------------------------
// The Input Node class is a generic Input Node. 
// It can be used with most networks
class Input_Node : public Base_Node
{
public:
	// Default of one value set member (NODE_VALUE) and 
	// one error set member (NODE_ERROR)
	Input_Node(int size = 1) : Base_Node(size, size)
	{
		for(int i = 0; i < size ; i++)
		{
			error[i] = 0.0;
			value[i] = 0.0;
		};
	};

	virtual char *Get_Name(void)
	{
		static char name[]="Input Node";
		return name;
	};
};

// -------------------------------------------------------------------
// The Bias Node class id a node that alaways proceduces the same output.
// The Bias Node's default output.
class Bias_Node : public Input_Node
{
public:
	Bias_Node( double bias = 1.0) : Input_Node(1)
	{	
		value[0] = bias;
	};

	virtual void Set_Value(double value, int id=0)
	{
	};

	virtual double Get_Value(int id = 0)
	{
		return value[0];
	};
	
	virtual char *Get_Name(void)
	{
		static char name[]="BIAS_NODE";
		return name;
	};

};

#endif