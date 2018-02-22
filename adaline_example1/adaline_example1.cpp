// Adaline_Example1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout

#include "Base.h"
#include "Adaline.h"
#include "Pattern.h"

using namespace std;

// Using ADALINE_Network Class
int main(int argc, char* argv[])
{
	srand(1);
	int i;

	Pattern *data[250];
	ifstream indata("lin2var.trn");
	
	if(indata.fail())
	{
		cout << "Unable to open Input Pattern File..." << endl;
		exit(0);
	}

	// Train Adaline
	for(i =0; i<250; i++)
	{
		data[i] = new Pattern(2,1,indata);
		data[i]->Print();
	}

	indata.close();

	ADALINE_Network a(2, 0.45);

	int iteration = 0;
	int good = 0;

	while(good < 250)
	{
		good = 0;
		for(int i =0; i <250; i++)
		{
			a.Set_Value(data[i]);

			a.Run();

			if(data[i]->Out(0) != a.Get_Value())
			{
				a.Learn();
				break;
			}
			else good++;
		}
		cout << iteration  << ".    " << good << "/250" << endl;
		iteration++;
	}

	ofstream adaline("adaline2.net");
	a.Save(adaline);
	adaline.close();

	ADALINE_Network b("adaline2.net");

	for(i =0; i <250; i++)
	{
		b.Set_Value(data[i]);

		b.Run();

		cout << setw(3) << i << "   Net : " << setw(3) << setprecision(7)
			<< b.Get_Value() << "   Actual : " << setprecision(7)
			<< data[i]->Out(0) << endl;
	}

	return 0;
}


/* Does not Using ADALINE_Network
int main(int argc, char* argv[])
{
	srand(1);
	int i;

	Pattern *data[250];
	ifstream indata("lin2var.trn");
	
	if(indata.fail())
	{
		cout << "Unable to open Input Pattern File..." << endl;
		exit(0);
	}

	// Train Adaline
	for(i =0; i<250; i++)
	{
		data[i] = new Pattern(2,1,indata);
		data[i]->Print();
	}

	indata.close();

	Base_Node *Node[4];
	Base_Link *Link[3];

	Node[0] = new Input_Node;
	Node[1] = new Input_Node;
	Node[2] = new Bias_Node;
	Node[3] = new ADALINE_Node(0.45);

	Link[0] = new ADALINE_Link;
	Link[1] = new ADALINE_Link;
	Link[2] = new ADALINE_Link;

	Connect(Node[0], Node[3], Link[0]);
	Connect(Node[1], Node[3], Link[1]);
	Connect(Node[2], Node[3], Link[2]);

	ofstream outfile("Network.net");

	for(i = 0 ; i < 4 ; i++)
	{
		Node[i]->Print(outfile);
	}
	
	outfile.close();

	int iteration = 0;
	int good = 0;

	while(good < 250)
	{
		good = 0;
		for(int i =0; i <250; i++)
		{
			Node[0]->Set_Value(data[i]->In(0));
			Node[1]->Set_Value(data[i]->In(1));

			Node[3]->Run();

			if(data[i]->Out(0) != Node[3]->Get_Value())
			{
				Node[3]->Learn();
				break;
			}
			else good++;
		}
		cout << iteration  << ".    " << good << "/250" << endl;
		iteration++;
	}

	ofstream adaline("adaline1.net");

	for(i = 0 ; i < 4 ; i++)
		Node[i]->Save(adaline);
	for(i = 0 ; i < 3 ; i++)
		Link[i]->Save(adaline);

	adaline.close();

	for(i = 0 ; i < 4 ; i++)
		delete Node[i];
	for(i = 0 ; i < 3 ; i++)
		delete Link[i];

	// Run AdaLine

	Node[0] = new Input_Node;
	Node[1] = new Input_Node;
	Node[2] = new Bias_Node;
	Node[3] = new ADALINE_Node(0.45);

	Link[0] = new ADALINE_Link;
	Link[1] = new ADALINE_Link;
	Link[2] = new ADALINE_Link;

	Connect(Node[0], Node[3], Link[0]);
	Connect(Node[1], Node[3], Link[1]);
	Connect(Node[2], Node[3], Link[2]);

	indata.open("adaline1.net");

	for(i = 0 ; i < 4 ; i++)
		Node[i]->Load(indata);
	for(i = 0 ; i < 3 ; i++)
		Link[i]->Load(indata);

	for(i =0; i <250; i++)
	{
		Node[0]->Set_Value(data[i]->In(0));
		Node[1]->Set_Value(data[i]->In(1));

		Node[3]->Run();

		cout << "Pattern : " << setw(3) << i << "    Input: ("
			<< data[i]->In(0) << ","
			<< data[i]->In(1) << ")    ADALINE :"
			<< setw(3) << Node[3]->Get_Value() << "    Actual :"
			<< setw(3) << data[i]->Out(0) << endl;
	}

	for(i = 0 ; i < 4 ; i++)
		delete Node[i];
	for(i = 0 ; i < 3 ; i++)
		delete Link[i];


	return 0;
}
*/
/**********************************************************************
//  Make Sample Traning Set
/*	double x, y, y1, output;
	ofstream outfile("lin2var.trn");
	for( int i=0; i < 250; i++)
	{
		x=((double)rand() / (double)RAND_MAX)*2.0-1.0;
		y=((double)rand() / (double)RAND_MAX)*2.0-1.0;

		y1=(-5*x-2)/-4;

		if(y <y1) output =1;
		else output=-1;

		outfile << i << " " << x << " " << y << " " << output << endl;
	}

	outfile.close();
*/


