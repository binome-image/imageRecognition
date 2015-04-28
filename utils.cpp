///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stack>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "utils.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers


int orientation(Point p, Point q, Point r){
  Point pq = q - p;
  Point pr = r - p;
  int det = pq[1]*pr[0] - pq[0]*pr[1];
  if(det > 0) return 1;
  if(det < 0) return -1;
  return 0;
}

Point nextpoint(Point p) {
  int i = 0;
  static Point array[] = {Point(0,1), Point(1,0), Point(0,-1), Point(-1,0),Point(0,1)};
  while(array[i] != p && i <4) {
    i++;
  }
  return array[i+1];
}

Point barycentre(DigitalSet forme){
  DigitalSet::Iterator it;
  Point bar(0,0);
  for( it = forme.begin(); it != forme.end(); ++it){
    bar += *it;
  }
  bar[0] = bar[0]/forme.size();
  bar[1] = bar[1]/forme.size();
  return bar;
}

double distFarthestPoint(DigitalSet forme, Point p){
  DigitalSet::Iterator it;
  double r;

  for( it = forme.begin(); it != forme.end(); ++it){
    r = max(r, (*it - p).norm());
  }
  return r;
}

vector<Point> borderExtraction(DigitalSet set2d){
  //border extraction
  Point pin = *set2d.begin();
  Point pout = pin;
  Point pp, tmp;
  vector<Point> contour;


  pout[1]++;
  int maxH = set2d.domain().upperBound()[1];
  int maxW = set2d.domain().upperBound()[0];
  while(pout[1] <= maxH) {
	while (pout[1] <= maxH && set2d(pout)) {
	pout[1]++;
	}
  	tmp = pout;
  	while (pout[1] <= maxH && !set2d(pout)) {
	pout[1]++;
	}
  }
  pout = tmp;
  pin = tmp;
  pin[1]--;
  contour.push_back(pin);
  pout[1]--;
  pout[0]++;
  if(!set2d(pout)) {
    pout[1]--;
    pout[0]--;
  }
  int c = 0;
  while( contour[0] != pout && c < 100000){
    c++;
    pp = nextpoint(pin - pout);
    while( (pout+pp)[0] > maxW || (pout+pp)[1] > maxH || not(set2d(pout + pp)) && c < 100000){
      c++;
      pp = nextpoint(pp);
      if(pp == pin - pout) break;
    }
    //cout << " -- vers : " << pp << endl;

    pin = pout;
    pout += pp;
    contour.push_back(pin);
  }
  return contour;
}


double indicatorMaxSegment(vector<Point> ch){
	int n = ch.size();
	double maxseg = (ch[n-1] - ch[0]).norm();
	double totallength = maxseg;
	for(int i = 0; i < n-1; i++){
		double l = (ch[i+1] - ch[i]).norm();
		totallength += l;
		if (maxseg < l) {maxseg = l;}
		//cout << l << " "<< maxseg << " " << totallength << endl;
	}
	return (maxseg/totallength);
}



