/*
 * ActorGraph.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/02/2016>
 *
 * This file holds the ActorGraph class and its functions. The class implements
 * a graph of ActorNode vertices and ActorEdge edges, along with methods to 
 * traverse the graph.
 */
 
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph(void) 
{
    earliest = 2015;
}



/* This function reads from the input file and calls the appropriate
 * graph building function depending on the parameter passed to it 
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges, bool blank) {

    // Initialize the file stream

    ifstream infile(in_filename);
    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // adding year to movie title
		movie_title+="#@";
		movie_title+=record[2];

        // earliest is used for actorconnections
        if (movie_year < earliest)
        {
            earliest = movie_year;
        }



        // If we want a graph with no edges
        if (blank)
        {
            buildBlank(actor_name, movie_title, movie_year);
        }

        // Regular graph
        else 
        {
            buildGraph(actor_name, movie_title);
        }

    }
	
    // Error checking	
    if (!infile.eof())
    {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }


    return true;
}



 /* This function adds an actor to a movie's cast, creating the actor
 *   and movie if either do not exist, otherwise gets the movies cast
 *   and adds an edge from each of them to the new actor. Called iteratively
 *   from LoadfromFile and addYear.
 */
bool ActorGraph::buildGraph(string actor_name, string movie_title)
{

    // Checking if actor node already exists, making it if it doesnt.
    ActorNode * currentActor = findActor(actor_name);

    // Case where its a new actor
    if (!currentActor)
    {

        currentActor = new ActorNode(actor_name);
        allNodes.push_back(currentActor);
        auto act = actors.insert(make_pair(actor_name, currentActor));
    }

        // uses movie title as key to hashmap storing its cast. Try to add currentactor to the cast vector
    auto mov = movies.insert(make_pair(movie_title, vector<ActorNode *>(1, currentActor)));

    // If movie already exists 
    if (!mov.second)
    {
    // When movie already exists we add an edge connecting current actor and all others starring in this movie
        vector <ActorNode *> costar = mov.first->second;
        int counter = 0;

            // Add edges to each star since the movies table's second value is its cast
            for (auto addcostars = costar.begin(); addcostars != costar.end(); ++addcostars)
            {
                ActorNode * current_costar = costar[counter];
                ActorEdge * newEdge = new ActorEdge(current_costar, currentActor, movie_title);
                allEdges.push_back(newEdge);
                current_costar->addEdge(newEdge);
                currentActor->addEdge(newEdge);
                counter++;
                
            }

            // Add the current actor to the movie's cast
            mov.first->second.push_back(currentActor);

        }

        // This vector is mostly for the extension
        currentActor->addMovie(movie_title);

}

/* This function behaves exactly like buildGraph but does not add edges, 
 * simply initializes actor objects and stores movie info. 
 */
bool ActorGraph::buildBlank(string actor_name, string movie_title, int movieyear)
{
        // Checking if actor node already exists, making it if it doesnt.
        ActorNode * currentActor = findActor(actor_name);

        // Case where its a new actor
        if (!currentActor)
        {

            currentActor = new ActorNode(actor_name);
            allNodes.push_back(currentActor);
            auto act = actors.insert(make_pair(actor_name, currentActor));
        }

        /* Storing an actors earliest film can help skip searches when
         * doing actorconnections */
        if (movieyear < currentActor->earliestfilm)
        {
            currentActor->earliestfilm = movieyear;
        }
       auto mov = movies.insert(make_pair(movie_title, vector<ActorNode *>(1, currentActor)));

       // If movie already exists 
        if (!mov.second)
        {
            mov.first->second.push_back(currentActor);
        }

        else 
        {
            /* Movies sorted by year. Only inserted when we know its 
             * a new movie */
            auto sortedmov = sortedmovies.insert(make_pair(movieyear, vector<string>(1, movie_title)));
            if (!sortedmov.second)
            {
                sortedmov.first->second.push_back(movie_title);
            }

        }





}


/* This function does a Breadthfirst search of the graph, going from 
 * actor1 to actor2. Used when looking for unweighted paths. Returns 
 * string containing path info. 
 */
