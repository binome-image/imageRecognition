using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers

bool comp(Point x, Point y);

vector<Point> convexHull(vector<Point> contour);


double indicatorMaxSegment(vector<Point> ch);

double indicatorVarSegment(vector<Point> ch);

double indicatorAngle(vector<Point> ch);