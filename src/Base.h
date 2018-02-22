//****************************************************************************
//  File Name	: Base.h
//	Purpose		: This Header file contains th base classes for Neural 
//				  Network nodes and links from linked list.
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//****************************************************************************
#pragma once

// Define Header File
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>     // std::cout
#include <iomanip>		// std::setprecision
#include <fstream>      // std::ifstream std::ofstream

// Define Header file Name
#ifndef BASE
#define BASE

// Initialize value
#define NODE_VALUE 0
#define LEARNING_RATE 1
#define NODE_ERROR 0
#define WEIGHT 0

class Base_Node;	//Foward declaration to links can use the base node type
class Base_Link  
{
private:
	static int ticket;

protected:
	int id;					// ID number form link
	double *value;			// Value(s) for link
	Base_Node *in_node;		// Node instance link is comming from
	Base_Node *out_node;	// Node instance link is going to
	int value_size;			// Number of Node instance link

public:
	Base_Link( int size=1);	// Constructor
	~Base_Link(void);		// Destructor

	virtual void Save( std::ofstream &outfile );		// Save to Outfile
	virtual void Load( std::ifstream &infile );		// Load to Infile
	inline virtual double Get_Value(int id=WEIGHT);	
	inline virtual void Set_Value(double new_val, int id=WEIGHT);
	inline virtual void Set_In_Node(Base_Node *node, int id);
	inline virtual void Set_Out_Node(Base_Node *node, int id);
	inline virtual Base_Node *In_Node(void);
	inline virtual Base_Node *Out_Node(void);
	inline virtual char *Get_Name(void);
	inline virtual void Update_Weight(double new_val);
	inline int Get_ID(void);
	inline virtual double In_Value(int mode=NODE_VALUE);
	inline virtual double Out_Value(int mode=NODE_VALUE);
	inline virtual double In_Error(int mode=NODE_ERROR);
	inline virtual double Out_Error(int mode=NODE_ERROR);
	inline virtual double Weighted_In_Value(int mode=NODE_VALUE);
	inline virtual double Weighted_Out_Value(int mode=NODE_VALUE);
	inline virtual double Weighted_In_Error(int mode=NODE_VALUE);
	inline virtual double Weighted_Out_Error(int mode=NODE_VALUE);
	inline virtual int Get_Set_Size(void);
	inline virtual void Epoch(int mode=0);
};

class LList  
{
private:
	struct NODE
	{
		NODE *next, *prev;
		Base_Link *element;
	};

	NODE *head, *tail, *curr;
	int count;

public:
	LList(void);
	~LList(void);

	int Add_To_Tail(Base_Link *element);
	int Add_Node(Base_Link *element);
	int Del_Node(void);
	int Del(Base_Link *element);
	int Find(Base_Link *element);
	inline void Clear(void);
	inline int Count(void);
	inline void Reset_To_Head(void);
	inline void Reset_To_Tail(void);
	inline Base_Link *Curr(void);
	inline void Next(void);
	inline void Prev(void);
};

int Base_Link::Get_Set_Size(void)
{
	return value_size;
};

Base_Link::Base_Link(int size)		// Constructor
{
	id=++ticket;
	value_size = size;
	if(value_size <= 0) 
		value = NULL;
	else
		value = new double[value_size];

	for(int i=0 ; i < value_size; i++)	// initilize value set to zero
	{
		value[i] = 0.0;
	}
	in_node = out_node = NULL;
};

Base_Link::~Base_Link(void) // Destructor for Base Links
{
	if(value_size > 0)
		delete[] value;
};

void Base_Link::Save( std::ofstream &outfile)
{
	outfile << id << std::endl;
	outfile << value_size;		// Store value set

	if(value)
		delete []value;
	value = new double[value_size];
	for(int i=0; i<value_size; i++)
		outfile << " " << std::setprecision(18) << value[i];

	outfile << std::endl;
};

void Base_Link::Load(std::ifstream &infile)
{
	infile >> id;
	infile >> value_size;
	
	if(value)
		delete []value;
	value=new double[value_size];	// Load value set

	for(int i=0; i < value_size ; i++)
		infile >> value[i];
};

double Base_Link::Get_Value(int id)
{
	return value[id];
};

void Base_Link::Set_Value(double new_val, int id)
{
	value[id] = new_val;
};

