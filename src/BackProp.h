//////////////////////////////////////////////////////////////////////
//****************************************************************************
//  File Name	: BackPropagation.h
//	Purpose		: This Header file contains th backpropagation classes 
//				  for Neural Network nodes and links from linked list.
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//****************************************************************************
#ifndef BACK_PROPAGATION
#define BACK_PROPAGATION

#include "Base.h"
#include "Common.h"
#include "Adaline.h"

#define DELTA 1
#define MOMENTUM 2

class BP_Node : public Feed_Forward_Node
{
protected:
	virtual double Transfer_Function(double value);
public:
	BP_Node(int v_size =1, int e_size=0); // Default of 1 value set member (NODE_VALUE)
	virtual char *Get_Name(void);
};

class BP_Link : public Base_Link
{
public:
	BP_Link(int size = 2); // default of 2 of 1 link value set members(WEIGHT, DELTA)
	virtual void Save( std::ofstream &outfile);
	virtual void Load(std::ifstream &infile);
	virtual char *Get_Name(void);
	virtual void Update_Weight(double new_val);
};

class BP_Output_Node : public BP_Node
{
public:
	BP_Output_Node( double lr, double mt, int v_size=3, int e_size=1);
	//default of 3 value set members (NODE_VALUE, LEARNING_RATE, MOMENTUM)
	//default of 1 error set member (NODE_ERROR)
protected:
	virtual double Compute_Error(int mode=0);
	virtual void Learn(int mode=0);
	virtual char *Get_Name(void);
};

class BP_Hidden_Node : public BP_Output_Node
{
public:
	BP_Hidden_Node(double lr, double mt, int v_size=3, int e_size=1);
	//default of 3 value set members (NODE_VALUE, LEARNING_RATE, MOMENTUM)
	//default of 1 error set member (NODE_ERROR)
	virtual char *Get_Name(void);
protected:
	virtual double Compute_Error(int mode=0);
};

double BP_Node::Transfer_Function(double value)
{
	return 1.0 / (1.0 + exp(-value));	// Sigmoid function
};

BP_Node::BP_Node(int v_size, int e_size) : Feed_Forward_Node(v_size, e_size)
{
};

char *BP_Node::Get_Name(void)
{
	static char name[] = "BP_NODE";
	return name;
};

BP_Link::BP_Link( int size) : Base_Link(size)
{
	value[WEIGHT] = Random(-1.0, 1.0);	// weight random value between -1.0 and 1.0
	value[DELTA] = 0.0;					// Initialize previous change to 0.0
};

void BP_Link::Save(std::ofstream &outfile)
{
	outfile << std::setw(4) << id << " " << std::setprecision(18)
		<< value[WEIGHT] << " " << std::setw(4)
		<< In_Node()->Get_ID() << " "
		<< std::setw(4) << Out_Node()->Get_ID() << std::endl;
};

void BP_Link::Load(std::ifstream &infile)
{
	infile >> id;
	infile >> value[WEIGHT];
	int dummy;
	infile >> dummy;
	infile >> dummy;
};

char *BP_Link::Get_Name(void)
{
	static char name[] = "BP_LINK";
	return name;
};

void BP_Link::Update_Weight(double new_val)
{
	double momentum = Out_Node()->Get_Value(MOMENTUM);
	value[WEIGHT] += new_val + (momentum * value[DELTA]);

	value[DELTA] = new_val;
};

BP_Output_Node::BP_Output_Node(double lr, double mt, int v_size, int e_size) : BP_Node(v_size, e_size)
{
	value[LEARNING_RATE] = lr;
	value[MOMENTUM] = mt;
};

double BP_Output_Node::Compute_Error(int mode)
{
	return value[NODE_VALUE] * (1.0-value[NODE_VALUE]) * (error[NODE_ERROR] - value[NODE_ERROR]);
};

void BP_Output_Node::Learn(int mode)
{
	double delta;
	error[NODE_ERROR] = Compute_Error();

	in_links.Reset_To_Head();
	Base_Link *link;
	int cnt = in_links.Count();

	for(int i = 0; i < cnt; i++)
	{
		link = in_links.Curr();
		delta = value[LEARNING_RATE] * error[NODE_ERROR] * link->In_Value(); // Delta Rule
		link->Update_Weight(delta);
		in_links.Next();
	}
};

char *BP_Output_Node::Get_Name(void)
{
	static char name[] = "BP_OUTPUT_NODE";
	return name;
};

BP_Hidden_Node::BP_Hidden_Node(double lr, double mt, int v_size, int e_size) : BP_Output_Node(lr, mt, v_size, e_size)
{
};

char *BP_Hidden_Node::Get_Name(void)
{
	static char name[] = " BP_HIDDEN_NODE";
	return name;
};

double BP_Hidden_Node::Compute_Error(int mode)
{
	double total =0;
	out_links.Reset_To_Head();
	int cnt=out_links.Count();

	for(int i = 0; i<cnt; i++)
	{
		total += out_links.Curr()->Weighted_Out_Error();
		out_links.Next();
	}
	return value[NODE_VALUE] * (1.0 - value[NODE_VALUE]) * total;
};

class BackProp_Network : public ADALINE_Network
{
protected:
	int num_layers;
	int first_hidden_node;
	int first_output_node;
	int *node_cnt;
	double momentum_term;

	virtual void Create_Network(void);

public:
	BackProp_Network(char filename[]);
	BackProp_Network(double lr, double mt, int layers, ...);
	BackProp_Network(double lr, double mt, int layers, int nodes[]);
	BackProp_Network(void);
	~BackProp_Network(void);

