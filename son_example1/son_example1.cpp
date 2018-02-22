// Self_Organizing_Map.cpp : Defines the entry point for the console application.
//
#include "Son.h"

int main(int argc, char* argv[])
{

	Pattern * data[300];
	std::ifstream infile("cross.dat");
	for(int i = 0; i < 300; i++)
	{
		data[i] = new Pattern(2, 0, infile);
	}

	infile.close();

	Base_Node *node[2];
	Base_Node *K_node[10][10];
	Base_Link *link[200];

	int x, y;
	int inputs =2, x_size = 10, y_size = 10;
	
	for(int i = 0; i < inputs; i++)
	{
		node[i] = new Input_Node;
	}

	int curr = 0;
	for(int x =0; x < x_size ; x++)
	{
		for(y = 0; y < y_size ; y++)
		{
			K_node[x][y] = new SON_Node;
			for(int i =0; i < inputs; i++)
			{
				link[curr] = new SON_Link;
				Connect(node[i], K_node[x][y], link[curr++]);
			}
		}
	}

	long iteration = 0;
	int good = 0;
	double initial_learning_rate = 0.5, final_learning_rate = 0.01, learning_rate;
	int neighborhood = 5;
	long final_iteration = 5000;
	int neighborhood_decrement = 1000;
	double node_value, min_value;
	int min_x, min_y;
	int x_start, x_stop, y_start, y_stop;

	std::ofstream outfile("cross.out");

	for(iteration = 0; iteration < final_iteration; iteration++)
	{
		learning_rate = initial_learning_rate - 
			(((double)iteration / (double)final_iteration) *
			(initial_learning_rate - final_learning_rate));

		if((iteration +1) % neighborhood_decrement == 0 && neighborhood > 0)
			neighborhood--;

		for(int i = 0; i < 300; i++)
		{
			node[0]->Set_Value(data[i]->In(0));
			node[1]->Set_Value(data[i]->In(1));

			min_value = 99999.0;

			for(x = 0; x < x_size ; x++)
			{
				for(y = 0; y < y_size ; y++)
				{
					K_node[x][y]->Run();
					node_value = K_node[x][y]->Get_Value();

					if(node_value < min_value)
					{
						min_value = node_value;
						min_x = x; min_y = y;
					}
				}
			}
			
			int x_start = min_x - neighborhood;
			int x_stop = min_x + neighborhood;
			int y_start = min_y - neighborhood;
			int y_stop = min_y + neighborhood;
			
			if(x_start < 0)
				x_start = 0;
			if(x_stop >= x_size) 
				x_stop = x_size - 1;
			if(y_start < 0)
				y_start = 0;
			if(y_stop >= y_size)
				y_stop = y_size - 1;
			
			for(int x = x_start; x <= x_stop; x++)
			{
				for(int y = y_start; y <= y_stop; y++)
				{
					K_node[x][y]->Set_Value(learning_rate, LEARNING_RATE);
					K_node[x][y]->Learn();
				}
			}
		}
		
//		if(iteration % 10 == 0)
		std::cout << iteration << ". Learning Rate : " << learning_rate
			<< " Neighborhood: " << neighborhood << std::endl;
		
//		if(iteration % 50 ==0)
		{
			for( x = 0; x < x_size; x++)
			{
				for(y = 0; y <y_size; y++)
				{
					K_node[x][y]->Print(outfile);
				}
			}
		}
	}

	outfile.close();

	std::ofstream outnet("son1.net");

	for(int i=0; i<inputs; i++)
		node[i]->Save(outnet);
	for(int x = 0; x < x_size; x++)
	{
		for(y = 0; y < y_size; y++)
		{
			K_node[x][y]->Save(outnet);
		}
	}

	for(int i =0; i< x_size * y_size; i++)
		link[i]->Save(outnet);

	outnet.close();

	for(int i =0; i < inputs; i++)
	{
		delete node[i];
	}

	for(int x = 0; x < x_size; x++)
	{
		for(int y = 0; y < y_size; y++)
		{
			delete K_node[x][y];
		}
	}

	for(int i =0; i< x_size * y_size; i++)
		delete link[i];


	return 0;
}

