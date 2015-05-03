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
#include "DGtal/images/ImageSelector.h"
#include "utils.h"
#include "convexhull.h"
#include "distancetransform.h"

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers

typedef ImageSelector < Z2i::Domain,uint>::Type Image;


void naiveDistance(Image image)
{ 

	// CALCULUS
	// FIRST : EXTRACT MAIN CONNECTED COMPONANT
	Object4_8 object(dt4_8, set2d);
	vector<Object4_8> cc;
	back_insert_iterator<vector<Object4_8> > it(cc);
	object.writeComponents(it);
	int maxsize = 0, rank = 0;
	for(int i=0; i< cc.size(); i++){
		if(cc[i].size() > maxsize) { maxsize = cc[i].size(); rank = i; }
  }
  Object4_8 mccObject = cc[rank];
  DigitalSet mcc = mccObject.pointSet();

  // SECOND : COMPUTE SIGNIFICANT SETS
  vector<Point> contour = borderExtraction(mcc);
  Point bar = barycentre(mcc);
  vector<Point> ch = convexHull(contour);
  DigitalSet border = mccObject.border().pointSet();
  vector<double> DTresults = distancetransform(image, bar);
  Point centreins;
  centreins[0] = (int) DTresults[1];
  centreins[1] = (int) DTresults[2];

  vector<double> signature(10)

  signature[0] = ((double) contour.size() / sqrt((double) set2d.size())) ;
  signature[1] = distFarthestPoint(set2d, bar) /  sqrt((double) set2d.size())<< endl;}
  signature[2] = ((double) ch.size()) << endl;}
  signature[3] = indicatorMaxSegment(ch) << endl;}
  signature[4] = indicatorVarSegment(ch) << endl;}
  signature[5] = indicatorAngle(ch) << endl;}
  signature[6] = DTresults[0] / sqrt((double) set2d.size()) << endl;
  signature[7] = DTresults[3] / sqrt((double) set2d.size()) << endl;  
  signature[8] = DTresults[4] / sqrt((double) set2d.size()) << endl;
  signature[9] = DTresults[5] / sqrt((double) set2d.size()) << endl;

}