void Base_Link::Set_In_Node(Base_Node *node, int id)
{
	in_node = node;
};

void Base_Link::Set_Out_Node(Base_Node *node, int id)
{
	out_node = node;
};

Base_Node *Base_Link::In_Node(void)
{
	return in_node;
};

Base_Node *Base_Link::Out_Node(void)
{
	return out_node;
};

char *Base_Link::Get_Name(void)
{
	static char name[]="BASE_LINK";
	return name;
};

void Base_Link::Update_Weight(double new_val)
{
	value[WEIGHT] += new_val;
};

int Base_Link::Get_ID(void)
{
	return id;
};

void Base_Link::Epoch(int code)
{
};

int Base_Link::ticket=-1;		// This static variable is shared by all
								// links derived from the base link class. Its
								// purpose is to give each link created from
								// the base_link class a unique identification
								// number

class Base_Node					//Base Neural-Network Node
{ 
private:
	static int ticket;
protected:
	int id;					// identification Number
	double *value;			// Value(s) stored by this node
	int value_size;			// Number of Values stored by this node
	double *error;			// Error value(s) stored by this node
	int error_size;			// Number of Error values stored by this node

	LList in_links;			// List for input links
	LList out_links;		// List from output links

public:
	Base_Node(int v_size=1, int e_size=1);	// Constructor
	~Base_Node(void);						// Destructor

	LList *In_Links(void);
	LList *Out_Links(void);

	virtual void Run(int mode=0);
	virtual void Learn(int mode=0);
	virtual void Epoch(int code=0);
	virtual void Load( std::ifstream &infile);
	virtual void Save( std::ofstream &outfile);

	inline virtual double Get_Value(int id=NODE_VALUE);
	inline virtual void Set_Value(double new_val, int id=NODE_VALUE);
	inline virtual double Get_Error(int id=NODE_ERROR);
	inline virtual void Set_Error(double new_val, int id=NODE_ERROR);
	inline int Get_ID(void);
	inline virtual char *Get_Name(void);
	void Create_Link_To(Base_Node &to_node, Base_Link *link);
	virtual void Print(std::ofstream &out);
	
	// Connection and Disconnect function
	friend void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link *link);
	friend void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link &link);
	friend void Connect(Base_Node *from_node, Base_Node *to_node, Base_Link *link);
	friend int DisConnect(Base_Node *from_node, Base_Node *to_node);

	friend double Random(double lower_bound, double upper_bound);
};

Base_Node::Base_Node(int v_size, int e_size)	// Constructor
{
	id=++ticket;
	if(v_size <= 0)			// Create value storage
	{
		value_size=0;
		value = NULL;
	}
	else
	{
		value_size = v_size;
		value = new double[v_size];
	}

	for (int i=0; i < v_size ; i++)
	{
		value[i] = 0.0;
	}

	if(e_size <= 0)		// Create error storage
	{
		error_size=0; 
		error = NULL;
	}
	else
	{
		error_size = e_size;
		error=new double[e_size];
	}

	for(int i=0; i< e_size; i++)		//set all errors to zero
		error[i] = 0.0;
};

Base_Node::~Base_Node(void)			// Destuctor
{
	if(value_size > 0) delete[] value;
	if(error_size > 0) delete[] error;
};

LList *Base_Node::In_Links(void)
{  
	return &in_links;
};

LList *Base_Node::Out_Links(void)
{    
	return &out_links;
};

void Base_Node::Run(int mode)
{    
};

void Base_Node::Learn(int mode)
{   
};

void Base_Node::Epoch(int code)
{    
};

void Base_Node::Load( std::ifstream &infile)
{
    infile >> id;
	infile >> value_size;
	if(value)
		delete []value;
	value = new double[value_size];		// Load value set

	for(int i = 0; i < value_size ; i++)
		infile >> value[i];
	infile >> error_size;

	if(error) delete []error;
	error = new double[error_size];		// Load error set

	for(int i = 0;i < error_size; i++)
		infile >> error[i];
};

void Base_Node::Save( std::ofstream &outfile)
{    
	outfile << std::setw(4) << id << std::endl;
	outfile << value_size;			// store value set

	for( int i=0; i < value_size; i++)
		outfile <<  " " << std::setprecision(18) << value[i];
	
	outfile << std::endl;
	outfile << error_size;			// store error set

	for(int i =0 ; i < error_size ; i++)
		outfile << " " << std::setprecision(18) << error[i];
	outfile << std::endl;
};

