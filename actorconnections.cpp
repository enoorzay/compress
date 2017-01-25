/*
 * actorconnections.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the file for the driver to create 
 * an edgeless graph, adding edges to determine
 * when pairs of vertices given by an input file
 * first become connected
 */

#include "ActorGraph.hpp"
using namespace std;

int main(int argc, char* argv[])
{ 
	// ufind by default
	bool bfs = true;
	

	if(argc != 5){
		cout << "Invalid number of arguments.\n" 
		     << "Usage: ./pathfinder <casting data file> <actor pairs> "
		 	 << "<output file> <bfs or ufind>\n";
		return -1;
	}	
	


	// Initialize graph
	ActorGraph mygraph = ActorGraph();
	
	// load graph and give appropriate message
    bool loaded;
	loaded = mygraph.loadFromFile(argv[1], false, true);
	
	if (loaded)
	{
		cout << "done" << '\n';
	}
	else 
	{
		cout << "FAILURE;" << '\n';
	}

	/* read pairs from input file, passes false since unweighted
	 * and true since it is using a blank graph */
	mygraph.readPairs(argv[2], argv[3], false, true);

	// Passes outfile to connectionFinder so it can write solution
	mygraph.connectionFinder(argv[3]);


}