	virtual double Get_Error(int id=0);
	virtual void Set_Error(double new_val, int id=0);
	virtual void Set_Error(Pattern *output_pattern);
	virtual double Get_Value(int id=0);
	virtual void Save(std::ofstream &outfile);
	virtual void Load(std::ifstream &infile);
	virtual void Run(int mode =0);
	virtual void Learn(int mode=0);
	virtual char *Get_Name(void);
	virtual int Get_Num_Layers(void);
	virtual int Get_Layer_Count(int id);
};

void BackProp_Network::Create_Network(void)
{
	node = new Base_Node*[num_nodes];
	link = new Base_Link*[num_links];

	int curr = 0;

	for( int i=0; i < node_cnt[0]; i++)
	{
		node[curr++] = new Input_Node;
	}

	first_hidden_node = curr;

	for(int i = 1; i < num_layers-1 ; i++)
	{
		for(int j=0; j < node_cnt[i]; j++)
		{
			node[curr++] = new BP_Hidden_Node(learning_rate, momentum_term);
		}
	}

	first_output_node = curr;

	for(int i =0; i < node_cnt[num_layers-1] ; i++)
	{
		node[curr++] = new BP_Output_Node(learning_rate, momentum_term);
	}

	for(int i=0; i< num_links; i++)
	{
		link[i] = new BP_Link;
		if(link[i] == NULL)
			std::cout << "Error - Fail to allockte link" << i << std::endl;
	}

	curr = 0;
	int layer1 =0, layer2=first_hidden_node;
	
	for(int i = 0; i<num_layers-1; i++)
	{
		for(int j = 0; j < node_cnt[i+1]; j++)
		for(int k = 0; k < node_cnt[i]; k++)
		Connect(node[layer1+k], node[layer2+j], link[curr++]);
		layer1 = layer2;
		layer2 += node_cnt[i+1];
		
	}
};

BackProp_Network::BackProp_Network(char filename[]) : ADALINE_Network()
{
	std::ifstream infile(filename);
	node_cnt = NULL;
	Load(infile);
	infile.close();
};

BackProp_Network::BackProp_Network(double lr, double mt, int layers, ...) : ADALINE_Network()
{ 
	num_nodes = 0;
	num_links = 0;

	num_layers = layers;
	va_list vl;

	va_start(vl, layers);
	node_cnt = new int[layers];
	for(int i = 0; i< layers; i++)
	{
		node_cnt[i] = va_arg(vl, int);
		num_nodes += node_cnt[i];
		if(i > 0)
		{
			num_links += node_cnt[i-1] * node_cnt[i]; // link between layers
		}
	}

	va_end(vl);

	learning_rate = lr;
	momentum_term = mt;
	Create_Network();
};

BackProp_Network::BackProp_Network(double lr, double mt, int layers, int nodes[]) : ADALINE_Network()
{ 
	num_nodes=0;
	num_links=0;

	num_layers = layers;
	node_cnt = new int[layers];

	for(int i = 0; i < layers;  i++)
	{
		node_cnt[i] = nodes[i];
		num_nodes += node_cnt[i];

		if( i > 0)
			num_links += node_cnt[i-1] * node_cnt[i]; // link between layers
	}

	learning_rate = lr;
	momentum_term = mt;

	Create_Network();
};

BackProp_Network::BackProp_Network(void) : ADALINE_Network()
{ 
	node_cnt = NULL;
};

BackProp_Network::~BackProp_Network(void)
{ 
	if(node_cnt) 
		delete []node_cnt;
};

double BackProp_Network::Get_Error(int id)
{ 
	return node[id + first_output_node]->Get_Error(); // in output layer only with next index 0
};

void BackProp_Network::Set_Error(double new_val, int id)
{   
	node[id + first_output_node]->Set_Error(new_val); // Set error in output layer only starting with index 0
};

void BackProp_Network::Set_Error(Pattern *output_pattern)
{   
	for(int i=0; i < node_cnt[num_layers-1] ; i++)
		node[i + first_output_node]->Set_Error(output_pattern->Out(i));
};

double BackProp_Network::Get_Value(int id)
{   
	return node[id+first_output_node]->Get_Value();
};

void BackProp_Network::Save(std::ofstream &outfile)
{   
	outfile << id << std::endl;
	outfile << num_layers << std::endl;
	for(int i =0; i<num_layers; i++)
		outfile << node_cnt[i] << std::endl;

	Save_Nodes_Links(outfile);
};

void BackProp_Network::Load(std::ifstream &infile)
{ 
	infile >> id;
	infile >> num_layers;
	if(node_cnt)
		delete []node_cnt;
	node_cnt = new int[num_layers];

	for(int i = 0; i < num_layers; i++)
		infile >> node_cnt[i];
	
	Load_Nodes_Links(infile);
};

void BackProp_Network::Run(int mode)
{ 
	Load_Inputs();
	for(int i = first_hidden_node; i < num_nodes; i++)
		node[i]->Run();
};

void BackProp_Network::Learn(int mode)
{   
	int cnt = out_links.Count();
	if(cnt > 0)
	{
		out_links.Reset_To_Head();
		for(int i =0; i < cnt; i++)
		{
			node[i+first_output_node]->Set_Error(out_links.Curr()->Out_Error());
			out_links.Next();
		}
	}
	for(int i = num_nodes-1; i >= first_hidden_node; i--)
		node[i]->Learn();
};

char *BackProp_Network::Get_Name(void)
{ 
	static char name[] = "BACKPROPAGATION_NETWORK";
	return name;
};

int BackProp_Network::Get_Num_Layers(void)
{ 
	return num_layers;
};

int BackProp_Network::Get_Layer_Count(int id)
{ 
	return node_cnt[id];
};

#endif 