double Base_Node::Get_Value(int id)
{
    return value[id];
};

void Base_Node::Set_Value(double new_val, int id)
{
    value[id] = new_val;
};

double Base_Node::Get_Error(int id)
{
    return error[id];
};

void Base_Node::Set_Error(double new_val, int id)
{
    error[id] = new_val;
};

int Base_Node::Get_ID(void)
{
    return id;
};

char *Base_Node::Get_Name(void)
{
    static char name[] = "BASE_NODE";
	return name;
};

void Base_Node::Create_Link_To(Base_Node &to_node, Base_Link *link) // Create Link from Link to Node
{
    out_links.Add_To_Tail(link);
	to_node.In_Links()->Add_To_Tail(link);
	link->Set_In_Node(this, id);
	link->Set_Out_Node(&to_node, to_node.Get_ID());
};

// Make Connection From Node1 to Node2 with Pointer Link
void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link *link)
{   
	from_node.Create_Link_To(to_node, link);
};
// Make Connection From Node1 to Node2 with Link
void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link &link)
{
    from_node.Create_Link_To(to_node, &link);
};
// Make Connection From Pointer Node1 to Pointer Node2 with Pointer Link
void Connect(Base_Node *from_node, Base_Node *to_node, Base_Link *link)
{    
	from_node->Create_Link_To(*to_node, link);
};
// Disconnection link from Node1 to Node2
int DisConnect(Base_Node *from_node, Base_Node *to_node)
{   
	LList *out_links = from_node->Out_Links();
	int flag = 0;
	out_links->Reset_To_Head();

	for( int i = 0; i < out_links->Count(); i++)	// for each output link
	{
		if(out_links->Curr()->Out_Node() == to_node)
		{
			flag =1;
			break;
		}
		out_links->Next();
	}

	if(flag == 1)	// link exists, delete it from both nodes
	{
		out_links->Curr()->Out_Node()->In_Links()->Del(out_links->Curr());
		out_links->Del_Node();
		return 1;
	}
	else
		return 0;		// link not found
};

double Random(double lower_bound, double upper_bound)	// Generate Random Number
{
	return ( (double)( ( rand()%RAND_MAX) )/(double)RAND_MAX) * 
		(upper_bound - lower_bound) + lower_bound;
};
// Save Outfile
void Base_Node::Print( std::ofstream &out)
{ 
	out << "Node ID : " << id << "     Node Name: " << Get_Name() << std::endl;
	out << "Value Set : ";
	for( int i=0; i< value_size; i++)
		out << value[i] << " ";

	out << std::endl;
	out << "Error Set: ";

	for(int i = 0; i < error_size ; i++)
		out << error[i] << " ";
	out << std::endl;

	in_links.Reset_To_Head();

	for(int i = 0 ; i < in_links.Count(); i++)
	{
		out << "In Link ID : " << in_links.Curr()->Get_ID() << " "
			<< "Link Name : " << in_links.Curr()->Get_Name() << " "
			<< "Source Node : " << in_links.Curr()->In_Node()->Get_ID() << " "
			<< "Value Set : ";
		
		for(int j=0; j < in_links.Curr()->Get_Set_Size(); j++)
			out << in_links.Curr()->Get_Value(j) << " ";
		out << std::endl;
		in_links.Next();
	}

	out_links.Reset_To_Head();
	for( int i = 0; i < out_links.Count(); i++)
	{
		out << "Out Link ID : " << out_links.Curr()->Get_ID() << " "
			<< "Link Name : " << out_links.Curr()->Get_Name() << " "
			<< "Dest Node : " << out_links.Curr()->Out_Node()->Get_ID() << " "
			<< "Value Set : ";

		for(int j=0; j < out_links.Curr()->Get_Set_Size(); j++)
			out << out_links.Curr()->Get_Value(j) << " ";
		out << std::endl;
		out_links.Next();
	}
	out << std::endl;
};
// This static variable id shared by all
// links derived from the base link class. Its
// purpose is to give each link created from
// the base_link class a unique identification
// number
int Base_Node::ticket=-1;

double Base_Link::In_Value(int mode)	// These Base_Link members
{		
	return in_node->Get_Value(mode);	// must be defined after the
};

