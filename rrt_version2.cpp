#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#include<random>
using namespace std;


struct linObstacle          // Coordinates of linear obstacles
{
  double x1, x2;
  double y1, y2;
};


auto Distance (pair < int, int >p1, pair < int, int >p2)     // Calculates distance between the locations
{
  return sqrt (pow (p2.first - p1.first, 2) + pow (p2.second - p1.first, 2));   // Distance formula
}

pair < int, int > getNearest (pair < double, double >newnode, vector < pair < double, double >>&node, int n)      // Returns the nearest node present in the graph
{
  int idx = 0;
  double min = 0;
  for (auto i = 0; i < n; ++i)
    {
      double tmpDist = Distance (newnode, node[i]);        // Calculates the distance between the random node(node) and previous node
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


bool onObstacle (pair < double, double >vert, vector < linObstacle > obs, int nObs)             // Checking for linear obstacles: random generated node and obstacles are same or different
{
  double xdiv, ydiv;
  for (auto i = 0; i < nObs; ++i)
    {
      xdiv = (vert.first - obs[i].x1) / (obs[i].x2 - obs[i].x1);
      ydiv = (vert.second - obs[i].y1) / (obs[i].y2 - obs[i].y1);
      if (xdiv == ydiv)                     
	    return true;
    }
  return false;             // will return either true or false depending on the random generated node and obstacles are same or different
}


double direction (pair < double, double >pi, pair < double, double >pj, pair < double, double >pk)      // Whether the the end points joining the obstacle lie on the same side or not (finding intersection)
{
  pair < double, double >seg1;
  seg1.first = pk.first - pi.first;
  seg1.second = pk.second - pi.second;
  pair < double, double >seg2;
  seg2.first = pj.first - pi.first;
  seg2.second = pj.second - pi.second;
  auto tmp1 = seg1.first * seg2.second;
  auto tmp2 = seg2.first * seg1.second;
  return tmp1 - tmp2;           // Return value can be checked for the direction 
}           


bool cutsObstacle (pair < int, int >nearnode, pair < int, int >newnode, vector < linObstacle > obs, int nObs)     // driver function to check intersection of random segment and nearest node with obstacle
{
  for (auto i = 0; i < nObs; ++i)
    {
      auto d1 = direction (make_pair (obs[i].x1, obs[i].y1), make_pair (obs[i].x2, obs[i].y2), nearnode);
      auto d2 = direction (make_pair (obs[i].x1, obs[i].y1), make_pair (obs[i].x2, obs[i].y2), newnode);
      auto d3 =	direction (nearnode, newnode, make_pair (obs[i].x1, obs[i].y1));
      auto d4 =	direction (nearnode, newnode, make_pair (obs[i].x2, obs[i].y2));
      if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))         // check for the intersection with all obstacles
	     return true;
    }
  return false;         // returns the intersection is there or not
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

  int nObs;
  cout << "Enter number of obstacles ";                 // user defined number of obstacles
  cin >> nObs;

  vector < linObstacle > obs (nObs);                    // defining struct type vector for obstacles
  for (auto i = 0; i < nObs; ++i)
  {   cout << "Enter the location of obstacles";                    //Taking the location of obstacles and storing it in vector obs
      cin >> obs[i].x1 >> obs[i].y1 >> obs[i].x2 >> obs[i].y2;
  }
  
  int count = 0;
  bool vertLimit = false;
  static double xdel = (goal.first - xinit) / nVert;
  static double ydel = (goal.second - yinit) / nVert;
  //cout << xdel << endl << ydel << endl;
  cout << "Iteration" << count + 1;
  while (count < iter)
    {
      auto newnode = randomConfig (node.back ().first, node.back ().second, xdel + 0.5, ydel + 0.5);   // Generating random nodes
      if (onObstacle (newnode, obs, nObs))                      // Checking if the Obstacle and Random nodes are same or not
	    {
	            cout << "RandomConfig is on the Obstacle\n";
	            continue;                                   // Since the random config is obstacke it will go for generating new node
	    }
      auto nearnode = getNearest (newnode, node, node.size ());             // Checking for the nearest node point
      if (cutsObstacle (nearnode, newnode, obs, nObs))                          // checking the node point and obstacle are on the same line or not
	    {
	            cout << "(" << newnode.first << "," << newnode.second << ") (" << nearnode.first << "," << nearnode.second << ")";
	            cout << " The nearest node and the initial node is cut by obstacle \n";
	            continue;
    	}
      cout << "Adding node to the tree node and the co-ordinates of the node are " << newnode.first << ", " << newnode.second << endl;
      //cout << round (newnode.first) << " " << round (newnode.second) << endl;
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
