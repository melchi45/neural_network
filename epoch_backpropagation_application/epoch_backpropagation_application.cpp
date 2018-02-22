// Epoch_BackProp_App.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout
#include <time.h>
#include <assert.h>
#include "EpochBP.h"
#include "GenBP.h"

using namespace std;

#define MAX_PATTERNS 10000

int main(int argc, char* argv[])
{

	srand((unsigned) time(NULL));

	double learning_rate;
	double momentum_term;
	double tolerance;
	int display_interval;
	int num_layers;
	int input_nodes;
	int output_nodes;
	int *node_cnt;
	int mode;

	enum {RUN, LEARN};

	cout << endl;
	
	if(argc == 1)
	{
		cout << "Usage : " << argv[0] << endl << endl
			<< "		<LEARN> <pattern file> <network file>" << endl
			<< "		<learning rate> <momentum term>" << endl
			<< "		<tolerance> <display> <input nodes>" << endl
			<< "		| <hidden nodes> | <output nodes>";
		cout << endl << endl << endl;
		cout << "Usage : " << argv[0] << endl << endl
			<< "		<RUN> <pattern file> <network file>"
			<< endl << endl << endl;
		exit(0);
	}

	cout << "Mode			: " << argv[1] << endl;
	cout << "Pattern File		: " << argv[2] << endl;
	cout << "Network File		: " << argv[3] << endl;

	if(toupper(argv[1][0]) == 'R') // RUN Mode
	{
		mode = RUN;
		if(argc < 4)
		{
			cout << "Invalid number of parameters for Run mode" << endl;
		}
	}
	else if(toupper(argv[1][0]) == 'L') // Learn Mode
	{
		mode = LEARN;
		if(argc < 10)
		{
			cout << "Invalid number of parameter for Learn mode" <<  endl;
		}
		learning_rate = atof(argv[4]);
		momentum_term = atof(argv[5]);
		tolerance = atof(argv[6]);
		display_interval = atoi(argv[7]);

		num_layers=argc-8;
		node_cnt = new int[num_layers];
		for(int i = 8; i < argc; i++)
		{
			node_cnt[i-8] = atoi(argv[i]);
		}

		cout << "Learning Rate		: " << learning_rate << endl;
		cout << "Mementum Term		: " << momentum_term << endl;
		cout << "Tolerance		: " << tolerance << endl;
		cout << "Display Interval	: " << display_interval << endl;

		output_nodes = node_cnt[num_layers-1];
		input_nodes = node_cnt[0];

		cout << "Layers			: " << num_layers << endl;
		cout << "Input Node		: " << node_cnt[0] << endl;
		for(int i = 1; i < num_layers-1 ; i++)
		{
			cout << "Hidden Layer		: " << node_cnt[i] << endl;
		}
		cout << "Output Node		: " << node_cnt[num_layers-1] << endl;
		cout << endl;
	}
	else
	{
		cout << "Invalid mode" << endl;
		exit(0);
	}

	if(mode == LEARN)
	{
		Pattern *data[MAX_PATTERNS];
		int pattern_count = 0;

		ifstream infile(argv[2]);
		assert(infile);

		while( !infile.eof() && !infile.fail())
		{
			data[pattern_count] = new Pattern(input_nodes, output_nodes, infile);
			pattern_count++;
		}
		
		infile.close();
		
		for(int i = 0; i <  pattern_count; i++)
		{
			data[i]->Print();
		}
		
		Generic_BackProp<Input_Node, BP_Hidden_Node, BP_Output_Node, Epoch_BP_Link>
			BPnet(learning_rate, momentum_term, num_layers, node_cnt);
		
		long iteration = 0;
		int good = 0;
		double total_error;
		int best = 0;
		ofstream ofile("test.out");
		BPnet.Print(ofile);
		ofile.close();
		
		while(good < pattern_count)
		{
			good = 0;
			total_error = 0.0;
			BPnet.Epoch(pattern_count);
			for(int i = 0; i <pattern_count; i++)
			{
				BPnet.Set_Value(data[i]);
				BPnet.Run();
				
				BPnet.Set_Error(data[i]);
				
				BPnet.Learn();
				int good_outputs = 0;
				
				for(int j =0; j < output_nodes; j++)
				{
					if(fabs(BPnet.Get_Value(j) - data[i]->Out(j)) < tolerance)
						good_outputs++;
					total_error += sqrt(BPnet.Get_Error(j));
				}
				if(good_outputs == output_nodes)
					good++;
			}
			
			if(best < good)
			{
				best = good;
				
				ofstream outfile(argv[3]);
				
				BPnet.Save(outfile);
				outfile.close();
				cout << "[Network Save]" << endl;
			}
			
//			if(iteration %display_interval == 0) 
				cout << iteration << ".     " << good
				<< "/" << pattern_count << "  rmse : "
				<< setprecision(15) << total_error << endl;
			iteration++;
		}
		
		cout << endl;
		cout << "Traning Complete........." << endl;
		
		ofstream outfile(argv[3]);
		BPnet.Save(outfile);
		outfile.close();

	}
	else
	{
		Generic_BackProp<Input_Node, BP_Hidden_Node, BP_Output_Node, BP_Link> BPnet(argv[3]);

		input_nodes = BPnet.Get_Layer_Count(0);
		output_nodes = BPnet.Get_Layer_Count(BPnet.Get_Num_Layers() -1);

		Pattern *input_data;

		ifstream infile(argv[2]);
		assert(infile);

		while( !infile.eof() && !infile.fail())
		{
			input_data = new Pattern(input_nodes, output_nodes, infile);

			BPnet.Set_Value(input_data);
			BPnet.Run();

			cout << input_data->Get_ID() << "    ";
			for(int j = 0; j < input_nodes; j++)
				cout << input_data->In(j) << " ";
			cout << "    ";
			for(int j = 0; j < output_nodes; j++)
				cout << BPnet.Get_Value(j) << " ";
			cout << endl;

			delete input_data;
		}

		infile.close();
		
	}

	return 0;
}

