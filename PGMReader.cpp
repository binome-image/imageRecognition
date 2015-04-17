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


Point next(Point p) {
  vector<Point> next(7);
  next[0] = Point(0,1);
  next[1] = Point(1,0);
  next[2] = Point(0,-1);
  next[3] = Point(-1,0);
  next[4] = Point(0,1);
  next[5] = Point(1,0);
  next[6] = Point(0,-1);

  std::vector<Point>::iterator it;

  it = find (next.begin(), next.end(), p);
  it++;
  return *it;
}

int main()
{

  std::string filename = "hop.pgm";

  //Image type (image of unsigned int)
  typedef ImageContainerBySTLVector< Domain, unsigned int > Image;

  //We read the PGM file
  Image image = PGMReader<Image>::importPGM(filename);

  trace.info() << "Image read :"<< image <<std::endl;

  //We convert pixels in ]0,255] into a digital set
  DigitalSet set2d( image.domain() );
  SetFromImage<DigitalSet>::append<Image>(set2d, image, 0, 255);

  //border extraction
  Point pin = *set2d.begin();
  Point pout = pin;
  Point pp;
  vector<Point> contour;


  pout[1]++;
  while (set2d(pout)) {
    pin[1]++;
    pout[1]++;
    }
  contour.push_back(pin);
  pout[1]--;
  pout[0]++;
  if(set2d(pout))
    cout << "go left" << endl;
  else{
    cout << "go down" << endl;
    pout[1]--;
    pout[0]--;
  }
  
  while( contour[0] != pout ){
    pp = next(pin - pout);
    pin = pout;
    pout += pp;
    contour.push_back(pin);
  }
  cout << " bordure : entre " << pout << " et " << pin << endl;
  cout << "nb de points dans l'image : " << set2d.size() << endl;
  cout << "nb de points dans le contour : " << contour.size();
  
  
  //We display the set
  Board2D board;
  board << image.domain() << set2d;

  board.saveEPS("hop-set.eps");

  return 0;
}
