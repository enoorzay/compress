/*
 * ActorEdge.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the file for the class representing the edge, containing 
 * its data, and the functions to access it.
 */



#include "ActorEdge.hpp"

using namespace std;

ActorEdge::ActorEdge(void) 
{
//	allEdges.push_back(this);
}

ActorEdge::	ActorEdge(ActorNode * node1, ActorNode * node2, string mov)
{
	// Initialize values from parameters
	actor1 = node1;
	actor2 = node2;
	movie = mov;
	year= stoi(movie.substr(movie.length() - 4));
	weight = 1 + (2015 - year);
}

// used to access movie title
string ActorEdge::getMovie()
{

	return movie;
	

}

/* "Moves across an edge" by passing current node
 *  as parameter to this, which returns a pointer 
 *  to the other node.
 */
ActorNode * ActorEdge::crossEdge(ActorNode * a)
{

	if (a == actor1)
	{
		return actor2;
	}
	else if (a == actor2)
	{
		return actor1;
	}

	else
	{
		cout << "ERROR";
		return 0;
	}
}
// Accesses weight
int ActorEdge::getWeight()
{
	return weight;
}


