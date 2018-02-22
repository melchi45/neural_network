// BackProp_Example1.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout
#include <conio.h>
#include <assert.h>
#include <time.h>

#include "BackProp.h"
#include "Common.h"

using namespace std;

int main(int argc, char* argv[])
{
	srand(1);

	Pattern *data[4];

	data[0] = new Pattern( 2, 1, 1, 0.0, 0.0, 0.0);
	data[1] = new Pattern( 2, 1, 2, 0.0, 1.0, 1.0);
	data[2] = new Pattern( 2, 1, 3, 1.0, 0.0, 1.0);
	data[3] = new Pattern( 2, 1, 4, 1.0, 1.0, 0.0);

	Base_Node *Node[6];
	Base_Link *Link[9];

	Node[0] = new Input_Node;					// Input Layer
	Node[1] = new Input_Node;					// Input Layer
	Node[2] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[3] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[4] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[5] = new BP_Output_Node( 0.4, 0.9);	// Output Layer

	for(int i = 0; i < 9; i++)
		Link[i] = new BP_Link();

	int curr = 0;

	for(int i = 2; i <= 4; i++)
	{
		for(int j = 0; j <= 1; j++)
			Connect(Node[j], Node[i], Link[curr++]);
	}

	for(int j = 2; j <= 4; j++)
		Connect(Node[j], Node[5], Link[curr++]);

		ofstream outfile("Network.net");

	for(int i = 0 ; i < 6 ; i++)
	{
		Node[i]->Print(outfile);
	}
	
	outfile.close();
		
	long iteration = 0;
	int good = 0;

	double tolerance = 0.5;
	double total_error;

	while( good <4)
	{
		good=0;

		total_error = 0.0;

		for(int i = 0; i < 4; i++)
		{
			Node[0]->Set_Value(data[i]->In(0));
			Node[1]->Set_Value(data[i]->In(1));

			for(int j = 2; j <= 5; j++)  // Foward Pass
				Node[j]->Run();

			Node[5]->Set_Error(data[i]->Out(0)); // Set Error Value

			for(int j = 5; j >= 2; j--) // Backward Pass
				Node[j]->Learn();

			if(fabs(Node[5]->Get_Value() - data[i]->Out(0)) < tolerance)
				good++;

			total_error += fabs(Node[5]->Get_Error());
		}
		if(iteration % 1000 == 0)
			cout << iteration << ".    " << good << "/4"
				<< "    rmse : " << setprecision(15) << total_error << endl;
		iteration++;
	}

	ofstream bp1("bp1.net");

	for(int i = 0; i < 6; i++)
		Node[i]->Save(bp1);
	for(int i = 0; i < 9; i++)
		Link[i]->Save(bp1);

	bp1.close();

	for(int i = 0; i < 6; i++)
		delete Node[i];
	for(int i = 0; i < 9; i++)
		delete Link[i];
	
	Node[0] = new Input_Node;					// Input Layer
	Node[1] = new Input_Node;					// Input Layer
	Node[2] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[3] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[4] = new BP_Hidden_Node( 0.4, 0.9);	// Hidden Layer
	Node[5] = new BP_Output_Node( 0.4, 0.9);	// Output Layer

	for(int i = 0; i < 9; i++)
		Link[i] = new BP_Link();

	curr=0;

	for(int i = 2 ; i <= 4; i++)
		for(int j = 0; j <= 1; j++)
			Connect(Node[j], Node[i], Link[curr++]);

	for(int j = 2; j <= 4; j++)
		Connect(Node[j], Node[5], Link[curr++]);

	ifstream infile("bp1.net");
	for(int i=0; i <6; i++)
		Node[i]->Load(infile);
	for(int i = 0; i<9; i++)
		Link[i]->Load(infile);

	infile.close();

	for(int i = 0; i < 4; i++)
	{
		Node[0]->Set_Value(data[i]->In(0));
		Node[1]->Set_Value(data[i]->In(1));
		
		for(int j = 2; j <= 5; j++)  // Foward Pass
			Node[j]->Run();

		double out = Node[5]->Get_Value();
		
		cout << "Pattern : " << setw(3) << "    Input : ("
			<< data[i]->In(0) << ","
			<< data[i]->In(1)
			<< ")    BackProp: (" << out
			<< ")    Actual : (" << data[i]->Out(0) << ")" <<endl;
	}

	for(int i = 0; i < 6; i++)
		delete Node[i];
	for(int i = 0; i < 9; i++)
		delete Link[i];

	return 0;
}

