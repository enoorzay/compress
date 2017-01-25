# actor-separation 
*pathfinder* 
Usage:  ./pathfinder <casting data file> <w or u> <pair file> <output file>

Casting file and pair file should match the format of the sample files. 

This program takes in two actors and finds their shortest path to each other via actors they've costarred with. Based off the
six degrees of Kevin Bacon game. 

Operates by utilizing the concepts of graph theory, with nodes representing actors and edges representing movies.


*extension* 
The extension is called by ./extension [movie cast tsv] and loads a graph from the file.
It then takes user input to find actors in a user-friendly way, and formats it to be used to search the 
graph and finds the nodes made from the movie cast file if they exist. 

Once two actors are found several operations are possible, at the choice of the user. 

1) 
A list of movies either (or both) actors have starred in may be viewed 
(which is made by a vector data member of the ActorNode class that has 
movie titles pushed to it everytime a new movie starring them is found.) 

2) 
The shortest path between them can be calculated 

3) 
The average distance to either actor can be calculated. (This is done by doing
a Breadth first traversal for that actor then summing the distances of all actors
connected to them and dividing by the total number of actors connected.) 

4) 
Exit
