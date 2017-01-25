/*
 * extension.cpp
 * Author: <Edrees Noorzay>
 * Date:   <06/03/16>
 *
 * This file takes a movie cast file and creates an Actorgraph from it.
 * It then takes a user input of 2 actor names, and finds the actors from
 * the graph. It allows the user to see different characteristics of that
 * actor node.
 */


#include "ActorGraph.hpp"
using namespace std;

	

int main(int argc, char* argv[])
{

	// check args
	if(argc != 2){
		cout << "Invalid number of arguments.\n"
		     << "Usage: ./extension <casting data file>";
		return -1;

	}


	ActorGraph mygraph = ActorGraph();

	bool loaded;
	cout << "Loading...\n";
	loaded = mygraph.loadFromFile(argv[1], false, false);

	if (loaded)
	{
		cout << "Done!" << '\n';
	}
	else
	{
		cout << "FAILURE;" << '\n';
	}

	// Holds first name
	string actorfirstname;

	// Holds Last name then has first name added to it
	string actorname;

	ActorNode * actor1;
	ActorNode * actor2;

	// If user is reusing actors
	bool reusing = false;

	// This is the main loop for handling user input
	while (1)
	{

		// This block takes two actor names from user and finds their nodes
		if (!reusing)
		{
			int run = 0;
			while (run < 2)
			{
				cout << "Please insert an actor name in the following format: \n";
		 		cout << "First name: ";
	 			getline(cin, actorfirstname);
	 			cout << "\n"<< "Last name: ";
	 			getline(cin, actorname);

	 			// Need upper case
				transform(actorname.begin(), actorname.end(), actorname.begin(), ::toupper);
				transform(actorfirstname.begin(), actorfirstname.end(), actorfirstname.begin(), ::toupper);

				// Make into 1 string
			 	actorname+= ", " + actorfirstname;
				ActorNode * actor = mygraph.findActor(actorname);


				// This block checks if actor exists with suffix if name not found
				if (!actor)
				{
					actor = mygraph.findActor(actorname + " (I)");

					if (!actor)
					{
						actor = mygraph.findActor(actorname + " (II)");

						if (!actor)
						{
							actor = mygraph.findActor(actorname + " (III)");
							if (!actor)
							{
								cout << "Actor not found. " << endl;
							}

						}
					}


				}

				// If actor found set appropriate pointer
				if (actor)
				{

					if (run == 0)
					{
						actor1 = actor;

					}

					else if (run == 1)
					{
						actor2 = actor;
					}

					cout << "FOUND " << actor->getName() << " who starred in: " <<
					actor->movies[0] << endl;

					run++;

				}

			}
		}

		// If both actors found then can operate on them
		if (actor1 && actor2)
		{


			cout << "\nPlease make a selection: " << endl;
			cout << "(F)ind shortest path to other actor, "
				 << "(A)verage distance to actor, "
				 << "(L)ist all movies starred in, or "
				 << "(E)xit." << endl;
			string selection;
			cin >> selection;

			// Just want first letter, uppercased
			char digit = selection[0];
			digit = toupper(digit);
			int input;

			// This block handles carrying out user-ordered functions
			switch(digit)
			{

				// Exit case
				case 'E':
					cout << "Exiting." << endl;
					return 1;

				// List movies case
				case 'L':
					cout << "Would you like to list the movies of \n"
						 << "(1) " << actor1->getName() <<"\n(2) "
						 << actor2->getName() << "\n(3) both: \n";
					cin >> input;
					if (input == 1)
					{
						cout << actor1->getName() << endl;
						cout << actor1->listMovies() << endl;
						break;
					}
					if (input == 2)
					{
						cout << actor2->getName() << endl;
						cout << actor2->listMovies() << endl;
						break;
					}
					if (input == 3)
					{
						cout << actor1->getName() << endl;
						cout << actor1->listMovies() << endl;
						cout << actor2->getName() << endl;
						cout << actor2->listMovies() << endl;
						break;

					}

				// Shortest path case, uses BFS
				case 'F':
					cout << "Shortest path " << endl;
					cout << mygraph.Breadthfirst(actor1->getName(), actor2->getName(), false);
					break;

				// Finds average distance to node
				case 'A':

					// Which actor to find avg distance to
					cout << "Would you like to compute average of \n"
						 << "(1) " << actor1->getName() <<"\n(2) "
						 << actor2->getName() << "\n";
					cin >> input;


					ActorNode * computingNode;
					if (input == 1)
					{
						computingNode = actor1;

					}
					if (input == 2)
					{
						computingNode = actor2;
					}
					string computename = computingNode->getName();

					// Holds sum of all distances
					double sum = 0;

					// Run BFS once so all distances are set
					mygraph.Breadthfirst(computename, actor2->getName(), true);

					// Total number of nodes connected to it
					double total = 0;

					// This loop finds the total # of nodes and sum of distances to actor
					for (int i = 0; i < mygraph.allNodes.size(); i++)
					{
						// If connected
						if (mygraph.allNodes[i]->prevEdge)
						{
							// Add distance to sum and increment total
							sum += mygraph.allNodes[i]->dist;
							total++;

							// Dont increment total for self
							if (mygraph.allNodes[i]->dist == 0)
							{
							 	total--;
							}
						}


					}

					double avg = (sum/total);
					cout << "Average distance to: " << computename << " is "
						 << avg << endl;
					break;



			}

		}



		// This block allows looping and repeat use of actors
		string repeat;
		cout << " \nWould you like to try again? (Y/N)" << endl;
		cin >> repeat;

		if (repeat == "N" || repeat == "n")
		{
			return 1;
		}

		else
		{
			cout << "Would you like to use the same actors? (Y/N)" << endl;
			cin >> repeat;
			if (repeat == "Y" || repeat == "y")
			{
				reusing = true;
			}
			else
			{
				reusing = false;
			}
		}


	}


}
