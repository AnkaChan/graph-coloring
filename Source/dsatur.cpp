#include "../Header/dsatur.h"

GraphColoring::Dsatur::Dsatur(map<string,vector<string> > input_graph): GraphColor(input_graph) {
	/* Do Nothing */
}

void GraphColoring::Dsatur::set_condition(int con) {
	/* Do Nothing, No Condition */
	cout << "DSATUR has no condition to set" << endl;
}

map<string,int> GraphColoring::Dsatur::color() {	

	vector<string> todo;
	string max_degree = "";
	int degree = -1;

	//find maximal degree vertex to color first and color with 0
	for(map< string, vector<string> >::iterator i = graph.begin(); i != graph.end(); i++) 
	{
		if((int)(*i).second.size() > degree) {
			degree = (*i).second.size();
			max_degree = (*i).first;
		}
	}
	if(max_degree == "")
	{
		cerr << "Error: Could not find a max degree node in the graph (reason unknown)" << endl;
	}
	coloring[max_degree] = 0;

	//Create saturation_level so that we can see which graph nodes have the
	//highest saturation without having to scan through the entire graph
	//each time
	map<string,int> saturation_level;

	//Add all nodes and set their saturation level to 0
	for(map<string,vector<string> >::iterator i = graph.begin(); i != graph.end(); i++)
	{
		saturation_level[(*i).first] = 0;
	}

	//For the single node that has been colored, increment its neighbors so
	//that their current saturation level is correct
	for(int i=0; i < graph[max_degree].size(); i++)
	{
		saturation_level[graph[max_degree][i]] += 1;
	}

	//Set the saturation level of the already completed node to -infinity so
	//that it is not chosen and recolored
	saturation_level[max_degree] = std::numeric_limits<int>::min();

	//Populate the todo list with the rest of the vertices that need to be colored
	for(map< string, vector<string> >::iterator i = graph.begin(); i != graph.end(); i++) 
	{
		if((*i).first != max_degree) {
			coloring[(*i).first] = -1;
			todo.push_back((*i).first);
		}
	}
	
	//Color all the remaining nodes in the todo list
	while(!todo.empty())
	{
		int saturation = -1;
		string saturation_name = "";
		vector<int> saturation_colors;
		//Find the vertex with the highest saturation level, since we keep the
		//saturation levels along the way we can do this in a single pass
		for(map<string,int>::iterator i = saturation_level.begin(); i != saturation_level.end(); i++)
		{
			//Find the highest saturated node and keep its name and neighbors colors
			if((*i).second > saturation)
			{
				saturation = (*i).second;
				saturation_name = (*i).first;

				//Since we're in this loop it means we've found a new most saturated
				//node, which means we need to clear the old list of neighbors colors
				//and replace it with the new highest saturated nodes neighbors colors
				//Since uncolored nodes are given a -1, we can add all neighbors and
				//start the check for lowest available color at greater than 0
				saturation_colors.clear();
				for(int j=0; j < graph[(*i).first].size(); j++)
				{
					saturation_colors.push_back(coloring[graph[(*i).first][j]]);
				}
			}
		}
		if(saturation_name == "")
		{
			cerr << "Error: Could not find a max saturated node in the graph (reason unknown)" << endl;
			coloring.clear();
			return coloring;
		}

		//We now know the most saturated node, so we remove it from the todo list
		todo.erase(std::find(todo.begin(), todo.end(), saturation_name));

		//Find the lowest color that is not being used by any of the most saturated
		//nodes neighbors, then color the most saturated node
		int lowest_color = 0;
		int done = 0;
		while(!done) 
		{
			done = 1;
			for(unsigned i=0; i<saturation_colors.size(); i++) 
			{
				if(saturation_colors[i] == lowest_color) 
				{
					lowest_color += 1;
					done = 0;
				}
			}
		}
		coloring[saturation_name] = lowest_color;

		//Since we have colored another node, that nodes neighbors have now
		//become more saturated, so we increase each ones saturation level
		//However we first check that that node has not already been colored
		//(This check is only necessary for enormeous test cases, but is
		//included here for robustness)
		for(int i=0; i < graph[saturation_name].size(); i++)
		{
			if(saturation_level[graph[saturation_name][i]] != std::numeric_limits<int>::min())
			{
				saturation_level[graph[saturation_name][i]] += 1;
			}
		}
		saturation_level[saturation_name] = std::numeric_limits<int>::min();
	}
	return coloring;
}

