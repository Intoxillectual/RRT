#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#include<random>
using namespace std;


auto distance (pair < int, int >p1, pair < int, int >p2)     // Calculates distance between the locations
{
  return sqrt (pow (p2.first - p1.first, 2) + pow (p2.second - p1.first, 2));   // Distance formula
}

pair < int, int > getNearest (pair < double, double >newnode, vector < pair < double, double >>&node, int n)      // Returns the nearest node present in the graph
{
  int idx = 0;
  double min = 0;
  for (auto i = 0; i < n; ++i)
    {
      double tmpDist = distance (newnode, node[i]);        // Calculates the distance between the random node(node) and previous node
      if (tmpDist < min)                                // Comparison with the minimum distance and if the condition is false then minimum distance changes to the temporary distance Calculated
	{
	  min = tmpDist;
	  idx = i;
	}
    }
  return node[idx];   // Returning the nearest node
}


pair < double, double > randomConfig (double xbound, double ybound, const double xd, const double yd)   // Random configuration using mersenne twister for generating random node points
{
  random_device rd;
  mt19937 mtw (rd ());
  uniform_real_distribution < double >dist (xbound, xbound + xd);   
  auto n1 = dist (mtw);
  uniform_real_distribution <> dist1 (ybound, ybound + yd);
  auto n2 = dist1 (mtw);
  return make_pair (n1, n2);            // returns random node points(node); referrred this snippet from stack overflow
}


int main ()
{

  vector < pair < double, double >>node;             // creating vector for the tree node
  double xinit, yinit;
  cout << "Enter starting coordinates ";
  cin >> xinit >> yinit;                                // user defined starting point
  node.push_back (make_pair (xinit, yinit));              // adding the starting point to the tree node
  pair < double, double >goal;
  cout << "Enter goal coordinates ";
  cin >> goal.first >> goal.second;                     // user defined goal point

  int iter;
  cout << "Enter rrt iterations ";                      // user defined number of iterations
  cin >> iter;

  int nVert;
  cout << "Enter number of vertices ";                  // user defined maximum size of the vector for the tree node
  cin >> nVert;
 
  int count = 0;
  bool vertLimit = false;
  static double xdel = (goal.first - xinit) / nVert;
  static double ydel = (goal.second - yinit) / nVert;
  //cout << xdel << endl << ydel << endl;
  cout << "Iteration" << count + 1 << endl;
  while (count < iter)
    {
      auto newnode = randomConfig (node.back ().first, node.back ().second, xdel + 0.5, ydel + 0.5);   // Generating random nodes
      auto nearnode = getNearest (newnode, node, node.size ());             // Checking for the nearest node point
      cout << "Adding node to the tree node and the co-ordinates of the node are " << newnode.first << ", " << newnode.second << endl;
      if (round (newnode.first) == goal.first && round (newnode.second) == goal.second)
	    {
        	    cout << "**************************GOAL REACHED!**************************\n";
	            break;
	    }
      if (node.size () > nVert)
	    {
	            vertLimit = true;
	            break;
	    }
      node.push_back (newnode);
      ++count;
      cout << "Iteration" << count + 1 << endl;
    }
  if (vertLimit)
        cout << "node limit reached\n";
  else if (count == iter)
        cout << "Iteration limit reached, graph reached " << node[nVert].first<< " " << node[nVert].second << endl;
  cout << "Co-ordinates of the path are ->\n";
  for (auto i:node)
        cout << i.first << ", " << i.second << endl;

  return 0;
}