double Base_Link::Out_Value(int mode)	// Base_Node class because
{
	return out_node->Get_Value(mode);	// they reference specific
};

double Base_Link::In_Error(int mode)	// Base_Node Members.
{
	return in_node->Get_Error(mode);
};

double Base_Link::Out_Error(int mode)
{
	return out_node->Get_Error(mode);
};

double Base_Link::Weighted_In_Value(int mode)
{
	return in_node->Get_Value(mode)*value[WEIGHT];
};

double Base_Link::Weighted_Out_Value(int mode)
{
	return out_node->Get_Value(mode)*value[WEIGHT];
};

double Base_Link::Weighted_In_Error(int mode)
{
	return in_node->Get_Error(mode)*value[WEIGHT];
};

double Base_Link::Weighted_Out_Error(int mode)
{
	return out_node->Get_Error(mode)*value[WEIGHT];
};

// This derived class provides
// a generic feed-forward
// neural-network node which
// can be used by the ADALINEs
// and Backprop networks
class Feed_Forward_Node : public Base_Node  
{
protected:
	virtual double Transfer_Function(double value); // Transfer_Function(Activatve Funztion)을 위한 가상함수
public:
	Feed_Forward_Node(int v_size = 1, int e_size = 1);	// Constructor
	virtual void Run(int mode =0);
	virtual char *Get_Name(void);

};

double Feed_Forward_Node::Transfer_Function(double value)
{
	return value;
};

Feed_Forward_Node::Feed_Forward_Node(int v_size, int e_size) : Base_Node(v_size, e_size)
{

};

void Feed_Forward_Node::Run(int mode)
{
	in_links.Reset_To_Head();	// Reset of links
	double total = 0.0;			// Initialize of Input weight
	int cnt=in_links.Count();	// Count value of Links

	for(int i=0 ; i < cnt; i++)	// For each node's input link
	{
		total += in_links.Curr()->Weighted_In_Value(); // Get of Input weight value and add total weight
		in_links.Next();	// Next Link
	}
	value[mode] = Transfer_Function(total);
};

char *Feed_Forward_Node::Get_Name(void)
{
	static char name[]="Feed_Forward_Node";
	return name;
};

class Base_Network : public Base_Node	// Base Network Node
{
protected:
	int num_nodes;			// Number of nodes in Network
	int num_links;			// Number of links in network
	Base_Node **node;		// Array of base nodes
	Base_Link **link;		// Array of base links

	virtual void Create_Network(void);
	virtual void Load_Inputs(void);
	virtual void Save_Nodes_Links(std::ofstream &outfile);
	virtual void Load_Nodes_Links(std::ifstream &infile);
public:
	Base_Network(void);		// Constructor
	~Base_Network(void);	// Destructor
	virtual void Epoch(int code=0);
	virtual void Print(std::ofstream &outfile);
	virtual char *Get_Name(void);

};

void Base_Network::Create_Network(void)
{  
	
};

void Base_Network::Load_Inputs(void)
{   
};

void Base_Network::Save_Nodes_Links(std::ofstream &outfile)
{   
	outfile << num_nodes << std::endl;
	outfile << num_links << std::endl;
	for(int i=0; i < num_nodes ; i++)		// Store all nodes
		node[i]->Save(outfile);
	for(int i=0 ; i < num_links; i++)			// Store all links
		link[i]->Save(outfile);
};

void Base_Network::Load_Nodes_Links(std::ifstream &infile)
{  
	infile >> num_nodes;
	infile >> num_links;

	Create_Network();
	for(int i = 0; i < num_nodes ; i++)		// Load all nodes
		node[i]->Load(infile);	
	for(int i=0; i < num_links; i++)			// Load all links
		link[i]->Load(infile);
}

Base_Network::Base_Network(void) : Base_Node( 0, 0)	// Constructor
{
	num_nodes = 0;
	num_links = 0;
	node = NULL;
	link = NULL;
};

Base_Network::~Base_Network(void)				// Destructor
{
	if(node !=NULL )
	{
		for(int i=0; i < num_nodes; i++)		// Free all nodes
			delete node[i];
		for(int i = 0; i < num_links; i++)			// Free all links
			delete link[i];
		delete []node;
		delete []link;
	}
};

