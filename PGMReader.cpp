///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers


Point nextpoint(Point p, vector<Point> next) {
  std::vector<Point>::iterator it;

  it = find (next.begin(), next.end(), p);
  it++;
  return *it;
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


int main(int argc, char** argv)
{

  if(argc < 2){
    cout << "need input pgm picture" << endl;
    return -1;
  }
  std::string filename = argv[1];

  //Image type (image of unsigned int)
  typedef ImageContainerBySTLVector< Domain, unsigned int > Image;

  //We read the PGM file
  Image image = PGMReader<Image>::importPGM(filename);

  //trace.info() << "Image read :"<< image <<std::endl;

  //We convert pixels in ]0,255] into a digital set
  DigitalSet set2d( image.domain() );
  SetFromImage<DigitalSet>::append<Image>(set2d, image, 0, 255);

  //border extraction
  Point pin = *set2d.begin();
  Point pout = pin;
  Point pp;
  vector<Point> contour;
  vector<Point> next(7);
  next[0] = Point(0,1);
  next[1] = Point(1,0);
  next[2] = Point(0,-1);
  next[3] = Point(-1,0);
  next[4] = Point(0,1);
  next[5] = Point(1,0);
  next[6] = Point(0,-1);


  pout[1]++;
  while (set2d(pout)) {
    pin[1]++;
    pout[1]++;
    }
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
    pp = nextpoint(pin - pout, next);
    while(not(set2d(pout + pp)) && c < 100000){
      c++;
      pp = nextpoint(pp, next);
    }
    //cout << " -- vers : " << pp << endl;

    pin = pout;
    pout += pp;
    contour.push_back(pin);
  }
  if (c == 100000)
    return -1;
  //cout << " bordure : entre " << pout << " et " << contour[0] << endl;
  //cout << "nb de points dans l'image : " << set2d.size() << endl;
  //cout << "nb de points dans le contour : " << contour.size() << endl;
  cout << ((float) contour.size() / sqrt((float) set2d.size())) << endl;
  cout << distFarthestPoint(set2d, barycentre(set2d)) /  sqrt((float) set2d.size())<< endl;
  /*for(int i =0; i< contour.size(); i++){
    cout << "   -- "<< contour[i] << endl;
  }*/
  
  //We display the set
  Board2D board;
  board << image.domain() << set2d;

  board.saveEPS("hop-set.eps");

  return 0;
}
