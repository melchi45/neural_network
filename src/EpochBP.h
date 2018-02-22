////////////////////////////////////////////////////////////////////////****************************************************************************
//  File Name	: EpochBP.h
//	Purpose		: Input/Output Pattern Class
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//****************************************************************************
#pragma once

#ifndef EPOCH_H_
#define EPOCH_H_

#include "Base.h"
#include "Common.h"
#include "BackProp.h"

#define EPOCH 2

class Epoch_BP_Link : public BP_Link
{
public:
	Epoch_BP_Link( int size=3); // Default of three value set members
								// (WEIGHT, DELTA, EPOCH)
	virtual char *Get_Name(void);
	virtual void Update_Weight(double new_val);
	virtual void Epoch(int mode);
};

Epoch_BP_Link::Epoch_BP_Link(int size) : BP_Link(size)
{
	value[WEIGHT] = Random(-1.0, 1.0);
};

void Epoch_BP_Link::Update_Weight(double new_val)
{
	value[EPOCH] += new_val;
};

void Epoch_BP_Link::Epoch(int mode)
{
	double momentum = Out_Node()->Get_Value(MOMENTUM);
	double delta = value[EPOCH]/mode;
	value[WEIGHT] += delta + (momentum * value[DELTA]);	
	value[DELTA] = delta;
	value[EPOCH] =0.0;
};

char *Epoch_BP_Link::Get_Name(void)
{
	static char name[] = "EPOCH_BP_LINK";
	return name;
};


#endif // !defined(AFX_EPOCHBP_H__255571C2_81F7_4409_AF7B_C362BE3E1AAA__INCLUDED_)
