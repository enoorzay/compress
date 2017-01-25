/*
 * ActorNode.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the file for the class representing the node, containing 
 * its data, and the functions to access it.
 */


#include "ActorNode.hpp"

using namespace std;

ActorNode::ActorNode(void) {

}

ActorNode::ActorNode(string n)
{
	name = n;
	dist = 999;	// initialized for BFS and djik
	done = false;
	earliestfilm = 9999;
}


// access name
string ActorNode::getName()
{
	return name;
}

// add new edge to vector
void ActorNode::addEdge(ActorEdge * edge)
{
	edges.push_back(edge);

}

// add new movie to vector
void ActorNode::addMovie(string mov)
{
	movies.push_back(mov);
}

// returns formatted output of all movies cast in
string ActorNode::listMovies()
{
	string list = movies[0];
	for (int i = 1; i < movies.size(); i++)
	{
		list += ", \t" + movies[i];

	}
	list += "\n";
	return list;
}
