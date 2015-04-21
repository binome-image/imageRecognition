

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

int util(){
	return 1;
}

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



