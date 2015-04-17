///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
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
  cout << " bordure : entre " << pout << " et " << pin << endl;
  cout << "nb de points dans l'image : " << set2d.size() << endl;
  
  //We display the set
  Board2D board;
  board << image.domain() << set2d;

  board.saveEPS( "hop-set.eps");

  return 0;
}
