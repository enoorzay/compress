/*
 * pathfinder.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the file for the driver to create 
 * a graph, either with weighted or weightless edges 
 * depending on arguments, and to read pairs from an infile
 * and output their shortest paths to outfile
 */

#include "ActorGraph.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	bool weighted;
	char weightselection;
	
	if(argc != 5)
	{
		cout << "Invalid number of arguments.\n" 
		     << "Usage: ./pathfinder <casting data file> <w or u> <pair file> <output file>\n";
		return -1;
	}	
	
	// Weighted or weightless?
	weightselection = *(argv[2]);
	if (weightselection == 'w')
	{
		weighted = true;
	}
	
	else if (weightselection == 'u')
	{
		weighted = false;
	}
	
	
	else 
	{
		weighted = false;
		cout << "Error: Expected 'u' or 'w' for 3rd arg ";
		return -1;
	}
	
	// Initialze and load graph
	ActorGraph mygraph = ActorGraph();
	bool loaded;
	loaded = mygraph.loadFromFile(argv[1], weighted, false);
	
	if (loaded)
	{
		cout << "done" << '\n';
	}
	else 
	{
		cout << "FAILURE;" << '\n';
	}


    
	// Read pairs from file, passing weighted bool
	mygraph.readPairs(argv[3], argv[4], weighted, false);


	
	
	
	
}
