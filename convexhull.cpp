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
#include "convexhull.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
          //32bit integers

Point p0 = Point(1,1);


bool comp(Point x, Point y){
  if (orientation(p0, x, y) == 0) {
    return ( (x-p0).norm() < (y-p0).norm() );
  }
  else return (orientation(p0, x, y) == -1);
}

vector<Point> convexHull(vector<Point> contour){
  Point bottomleft = contour[0];
  vector<Point>::iterator it, mem;
  mem = contour.begin();
 for(it = contour.begin()+1; it != contour.end(); ++it){
    if(bottomleft[1] > (*it)[1] || (bottomleft[1] == (*it)[1] && bottomleft[0] > (*it)[0])){
      bottomleft = *it;
      mem = it;
    }
  }
  p0 = bottomleft;
  contour.erase(mem);
  vector<Point> tmp(contour);
  cout << "size " << tmp.size() << " " << tmp.max_size() << endl;
  sort(tmp.begin(), tmp.end(), comp);

  vector<Point> ch(3);
  ch[0] = p0;
  ch[1] = tmp[0];
  ch[2] = tmp[1];
  int n = 2; // size of ch
  //cout << ch << endl;

  for(int i = 3; i<tmp.size(); i++){
    while(n >=1 && orientation(ch[n-1], ch[n], tmp[i]) >= 0){
      ch.pop_back();
      n--;
    }
    ch.push_back(tmp[i]);
    n++;
  }
  
  return ch;
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

double indicatorVarSegment(vector<Point> ch){
  int n = ch.size();
  double moy = (ch[n-1] - ch[0]).norm();
  for(int i = 0; i < n-1; i++){
    moy += (ch[i+1] - ch[i]).norm();
  }
  moy = moy/n;
  double var = pow((ch[n-1] - ch[0]).norm() - moy, 2);
  for(int i = 0; i < n-1; i++){
    var += pow((ch[i+1] - ch[i]).norm() - moy,2);
  }
  var = var/n;
  return var;
}

