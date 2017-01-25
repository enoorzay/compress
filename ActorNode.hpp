/*
 * ActorNode.hpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This is the header for the class representing the node, containing 
 * its data, and the functions to access it.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP
#include <string>
#include <vector>
using namespace std;

class ActorEdge;
class ActorNode {



protected: 
	string name;


public: 
	int earliestfilm; // earliest connection

	// all edges. This is essentially a column of adj list
	vector<ActorEdge *> edges;

	// path algorithm values
	bool done;
	int dist;
	ActorEdge * prevEdge;

	// Used for extension
	vector<string> movies;

	ActorNode(void);
	ActorNode(string n);

	// Used to access name
	string getName();

	// Adds edge to vector
	void addEdge(ActorEdge * edgetoadd);

	// Adds movie to vector
	void addMovie(string mov);

	// Returns output of all movies casted in
	string listMovies();


};

#endif // ACTORNODE_HPP
