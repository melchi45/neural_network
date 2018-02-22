// Adaline_Example2.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout

#include "Base.h"
#include "Adaline.h"
#include "Pattern.h"

using namespace std;

int main(int argc, char* argv[])
{
	srand(1);

	Pattern *data[250];

	ifstream infile("lin2var.trn");
	if(infile.fail())
	{
		cout << "Could not open pattern file..." << endl;
		exit(0);
	}

	for(int i = 0; i <250 ; i++)
		data[i] = new Pattern(2,1,infile);
	infile.close();

	ADALINE_Network a(2, 0.45);

	int iteration = 0;
	int good = 0;

	while( good <250)
	{
		good = 0;
		for( int i =0; i < 250; i++)
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
		cout << iteration << ".    " << good << "/250" << endl;
		iteration++;
	}

	ofstream outfile("test.dta");
	a.Save(outfile);
	outfile.close();

	ADALINE_Network b("test.dta");

	for(int i = 0; i<250 ; i++)
	{
		b.Set_Value(data[i]);
		b.Run();

		cout << setw(3) << i << " Net : " << setprecision(7)
			<< b.Get_Value() << "   Actual : " << setprecision(7)
			<< data[i]->Out(0) << endl;
	}


	return 0;
}