void Base_Network::Print(std::ofstream &outfile)
{   
	for (int i = 0; i < num_nodes; i++)		// Print each node in network
		node[i]->Print(outfile);
};

char *Base_Network::Get_Name(void)
{   
	static char name[]="BASE_NETWORK";
	return name;
};

void Base_Network::Epoch(int code)
{   
	for(int i=0; i < num_nodes; i++)	// Run Epoch for each node in network
		node[i]->Epoch(code);
	for(int i =0; i < num_links; i++)		// Run Epoch for each link in network
		link[i]->Epoch(code);
}

// --------------------------------------------
// Linked-List Constructor

LList::LList(void)
{
	curr=head=tail=NULL;
	count=0;
}

LList::~LList(void)
{
	Clear();
}

int LList::Count(void)
{
	return count;
}
// ---------------------------------------
// Clear out the contents of a list
void LList::Clear(void)
{
	NODE *i=head, *temp;

	while(i != NULL)
	{
		temp = i;
		i = i->next;
		delete temp;
	}
	curr=head=tail=NULL;
	count =0;
}
// ----------------------------------------
// Add an element to the tail of a list
int LList::Add_To_Tail(Base_Link *element)
{
	curr=NULL;
	return Add_Node(element);
}
// -------------------------------------------------------------------
// This function add a node before the node curr points to. If curr is
// NULL then the node is added to the tail of the list
int LList::Add_Node(Base_Link *element)
{
	NODE *temp=new NODE;

	if(temp == NULL)
	{
		std::cout << "Unable to allocate Node..." << std::endl;
	}

	temp->element = element;

	if(temp == NULL)
		return 0;

	if(curr==NULL)
	{
		temp->prev=tail;
		temp->next=NULL;

		if(tail == NULL)
		{
			head=temp;
			tail=temp;
		}
		else
		{
			tail->next = temp;
			tail=temp;
		}
	}
	else if(curr == head)
	{
		temp->prev= NULL;
		temp->next= head;
		if(head == NULL)
		{
			head = temp;
			tail = temp;
		}
		else
		{
			head->prev = temp;
			head = temp;
		}
	}
	else
	{
		temp->prev=curr->prev;
		temp->next=curr;
		curr->prev->next = temp;
		curr->prev = temp;
	}
	count++;
	return 1;
}
// --------------------------------------------------------------------
// Function to verify existence of element in list and returns position
int LList::Find(Base_Link *element)
{
	NODE *temp=head;
	int cnt=1;
	curr = NULL;

	while(temp != NULL)
	{
		if(temp->element == element)
		{
			curr=temp;
			return cnt;
		}
		cnt++;
		temp=temp->next;
	}
	return 0;
}
// ------------------------------------------------------------------------
// Deletes an element anywhere in a list(first occurence)
int LList::Del(Base_Link *element)
{
	if(!Find(element))
		return 0;
	return Del_Node();
}
// -------------------------------------------------------------------------
// This function deletes the current node in the list(curr)
int LList::Del_Node(void)
{
	if(curr == NULL)
		return 0;
	delete curr->element;

	if(curr == head)
	{
		if(head==tail)
			tail=NULL;
		else head->next->prev=NULL;
			head = curr->next;
	}
	else if(curr==tail)
	{
		tail->prev->next=NULL;
		tail = curr->prev;
	}
	else
	{
		curr->next->prev = curr->prev;
		curr->prev->next = curr->next;
	}
	delete curr;
	curr = NULL;
	count--;
	return 1;
}
// ------------------------------------------------------------------------
// Reset current node position (curr) to head of list
void LList::Reset_To_Head(void) 
{
	curr = head;
}
// ------------------------------------------------------------------------
// Reset current node position (curr) to tail of list
void LList::Reset_To_Tail(void)
{
	curr = tail;
}
// ------------------------------------------------------------------------
// Return the Current element pointed to in the Container
Base_Link *LList::Curr(void)
{
	if(curr == NULL)
		return NULL;
	else 
		return curr->element;
}
// ------------------------------------------------------------------------
// Advances current node
void LList::Next(void)
{
	if(curr->next == NULL)
		curr=head;
	else 
		curr = curr->next;
}
// ------------------------------------------------------------------------
// Mode the current node backwards
void LList::Prev(void)
{
	if(curr->prev == NULL)
		curr = tail;
	else
		curr = curr->prev;
}

#endif BASE