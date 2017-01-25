/*
 * ActorEdge.hpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the header for the class representing the edge, containing 
 * its data, and the functions to access it.
 */

#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP
#include <string>
#include <vector>
#include "ActorNode.hpp"
#include <iostream>

using namespace std;
class ActorNode;
class ActorEdge{

protected: 

 	// Nondirectional
	ActorNode * actor1;
	ActorNode * actor2;
	string movie;
	int year;
	int weight;



public: 


	ActorEdge(void);
	ActorEdge(ActorNode * node1, ActorNode * node2, string movie);

	// Used to access movie title
	string getMovie();

	// Used to cross from one node to the one its connected to
	ActorNode * crossEdge(ActorNode * a);

	// used to access weight
	int getWeight();
	

	
};

#endif // ACTOREDGE_HPP
