// BackProp_Example2.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout
//#include <conio.h>
#include <assert.h>
#include <time.h>

#include "Backprop.h"
#include "Common.h"

using namespace std;

int main(int argc, char* argv[])
{
	srand(1);

	Pattern *data[150];

	ifstream infile("iris.dat");

	for(int i =0; i < 150 ; i++)
	{
		data[i] = new Pattern(4,3,infile);
		data[i]->Print();
	}

	infile.close();

	BackProp_Network a(0.3, 0.9, 3, 4, 5, 3);
	
	long iteration = 0;

	int good = 0;
	double tolerance = 0.5;
	double total_error;
	
	while( good < 150)
	{
		good = 0;
		total_error=0.0;

		for(int i=0; i<150; i++)
		{
			a.Set_Value(data[i]);

			a.Run();

			a.Set_Error(data[i]);

			a.Learn();

			if(fabs(a.Get_Value(0) - data[i]->Out(0)) < tolerance)
				good++;
			total_error += fabs(a.Get_Error(0));
		}

		if(iteration % 1000 == 0) 
			cout << iteration << ".     " << good
				<< "/150 rmse : " << setprecision(15)
				<< total_error << endl;
		iteration++;
	}

	ofstream outfile("iris.net");
	a.Save(outfile);
	outfile.close();
	
	BackProp_Network b;

	ifstream innet("iris.net");
	b.Load(innet);
	innet.close();

	for(int i=0 ; i < 150; i++)
	{
		b.Set_Value(data[i]);
		b.Run();

		int out1=(int)(b.Get_Value(0)+0.5); //Get Output from Network
		int out2=(int)(b.Get_Value(1)+0.5); //Get Output from Network
		int out3=(int)(b.Get_Value(2)+0.5); //Get Output from Network

		cout << "Pattern : " << setw(3) << i << "  Input : ("
			<< data[i]->In(0) << ","
			<< data[i]->In(1) << ","
			<< data[i]->In(2) << ","
			<< data[i]->In(3) 
			<< ")   BackProp : (" << out1 << "," << out2 << "," << out3
			<< ")   Actual : (" 
			<< data[i]->Out(0) << ","
			<< data[i]->Out(1) << ","
			<< data[i]->Out(2)
			<< ")" << endl;

	}

	return 0;
}