string ActorGraph::Breadthfirst(string  actor_1, string actor_2, bool connecting)
{   
    // Reset values
    for (auto resetter = actors.begin(); resetter != actors.end(); ++resetter)
    {
        (*resetter).second->dist = 999;
        (*resetter).second->prevEdge = 0;
        
    }

    // Passed strings which we lookup in hashmap 
    ActorNode * actor1 = findActor(actor_1);
    ActorNode * actor2 = findActor(actor_2);

    // Structures to begin BFS search
    queue<ActorNode *> toExplore;
    ActorNode * start;
    start = actor1;
    start->dist = 0;
    
    toExplore.push(start);

    // This loop contains the BFS algorithm
    while (!toExplore.empty() )
    {
        ActorNode * next = toExplore.front();
        toExplore.pop();
        vector<ActorEdge *>::iterator it = next->edges.begin();
        for (; it != next->edges.end(); ++it)
        {
             ActorNode * neighbor = (*it)->crossEdge(next);
             if (next->dist+1 < neighbor->dist)
             {
                neighbor->dist = next->dist +1;
                neighbor->prevEdge = *it;
                toExplore.push(neighbor);
             }
        }
    }

    // Going backwards to get the path
    ActorNode * hi = actor2;
    stack<string> path;
    string output;

    // If theres no connection
    if (!(hi->prevEdge))
    {
        return ERROR;

    }

    /* If doing actor connections dont need path.
     * when a string that isnt ERROR is passed, the
     * connections function recognizes a connection 
     * has been made */
    else if (connecting)
    {
        return "0";
    }

    /* Going backwards along path but using
     * stack to get right path
     */
    while (hi != actor1)
    {
        path.push(hi->getName());
        path.push(hi->prevEdge->getMovie());
        hi = hi->prevEdge->crossEdge(hi);



    } 
    path.push(hi->getName());

    // Formats output 
    while (!path.empty())
    {
        output+= "(";
        output+= path.top();
        if (path.top() == actor2->getName()){
            output+= ")";
            break;
        }
        path.pop();
        output+=")--[";
        output+=path.top();
        path.pop();
        output+="]-->";

    } 

    output += "\n";

    return output;

}

/* This structure allows us to compare ActorNodes for the minheap
 * used in Dijkstra function
 */
struct CompareWeight: public std::binary_function<ActorNode*, ActorNode*, bool>        
{
   bool operator()(const ActorNode * a1, const ActorNode * a2)
    {   
        return a1->dist > a2->dist;
    }
};                                                                                    


/* This function is used to find shortest path between two 
 * nodes with weighted edges. Returns the path output 
 */
string ActorGraph::Dijkstra(string  actor_1, string actor_2)
{
    // Reset values
    for (auto resetter = actors.begin(); resetter != actors.end(); ++resetter)
    {
        (*resetter).second->dist = 999;
        (*resetter).second->done = false;
        (*resetter).second->prevEdge = 0;
    }

    // Get actornodes from names using hashmap
    ActorNode * actor1 = findActor(actor_1);
    ActorNode * actor2 = findActor(actor_2);

    // Structures for dijkstra's algorithm. 
    priority_queue<ActorNode *, vector<ActorNode *>, CompareWeight> toExplore;
    ActorNode * start;
    start = actor1;
    start->dist = 0;
    toExplore.push(start);

    // This loop contains the dijkstra algorithm
    while (!toExplore.empty())
    {
        ActorNode * next = toExplore.top();
        toExplore.pop();
        if (!next->done)
        {
            next->done = true;
            vector<ActorEdge *>::iterator it = next->edges.begin();

            for (; it != next->edges.end(); ++it)
            {
                ActorNode * neighbor = (*it)->crossEdge(next);
                int neighbordist = next->dist + ((*it)->getWeight());
              
                if (neighbordist < neighbor->dist)
                {
                    neighbor->dist = neighbordist;
                    neighbor->prevEdge = *it;
                    toExplore.push(neighbor);
                }

            }
        }

     }

    // Structures to get path
    ActorNode * hi = actor2;
    stack<string> path;
    string output;

    // If no prevEdge at actor2 then no connection
    if (!(hi->prevEdge))
    {
        return ERROR;

    }
    
    // Go along path backwards using a stack to make forwards
    while (hi != actor1)
    {
        path.push(hi->getName());
        path.push(hi->prevEdge->getMovie());
        hi = hi->prevEdge->crossEdge(hi);

    } 
    path.push(hi->getName());

    // Formats output
    while (!path.empty())
    {
        output+= "(";
        output+= path.top();
        if (path.top() == actor2->getName()){
            output+= ")";
            break;
        }
        path.pop();
        output+=")--[";
        output+=path.top();
        path.pop();
        output+="]-->";

    }
        
    output += "\n";

    return output;


}

/* This function adds the edges that come along from
 * an individual year of movies to the graph. Returns 
 * false if no movies stored in that year.
 */
