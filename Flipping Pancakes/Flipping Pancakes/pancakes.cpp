// Flipping Pancakes
// Austin Andrews
// Visual Studio 2013
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

const string INPUT = "pancakes.in";
const string OUTPUT = "pancakes.out";

struct Node
{
	int identifier;
	string value;
	int Parent;
};

// www.algolist.net/Algorighms/Sorting/Quicksort
void quicksort(string& arr, int left, int right)
{
	int i = left, j = right;
	char tmp;
	char pivot = arr[(left + right) / 2];

	// partition
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	// recursion
	if (left < j)
		quicksort(arr, left, j);
	if (i < right)
		quicksort(arr, i, right);
}

// checks if the string is all uppercase and in order
bool Finished(string readline)
{
	string compare = readline;
	
	quicksort(compare, 0, compare.length()-1);

	for (unsigned int i = 0; i < compare.length(); i++)
		compare[i] = toupper(compare[i]);

	if (readline == compare)
		return true;

	return false;
}

// flips the value in the Node sent as a parameter and
// changes casing
string Flip(Node FlippedNode, int howManyToFlip)
{
	string result = FlippedNode.value.substr(0, FlippedNode.value.length() - howManyToFlip);
	string partial = FlippedNode.value.substr(FlippedNode.value.length() - howManyToFlip);

	for (int i = 0; i < howManyToFlip / 2; i++)
	{
		char temp = partial[i];
		partial[i] = partial[(howManyToFlip - 1) - i];
		partial[(howManyToFlip - 1) - i] = temp;
	}
	for (int i = 0; i < howManyToFlip; i++)
	{
		// if lowercase
		if (partial[i] >= 97 && partial[i] <= 122)
			partial[i] = toupper(partial[i]);
		else if (partial[i] >= 65 && partial[i] <= 90)
			partial[i] = tolower(partial[i]);
	}

	result.append(partial);

	return result;
}

int FindFullPath(Node FinishedNode, vector<Node> NodeVector, int numberOfFlips)
{
	Node PathNode = FinishedNode;
	if (FinishedNode.Parent != NULL)
	{
		for (int i = 0; i < NodeVector.size(); i++)
		{
			if (NodeVector.at(i).identifier == PathNode.Parent)
			{
				PathNode = NodeVector.at(i);
				numberOfFlips++;
				i = 0;
			}
		}
	}

	if (PathNode.Parent == NULL && PathNode.identifier != 0)
		numberOfFlips++;

	return numberOfFlips;
}

int FlipPancakes(Node FirstNode, int nodeCount)
{
	queue<Node> NodeQueue;
	vector<Node> NodeVector;
	int numberOfFlips = 0;

	NodeQueue.push(FirstNode);
	NodeVector.push_back(FirstNode);

	while (!NodeQueue.empty())
	{
		Node FrontNode = NodeQueue.front(); NodeQueue.pop();

		if (Finished(FrontNode.value))
		{
			while (!NodeQueue.empty()) NodeQueue.pop();
			numberOfFlips = FindFullPath(FrontNode, NodeVector, numberOfFlips);
		}
		else
		{
			// the # of possible flips has to do with the # of chars
			// in the FirstNode's value string
			for (int size = 1; size <= FirstNode.value.length(); size++)
			{
				Node NewNode = FrontNode;
				nodeCount++;
				NewNode.identifier = nodeCount;
				NewNode.Parent = FrontNode.identifier;
				NewNode.value = Flip(NewNode, size);

				NodeQueue.push(NewNode);
				NodeVector.push_back(NewNode);

				if (Finished(NewNode.value))
				{
					while (!NodeQueue.empty()) NodeQueue.pop();
					NodeQueue.push(NewNode);
					size = FirstNode.value.length();
				}
			}
		}
	}
	return numberOfFlips;
}

void main()
{
	ifstream fin(INPUT);
	if (fin)
	{
		ofstream fout(OUTPUT);
		string readline;

		fin >> readline;
		while (readline != "0")
		{
			int nodeCount = 0;

			Node FirstNode;
			FirstNode.identifier = nodeCount;
			FirstNode.Parent = NULL;
			FirstNode.value = readline;

			int numberOfFlips = FlipPancakes(FirstNode, nodeCount);

			cout << readline << " " << numberOfFlips << endl;
			fout << numberOfFlips << endl;

			fin >> readline;
		}

		fout.close();
		fin.close();
	}
	else
	{
		cout << "Input file not found." << endl;
	}
}