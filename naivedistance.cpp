#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
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

double distance(int nb_ind, double s1[], double s2[]);

void naiveDistance(Image image, int nbcmp)
{ 

	// CALCULUS
	// FIRST : EXTRACT MAIN CONNECTED COMPONANT
  DigitalSet set2d( image.domain() );
  SetFromImage<DigitalSet>::append<Image>(set2d, image, 0, 255);
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

  // THIRD : GET SIGNATURE
  int nb_ind = 10;
  double signature[nb_ind];
  signature[0] = ((double) contour.size() / sqrt((double) set2d.size())) ;
  signature[1] = distFarthestPoint(set2d, bar) /  sqrt((double) set2d.size());
  signature[2] = ((double) ch.size());
  signature[3] = indicatorMaxSegment(ch);
  signature[4] = indicatorVarSegment(ch);
  signature[5] = indicatorAngle(ch);
  signature[6] = DTresults[0] / sqrt((double) set2d.size());
  signature[7] = DTresults[3] / sqrt((double) set2d.size());  
  signature[8] = DTresults[4] / sqrt((double) set2d.size());
  signature[9] = DTresults[5] / sqrt((double) set2d.size());

  // FOURTH : GET RESULTS FROM DATABASE
  int nb_ind_calc = 10; // nb d'indicateurs qui ont été calculés pour toute la DB
  double results[70][15][nb_ind_calc];
  for(int k=0; k<nb_ind_calc; k++){
    string filename = "../resultat/resultat_liste_ind" + to_string(k+1) + ".dat";
    ifstream file (filename);
    string line;
    if (file.is_open()){
      getline (file,line);
      for(int i=0; i<70; i++){
        for(int j=0; j<15; j++){
          getline (file,line);
          //cout << "class " << i << " value " <<line.substr(line.find(' ')) <<endl;
          double r = stod(line.substr(line.find(' ')));
          results[i][j][k] = r;
        }
      }
      file.close();

    }
    else { cout << "unable to open " << filename << endl;}
  }

  // FIFTH : RETURN BEST MATCHES
  vector<double> best(nbcmp,100);
  vector<pair<int,int>> img(nbcmp);
  for(int i=0; i<70; i++){
    for(int j=0; j<15; j++){
      double d = distance(nb_ind_calc, results[i][j], signature);
      if(d < best[nbcmp-1]) {
        if( d < best[0]) {
          best.insert(best.begin(), d);
          img.insert(img.begin(), make_pair(i,j));
        }
        for(int k=1; k<nbcmp; k++){
          if(d>best[k] && d<best[k-1]) {
            best.insert(best.begin(), k, d);
            img.insert(img.begin(), k, make_pair(i,j));
          }
        }
      }
    }
  }

  for(int i=0; i<nbcmp; i++){
    cout << "best distance is " << best[i] << " for image " 
         << (get<0>(img[i]) +1) << ", " << (get<1>(img[i]) +1) << endl;
  }
}

double distance(int nb_ind, double s1[], double s2[]){
  double r=0;
  for(int i=0; i<nb_ind; i++){
    r += 2*abs(s1[i]-s2[i])/(s1[i]+s2[i]);
  }
  return r;
}