bool ActorGraph::addYear(int year)
{
    auto movs = sortedmovies.find(year);

    // If no movies with that year
    if (movs==sortedmovies.end())
    {
        return false;
    }


    vector<string>movie_titles = movs->second;

    // Uses buildGraph to add edges from movies released that year
    for (int i = 0; i < movie_titles.size(); i++)
    {
        auto moviestar = movies.find(movie_titles[i]);
        vector <ActorNode * > stars = moviestar->second; 
        for (int j = 0; j < stars.size(); j++)
        {
            string actorname = stars[j]->getName();
            buildGraph(actorname, movie_titles[i]);
        }


    }


}


/* This function reads pairfiles and sends them to be processed to the 
 * correct function based on the parameters given to it.
 */
bool ActorGraph::readPairs(const char* in_filename, const char* out_filename, bool weighted, bool connection)
{
    ifstream infile(in_filename);
    ofstream outfile(out_filename);
    bool have_header = false;

    // Header for paths
    if (!connection)
    {

       outfile << "(actor)--[movie#@year]-->(actor)--..." << '\n';
    }

  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        string output;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        string from_actor(record[0]);
        string to_actor(record[1]);

        /* If doing connections stores all pairs in one vector
         * so that each can be checked for connection as years 
         * are added to the graph */
        if (connection)
        {
            Actorpairs.push_back(from_actor);
            Actorpairs.push_back(to_actor);
        }

        // If weighted we call dijkstra function and write to file
        else if (weighted)
        {
            outfile << Dijkstra(from_actor, to_actor);  
        }

        // If unweighted we call BFS func and write to file
        else
        {
           outfile <<  Breadthfirst(from_actor, to_actor, false);  

        }



    }

    // Error checking
    if (!infile.eof()) {
         cerr << "Failed to read " << in_filename << "!\n";
          return false;
    }

    return true;


}


/* This function is called after all pairs are read and stored
 * in a graph data member vector so that it can add edges by year
 * and check all pairs for connections
 */
void ActorGraph::connectionFinder(const char* out_filename)
{   
    // Start at earliest year 
    int currentyear = earliest;

    /* Stores the year a connection is first made for a pair
     * in corresponding location of its vector */
    vector<int> years(Actorpairs.size(), 0);

    string searchresult;

    /* Stores the earliest possible year of connection for 
     * each pair, which is the first year both actors have an edge
     */
    vector<int>mins(Actorpairs.size(), 0);

    // This loop adds each year and checks for connections
    while (currentyear != 2015)
    { 
        addYear(currentyear);
        for (int i = 0; i < Actorpairs.size(); i++)
        {
            // Since pairs we only check evens
            if (i % 2 == 0)
            {
                /* First run of loop initializes each pair to see
                * earliest year connection is possible to avoid 
                * unnecessary searches.
                */ 
                if (!mins[i])
                {
                    
                    ActorNode * a1 = findActor(Actorpairs[i]);
                    ActorNode * a2 = findActor(Actorpairs[i+1]);
                    mins[i] = max(a1->earliestfilm, a2->earliestfilm);
                }

                // We only search if not yet found and above min year
                if (!years[i] && (currentyear >= mins[i]) )
                {
                    searchresult = Breadthfirst(Actorpairs[i], Actorpairs[i+1], false);
                    // BFS returns error when no connection

                    if (searchresult != ERROR)
                    {
                      years[i] = currentyear;
                      years[i+1] = currentyear;
                    }

                }
            }

        }
        // Check if all years have been found 
        auto it = find (years.begin(), years.end(), 0);
        if (it == years.end())
        {
            break;

        }
        currentyear++;


    }

    // Writes connection data to file
    ofstream outfile(out_filename);
    outfile  << "Actor1\tActor2\tYear\n";

    for (int i = 0; i < Actorpairs.size(); i++)
    {
        // Each even member of the vector is first member of the pair
        if ( i%2 == 0)
        {
          outfile << Actorpairs[i] << "\t" << Actorpairs[i+1] << "\t" << years[i] << endl;
        }

    }

    
}

/* This function abstracts finding an actor from the 
 * hash map given its name, which is used as the key 
 */
ActorNode * ActorGraph::findActor(string name)
{
     auto actor_find = actors.find (name);

     if (actor_find == actors.end())
     {
        return 0;
     }
     else
     {
         return actor_find->second;
     }


}

/* Destroys and deinitializes */
ActorGraph::~ActorGraph()
{
    for (int i = 0; i < allNodes.size(); i++)
    {
        delete allNodes[i];
    }
 
    for (int i = 0; i < allEdges.size(); i++)
    {
        delete allEdges[i];
    }

    actors.clear();
    movies.clear();
    sortedmovies.clear();
    allNodes.clear();
    allEdges.clear();


}
