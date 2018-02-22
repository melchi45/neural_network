//****************************************************************************
//  File Name	: ADALINE_Node.h
//	Purpose		: This headrt file contains the ADALINE_Node classes
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//  Reference   : Adaptive Linear Neuron or later Adaptive Linear Element
//				  https://en.wikipedia.org/wiki/ADALINE
//****************************************************************************
#pragma once

#ifndef ADALINE
#define ADALINE

#include "Base.h"
#include "Common.h"
#include "Pattern.h"

class ADALINE_Node : public Feed_Forward_Node
{
protected:
	virtual double Transfer_Function(double value);
public:
	ADALINE_Node();
	ADALINE_Node(double lr);
	virtual void Learn(int mode);
	virtual char *Get_Name(void);
};
//******************************************************************
// Constructor
ADALINE_Node::ADALINE_Node() 
	: Feed_Forward_Node(2/*v_size*/ ,1/*e_size*/)
{
};
//******************************************************************
// Constructor with Learning Rate specified
ADALINE_Node::ADALINE_Node(double lr) 
	: Feed_Forward_Node(2/*v_size*/ ,1/*e_size*/)
{
	value[LEARNING_RATE] = lr;
};

//******************************************************************
// Threshold Transfer Function 
// if x < 0 f(x) = -1.0
// else		f(x) = 1.0
double ADALINE_Node::Transfer_Function(double value)
{
	if(value < 0)
		return -1.0;
	else
		return 1.0;
};

void ADALINE_Node::Learn(int mode)
{
	error[NODE_ERROR] = value[NODE_VALUE] * -2.0; // Compute error
	Base_Link *link;			// create link
	in_links.Reset_To_Head();	// link initialize
	int cnt = in_links.Count(); // link count number
	double delta;				

	for(int i = 0; i < cnt ; i++)  // Traverse Input links
	{
		link = in_links.Curr();
		delta = value[LEARNING_RATE] * link->In_Value() * error[NODE_ERROR]; // Delta Rule
		link->Update_Weight(delta); // new weight
		in_links.Next();
	}
};

char *ADALINE_Node::Get_Name(void)
{
	static char name[] = "ADALINE_NODE";
	return name;
};

class ADALINE_Link : public Base_Link // Link for ADALINE Node
{
public:
	ADALINE_Link(void);
	virtual void Save( std::ofstream &outfile);
	virtual void Load( std::ifstream &infile);
	virtual char *Get_Name(void);
};
//******************************************************************
// Automatically initialized weight value to random number
// Small random values are usually used
ADALINE_Link::ADALINE_Link(void) : Base_Link()
{
	value[WEIGHT] = Random(-1.0, 1.0);
};

void ADALINE_Link::Save( std::ofstream &outfile)
{
	outfile << std::setw(4) << id << " " << std::setprecision(18)
		<< value[WEIGHT] << " " << std::setw(4) << In_Node()->Get_ID() << " "
		<< std::setw(4) << Out_Node()->Get_ID() << std::endl;
};

void ADALINE_Link::Load( std::ifstream &infile)
{
	infile >> id;
	infile >> value[WEIGHT];

	int dummy;

	infile >> dummy;	// Skip In node IDs
	infile >> dummy;	// Skip Out node IDs
};

char *ADALINE_Link::Get_Name(void)
{
	static char name[] = "ADALINE_LINK";
	return name;
};

class ADALINE_Network : public Base_Network // ADALINE Network Node
{
protected:
	virtual void Create_Network(void);
	virtual void Load_Inputs(void);
	double learning_rate;
public:
	ADALINE_Network(char filename[] );
	ADALINE_Network(int size);
	ADALINE_Network(int size, double lr);
	ADALINE_Network(void);
	virtual double Get_Value(int id = 0);
	virtual void Set_Value(double new_val, int id=0);
	virtual void Set_Value(Pattern *input_pattern);
	virtual void Save(std::ofstream &outfile);
	virtual void Load(std::ifstream &infile);
	virtual void Run(int mode=0);
	virtual void Learn(int mode=0);
	virtual char *Get_Name(void);
};

void ADALINE_Network::Create_Network(void)
{ 
	node = new Base_Node*[num_nodes];
	link = new Base_Link*[num_links];

	for(int i=0; i < num_nodes-2; i++) // Create input Nodes
		node[i] = new Input_Node;

	node[num_nodes-2] = new Bias_Node;	// Create Bias Node

	node[num_nodes-1] = new ADALINE_Node(learning_rate);	// Create ADALINE

	for(int i = 0; i < num_links; i++)		// Create links
		link[i] = new ADALINE_Link;

	for(int i = 0; i < num_links; i++)			// Connect inputs to ADALINE
		Connect(node[i], node[num_nodes-1], link[i]);
};

void ADALINE_Network::Load_Inputs(void)
{   
	int cnt = in_links.Count();	// If network node has any input links.

	if(cnt >0)			// load the connected node's input values into the input layer
	{				
		in_links.Reset_To_Head();		// For all in links connected to Network node

		for(int i =0; i <cnt; i++)
		{
			Set_Value(in_links.Curr()->In_Value(), i);
			in_links.Next();
		}
	}
};

ADALINE_Network::ADALINE_Network(char filename[] ) : Base_Network()	// Constructor
{   
	std::ifstream infile(filename);
	Load(infile);
	infile.close();
};

ADALINE_Network::ADALINE_Network(int size)
{   
	num_nodes = size +2;	// num nodes equals input layer
							// size +bias + ADALINE Node
	num_links = size +1;	// num links equal on for each input
							// layer node and on for bias node
	learning_rate = 0;
	Create_Network();
};

ADALINE_Network::ADALINE_Network(int size, double lr) : Base_Network()
{   
	num_nodes = size + 2;
	num_links = size + 1;
	learning_rate = lr;
	Create_Network();
};

ADALINE_Network::ADALINE_Network(void) : Base_Network()
{   
};

double ADALINE_Network::Get_Value(int id)
{ 
	return node[num_nodes -1]->Get_Value();	// return only ADALINE node value 
											// id defaults to 0
};

void ADALINE_Network::Set_Value(double new_val, int id)
{   
	node[id]->Set_Value(new_val);
};

void ADALINE_Network::Set_Value(Pattern *input_pattern)
{ 
	for(int i =0; i < input_pattern->In_Size() ;  i++)	// Load pattern's input
		node[i]->Set_Value(input_pattern->In(i));		// valur into input layer
};

void ADALINE_Network::Save(std::ofstream &outfile)
{ 
	outfile << id << std::endl;
	Save_Nodes_Links(outfile);
};

void ADALINE_Network::Load(std::ifstream &infile)
{ 
	infile >> id;
	Load_Nodes_Links(infile);
};

void ADALINE_Network::Run(int mode)
{   
	Load_Inputs();
	node[num_nodes-1]->Run();	// Run ADALINE node only
};

void ADALINE_Network::Learn(int mode)
{   
	node[num_nodes-1]->Learn();	// only ADALINE node needs to excute its Learn
};

char *ADALINE_Network::Get_Name(void)
{ 
	static char name[]="ADALINE_Network";
	return name;
};
#endif