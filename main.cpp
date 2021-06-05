#include "Graph.h"

struct Web_Graph_Edge
{
	string source_web;
	string destination_web;
};

//Listing the websites (add an index to each unique website)
map<int, string> indexing_websites_function(string file_name)
{
	ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}

	vector<string>websites_list;
	string line, website;
	while (!file.eof())
	{
		getline(file, line);
		stringstream ss(line);
		while (getline(ss, website, ','))
			websites_list.push_back(website);

	}
	auto end = websites_list.end();
	for (auto it = websites_list.begin(); it != end; ++it) {
		end = remove(it + 1, end, *it);
	}

	websites_list.erase(end, websites_list.end());

	map<int, string> indexing_websites;
	for (unsigned int i = 0; i < websites_list.size(); i++)
	{
		indexing_websites[i] = websites_list[i];
	}
	return indexing_websites;
}



Graph web_graph( int x)
{
	map<int, string>indexing_websites_graph = indexing_websites_function("web_graph.csv");
	Graph web_graph(indexing_websites_graph.size());
	ifstream web_graph_file("web_graph.csv");
	if (!web_graph_file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	Web_Graph_Edge current_edge;
	while (web_graph_file.good())
	{
		getline(web_graph_file, current_edge.source_web, ',');
		getline(web_graph_file, current_edge.destination_web, '\n');
		int from_index, to_index;
		for (map<int, string>::iterator it = indexing_websites_graph.begin(); it != indexing_websites_graph.end(); it++)
		{

			if (it->second == current_edge.source_web)
				from_index = it->first;
			if (it->second == current_edge.destination_web)
				to_index = it->first;
		}
		web_graph.addEdge(from_index, to_index);
	}
	if (x==1)
		return web_graph;
	if (x==-1)
		return web_graph.get_transpose();

}

vector<double> PageRank_algorithem()
{

	Graph TransposedWebGraph = web_graph(-1);
	Graph WebGraph = web_graph(1);
	vector<int>degreeIn = TransposedWebGraph.Degree();
	vector<int>degreeOut = WebGraph.Degree();
	vector<double>impressions_vector(degreeIn.size(), 0);
	map<int, string>indexing_websites_graph = indexing_websites_function("web_graph.csv");


	int** adj = TransposedWebGraph.display_adj(); 
	
	
// intializing pageranks of all websites by 1/n

	double LowestRank =1;
	double HighestRank = 0;
	double sum = 0;
	vector <double>PageRank(degreeIn.size(), ((double)1 / WebGraph.getV()));

	vector <double>PrevPageRAnk(degreeIn.size(), ((double)1 / WebGraph.getV()));

	vector <double>NormPageRAnk(degreeIn.size());


	int flag = 0;
	for (int i = 1; i < 300; ++i)
	{
		
		for (int j = 0; j < degreeOut.size(); j++) 
		{
			sum = 0;
			flag = 0;

			for (int k = 0; k < degreeIn[j]; k++) 
			{
				flag = 1;
					PageRank[j] = (PrevPageRAnk[adj[j][k]]/degreeOut[adj[j][k]]);

				sum = sum + PageRank[j];
			}

			if (flag == 0)
				PageRank[j] = 0;

			if (sum < LowestRank)
				LowestRank = sum; 
			if (sum > HighestRank)
				HighestRank = sum;
			PageRank[j] = sum; 
		}
		
		PrevPageRAnk = PageRank; 

	}

	for (int i = 0; i < PageRank.size(); i++)
	{
		
		NormPageRAnk[i] = (PageRank[i]-LowestRank) / (HighestRank-LowestRank);
	}


	return NormPageRAnk;

}


//Creating a map of keywords
map<string, vector<string>> KeywordsMap(string file_name)
{
	ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	map<string, vector<string>> keywords_map;
	string line, website, keyword;
	vector<string>keywords;
	while (!file.eof())
	{
		getline(file, line);
		stringstream ss(line);
		getline(ss, website, ',');
		while (getline(ss, keyword, ','))
			keywords.push_back(keyword);
		keywords_map[website] = keywords;
		keywords.clear();
	}

	return keywords_map;
}

//Making a new search
vector<string> New_Search(string search_query)
{
	vector<string>results;
	vector<string>search_query_as_words;
	map<string, vector<string>>data = KeywordsMap("keywords.csv");
	stringstream ss(search_query);
	string word;
	while (getline(ss, word, ' '))
		search_query_as_words.push_back(word);
	if (search_query_as_words.size() == 1)
	{
		if (*search_query_as_words[0].begin() == '"')
		{
			search_query_as_words[0] = search_query_as_words[0].substr(1);
			search_query_as_words[0].pop_back();
			for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
			{
				for (unsigned int j = 0; j < it->second.size(); j++)
				{
					if (search_query_as_words[0] == it->second[j])
						results.push_back(it->first);
				}
			}

		}
		else
		{
			for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
			{
				transform(search_query_as_words[0].begin(), search_query_as_words[0].end(), search_query_as_words[0].begin(), ::tolower);
				for (unsigned int j = 0; j < it->second.size(); j++)
				{
					if (search_query_as_words[0] == it->second[j])
						results.push_back(it->first);
				}
			}
		}

	}
	else if (search_query_as_words[1] == "AND")
	{
		for (unsigned int i = 0; i < search_query_as_words.size(); i++)
		{
			transform(search_query_as_words[i].begin(), search_query_as_words[i].end(), search_query_as_words[i].begin(), ::tolower);
		}
		bool flag1 = 0, flag2 = 0;
		string keyword1 = search_query_as_words[0];
		string keyword2 = search_query_as_words[2];
		for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
		{
			for (unsigned int j = 0; j < it->second.size(); j++)
			{
				if (keyword1 == it->second[j])
				{
					flag1 = 1;
				}

				if (keyword2 == it->second[j])
				{
					flag2 = 1;
				}

			}
			if (flag1 && flag2)
			{
				results.push_back(it->first);
			}
			flag1 = 0;
			flag2 = 0;
		}
	}
	else if (search_query_as_words[1] == "OR")
	{
		for (unsigned int i = 0; i < search_query_as_words.size(); i++)
		{
			transform(search_query_as_words[i].begin(), search_query_as_words[i].end(), search_query_as_words[i].begin(), ::tolower);
		}
		bool flag1 = 0, flag2 = 0;
		string keyword1 = search_query_as_words[0];
		string keyword2 = search_query_as_words[2];
		for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
		{
			for (unsigned int j = 0; j < it->second.size(); j++)
			{
				if (keyword1 == it->second[j])
				{
					flag1 = 1;
				}

				if (keyword2 == it->second[j])
				{
					flag2 = 1;
				}

			}
			if (flag1 || flag2)
			{
				results.push_back(it->first);
			}
			flag1 = 0;
			flag2 = 0;
		}
	}
	else if (*search_query_as_words[0].begin() == '"')
	{
		search_query_as_words[0] = search_query_as_words[0].substr(1);
		search_query_as_words[1].pop_back();
		string keyword = search_query_as_words[0] + ' ' + search_query_as_words[1];
		for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
		{
			for (unsigned int j = 0; j < it->second.size(); j++)
			{
				if (keyword == it->second[j])
					results.push_back(it->first);
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < search_query_as_words.size(); i++)
		{
			transform(search_query_as_words[i].begin(), search_query_as_words[i].end(), search_query_as_words[i].begin(), ::tolower);
		}
		for (unsigned int i = 0; i < search_query_as_words.size(); i++)
		{
			for (map<string, vector<string>>::iterator it = data.begin(); it != data.end(); it++)
			{
				for (unsigned int j = 0; j < it->second.size(); j++)
				{
					if (search_query_as_words[i] == it->second[j])
						results.push_back(it->first);
				}
			}
		}
	}



	auto end = results.end();
	for (auto it = results.begin(); it != end; ++it)
	{
		end = remove(it + 1, end, *it);
	}

	results.erase(end, results.end());


	return results;
}


vector<string> UpdateScore(vector<double> PRnorm, vector<string> results)
{
	Graph WebGraph = web_graph(1);

	map<int, string>indexing_websites_graph = indexing_websites_function("web_graph.csv");

	vector<double>impressions(WebGraph.getV(), 0);
	vector<double>NoOfClicks(WebGraph.getV(), 0);
	vector<double>CTR(WebGraph.getV(), 0);
	vector<double>Score(WebGraph.getV(), 0);

	vector<string> RankedResults;

	// Read impressions //
	ifstream impressions_file("impressions.csv");
	if (!impressions_file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	string url, Imp;
	while (impressions_file.good())
	{
		getline(impressions_file, url, ',');
		getline(impressions_file, Imp, '\n');
		for (map<int, string>::iterator it = indexing_websites_graph.begin(); it != indexing_websites_graph.end(); it++)
		{

			if (it->second == url) impressions[it->first] = stoi(Imp);
		}
	}

	// Read Number of clicks //

	ifstream NoOfClicks_file("NumberOfClicks.csv");
	if (!NoOfClicks_file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	string URL, NumofClicks;
	while (NoOfClicks_file.good())
	{
		getline(NoOfClicks_file, URL, ',');
		getline(NoOfClicks_file, NumofClicks, '\n');
		for (map<int, string>::iterator it = indexing_websites_graph.begin(); it != indexing_websites_graph.end(); it++)
		{

			if (it->second == URL) NoOfClicks[it->first] = stoi(NumofClicks);
		}
	}

	// Calculating the CTR //
	for (int i = 0; i < WebGraph.getV(); i++)
	{

		CTR[i] = NoOfClicks[i] / impressions[i];
	}

	// 	Calculating the scores //

	for (int i = 0; i < WebGraph.getV(); i++)
	{
		Score[i] = (0.4 * PRnorm[i]) + (((1 - (0.1 * impressions[i]) / (1 + 0.1 * impressions[i])) * PRnorm[i] + ((0.1 * impressions[i]) / (1 + 0.1 * impressions[i])) * CTR[i]) * 0.6);
	}


	set <double, greater <double> > OrderedScores;
	set<double>::iterator it;


	for (int i = 0; i < indexing_websites_graph.size(); i++)
	{
		for (int j = 0; j < results.size(); j++)
		{
			if (indexing_websites_graph[i] == results[j])
				OrderedScores.insert(Score[i]);

		}
		

	}

	
		for (int j = 0; j < OrderedScores.size()+1; j++)
		{
				for (int i = 0; i < Score.size(); i++)
				{
					if ( !OrderedScores.empty() && OrderedScores.find(Score[i]) == OrderedScores.begin())
					{
						RankedResults.push_back(indexing_websites_graph[i]);
						OrderedScores.erase(OrderedScores.begin());
					}

				}
	
		
		}
	
		
	return RankedResults;
}

// update the impressions 

void UpdateImpr(vector<string> WebsitesNeedtoGetupdated)
{
	map<int, string>indexing_websites_graph = indexing_websites_function("web_graph.csv");

	vector<double>impressions(indexing_websites_graph.size(), 0);

	ifstream impressions_file("impressions.csv");
	if (!impressions_file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	string url, Imp;
	while (impressions_file.good())
	{
		getline(impressions_file, url, ',');
		getline(impressions_file, Imp, '\n');
		for (map<int, string>::iterator it = indexing_websites_graph.begin(); it != indexing_websites_graph.end(); it++)
		{

			if (it->second == url) impressions[it->first] = stoi(Imp);
		}
	}

	impressions_file.close();

	ofstream output;
	output.open("newimpressions.csv");

	int flag = false;

	for (int i = 0; i < indexing_websites_graph.size(); i++)
	{
		flag = false;
		for (int j = 0; j < WebsitesNeedtoGetupdated.size(); j++)
		{
			
			if (WebsitesNeedtoGetupdated[j] == indexing_websites_graph[i])
			{
				output << indexing_websites_graph[i] << "," << ++impressions[i] << "\n";
				flag = true;
			}
					
		}

		if (!flag)
			output << indexing_websites_graph[i] << "," << impressions[i] << "\n";
		
	} 

	output.close();

	remove("impressions.csv");
	rename("newimpressions.csv", "impressions.csv");

}

// update the No. of Clicks

void UpdateNoOfClicks(string WebsiteClicked)
{
	map<int, string>indexing_websites_graph = indexing_websites_function("web_graph.csv");

	vector<double>NoOfClicks(indexing_websites_graph.size(), 0);

	ifstream NoOfClicks_file("NumberOfClicks.csv");
	if (!NoOfClicks_file.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}
	string URL, NumofClicks;
	while (NoOfClicks_file.good())
	{
		getline(NoOfClicks_file, URL, ',');
		getline(NoOfClicks_file, NumofClicks, '\n');
		for (map<int, string>::iterator it = indexing_websites_graph.begin(); it != indexing_websites_graph.end(); it++)
		{

			if (it->second == URL) NoOfClicks[it->first] = stoi(NumofClicks);
		}
	};

	NoOfClicks_file.close();

	ofstream output;

	output.open("newNoOfClicks.csv");


	for (int i = 0; i < indexing_websites_graph.size(); i++)
	{
		
			if (WebsiteClicked == indexing_websites_graph[i])
				output << indexing_websites_graph[i] << "," << NoOfClicks[i]+1 << "\n";
			else 
				output << indexing_websites_graph[i] << "," << NoOfClicks[i] << "\n";

	}

	output.close();

	remove("NumberOfClicks.csv");
	rename("newNoOfClicks.csv", "NumberOfClicks.csv");

}



//Display search results
void Display()
{
	string search_query;
	getline(cin, search_query);
	vector<string>results = New_Search(search_query);
	vector <double> NormPageRanks = PageRank_algorithem();
	vector<string> RankedResults = UpdateScore(NormPageRanks, results);
	

	cout << " \nSearch results: \n";
	if (RankedResults.size() == 0)
		cout << "No results found" << endl;
	for (unsigned int i = 0; i < RankedResults.size(); i++)
	{
		cout << i + 1 << ".   " << RankedResults[i] << endl;
	}
	cout << endl;
	// update the impressions file //
	UpdateImpr(RankedResults);

	cout << " Would you like to\n1.   Choose a wepage to open\n2.   New search\n3.   Exit\n" << endl;
	cout << " Type in your choice: ";
	int choice, n;
	cin >> choice;
	cin.ignore();
	while (choice == 2)
	{

		Display();
	}

	switch (choice)
	{
	case 1:
		cout << " Enter the number of webpage that you want to open\n\n Type in your choice: ";
		cin >> n;
		cin.ignore();
		cout << " You're now viewing " << RankedResults[n - 1] << ".\n Would you like to\n1.   Back to search results\n2.   New Search\n3.   Exit\n\n";

		// update the NumberOfCLicks file //
		UpdateNoOfClicks(RankedResults[n - 1]);

		cout << " Type in your choice: ";
		cin >> n;
		if (n == 1)
		{
			cout << " \nSearch results: \n";
			if (RankedResults.size() == 0)
				cout << "No results found" << endl;
			for (unsigned int i = 0; i < RankedResults.size(); i++)
			{
				cout << i + 1 << ".   " << RankedResults[i] << endl;
			}
			cout << endl;
		};
		cin.ignore();
		while (n == 2)
		{
			Display();
		}
		if (n == 3) exit(1);
		break;
	case 3:
		exit(1);
	}
}



int main()
{
	//the main of the project

	cout << " Welcome!\n What would you like to do?\n1.   New Search\n2.   Exit\n\n Type in your choice: ";
	int choice;
	cin >> choice;
	cin.ignore();
	switch (choice)
	{
	case 1:
		Display();
		break;
	case 2:
		exit(1);
		break;


	}
	return 0;
}