//****************************************************************************
//  File Name	: GenBP.h
//	Purpose		: Generic BackPropagation Neural Network Class
//  Creator		: Young ho Kim<melchi45@gmail.com>
//				  MFC Forum <https://mfcforum.blogspot.kr/>
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout
//#include <conio.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include "BackProp.h"
#include "Common.h"

template<class INPUT_NODE, class HIDDEN_NODE, class OUTPUT_NODE, class LINK>
class Generic_BackProp : public BackProp_Network
{
	public:
		Generic_BackProp(char *filename);
		Generic_BackProp(double lr, double mt, int layers, ...);
		Generic_BackProp(void) : BackProp_Network(){};
		Generic_BackProp(double lr, double mt, int layers, int nodes[]);

		virtual void Create_Network(void);

		char *Get_Name(void)
		{
			static char name[] = "Generic_BackProp_Network";
			return name;
		};
};		

template<class INPUT_NODE, class HIDDEN_NODE, class OUTPUT_NODE, class LINK>
	Generic_BackProp<INPUT_NODE, HIDDEN_NODE, OUTPUT_NODE, LINK>::
	Generic_BackProp(char filename[]) : 
		BackProp_Network()
{
	ifstream infile(filename);
	Load(infile);
	infile.close();
};

template<class INPUT_NODE, class HIDDEN_NODE, class OUTPUT_NODE, class LINK>
	Generic_BackProp<INPUT_NODE, HIDDEN_NODE, OUTPUT_NODE, LINK>::
	Generic_BackProp(double lr, double mt, int layers, ...) : 
		BackProp_Network()
{
	num_nodes = 0;
	num_links = 0;

	num_layers = layers;
	va_list vl;
	va_start(vl, layers);
	node_cnt = new int[layers];

	for(int i=0; i<layers; i++)
	{
		node_cnt[i] = va_arg(vl, int);
		num_nodes += node_cnt[i];

		if( i > 0)
			num_links += node_cnt[i-1] * node_cnt[i]; // links between layers
	}

	va_end(vl);

	learning_rate = lr;
	momentum_term = mt;

	Create_Network();
};

template<class INPUT_NODE, class HIDDEN_NODE, class OUTPUT_NODE, class LINK>
	Generic_BackProp<INPUT_NODE, HIDDEN_NODE, OUTPUT_NODE, LINK>::
	Generic_BackProp(double lr, double mt, int layers, int nodes[]) : 
		BackProp_Network()
{
	num_nodes = 0; 
	num_links = 0;

	num_layers = layers;
	node_cnt = new int[layers];

	for(int i = 0; i<layers; i++)
	{
		node_cnt[i] = nodes[i];
		num_nodes += node_cnt[i];

		if(i > 0)
			num_links += node_cnt[i-1] * node_cnt[i]; // link between layers
	}

	learning_rate = lr;
	momentum_term = mt;

	Create_Network();
};

template<class INPUT_NODE, class HIDDEN_NODE, class OUTPUT_NODE, class LINK>
void Generic_BackProp<INPUT_NODE, HIDDEN_NODE, OUTPUT_NODE, LINK>::
	Create_Network(void)
{
	node = new Base_Node*[num_nodes];
	link = new Base_Link*[num_links];

	int curr = 0;
	for(int i = 0; i < node_cnt[0]; i++)
		node[curr++] = new INPUT_NODE;

	first_hidden_node = curr;
	for(int i = 1; i < num_layers-1; i++)
		for(int j = 0 ; j < node_cnt[i]; j++)
			node[curr++] = new HIDDEN_NODE(learning_rate, momentum_term);

	first_output_node = curr;

	for(int i = 0; i < node_cnt[num_layers-1] ; i++)
		node[curr++] = new OUTPUT_NODE(learning_rate, momentum_term);

	for(int i = 0; i < num_links; i++)
		link[i] = new LINK;

	curr = 0;

	int layer1 = 0, layer2 = first_hidden_node;

	for(int i= 0; i < num_layers -1 ; i++)
	{
		for(int j=0; j<node_cnt[i+1]; j++)
			for(int k =0; k < node_cnt[i]; k++)
				Connect(node[layer1+k], node[layer2+j], link[curr++]);

		layer1 = layer2;
		layer2 += node_cnt[i+1];
	}
};
