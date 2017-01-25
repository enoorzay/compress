/*
 * ActorGraph.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/02/2016>
 *
 * This is the header for the ActorGraph class and its functions. The class implements
 * a graph of ActorNode vertices and ActorEdge edges, along with methods to 
 * traverse the graph.
 */
#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#define ERROR "No connection"

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <stack>
#include "ActorNode.hpp"
#include "ActorEdge.hpp"

using namespace std;


class ActorGraph {
protected:
  
     // Holds actor object at key of its name
    unordered_map<string, ActorNode * > actors; 

    // movie title key containing cast of movie 
    unordered_map<string, vector <ActorNode *> > movies;

    // Movie titles sorted by year
    unordered_map<int, vector <string> > sortedmovies;

    vector<ActorEdge *> allEdges;

    /* Holds all the pairs which are scanned for connections together when
    * adding edges by year
    */
    vector<string> Actorpairs;


public:
    // earliest year a connection exists
    int earliest;
    ActorGraph(void);

    vector<ActorNode *> allNodes;

 
    /* This function reads from the input file and calls the appropriate
    * graph building function depending on the parameter passed to it 
    */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges, bool blank );
   

    /* This function adds an actor to a movie's cast, creating the actor
    *   and movie if either do not exist, otherwise gets the movies cast
    *   and adds an edge from each of them to the new actor. Called iteratively
    *   from LoadfromFile and addYear.
    */
    bool buildGraph (string actor_name, string movie_title);
    


    /* This function behaves exactly like buildGraph but does not add edges, 
    *  simply initializes actor objects and stores movie info. 
    */
    bool buildBlank(string actor_name, string movie_title, int movieyear);


    /* This function does a Breadthfirst search of the graph, going from 
    * actor1 to actor2. Used when looking for unweighted paths. Returns 
    * string containing path info. 
    */ 
    string Breadthfirst(string  actor_1, string actor_2, bool connecting);


    /* This function is used to find shortest path between two 
    * nodes with weighted edges. Returns the path output 
    */
    string Dijkstra(string  actor_1, string actor_2);



    /* This function adds the edges that come along from
    * an individual year of movies to the graph. Returns 
    * false if no movies stored in that year.
    */
    bool addYear(int year);


    /* This function reads pairfiles and sends them to be processed to the 
    * correct function based on the parameters given to it.
    */
    bool readPairs(const char* in_filename, const char* out_filename, bool weighted, bool connection);
 

    /* This function is called after all pairs are read and stored
    * in a graph data member vector so that it can add edges by year
    * and check all pairs for connections
    */
    void connectionFinder(const char* out_filename);


    /* This function abstracts finding an actor from the 
    * hash map given its name, which is used as the key 
    */
    ActorNode * findActor(string name);


/* Destroys and deinitializes */
    ~ActorGraph();

};


#endif // ACTORGRAPH_HPP