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
#include "DGtal/images/ImageSelector.h"
#include "utils.h"
#include "convexhull.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers

typedef ImageSelector < Z2i::Domain,uint>::Type Image;
void displayEps(DigitalSet set2d, DigitalSet border,
				vector<Point> contour, 
                vector<Point> ch, Point bar,
                Image image );

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

  // CALCULUS
  Object8_4 object(dt8_4, set2d);
  vector<Object8_4> cc;
  back_insert_iterator<vector<Object8_4>>::iterator it(cc);
  object.writeComponents(it);
  //Similarly, Object4_8 and dt4_8 exist if you want to change
  //connectivity
  vector<Point> contour = borderExtraction(set2d);
  Point bar = barycentre(set2d);
  /*for(int i=0; i<(contour.size()); i++){
    cout << contour[i] << endl;
  }*/
  vector<Point> ch = convexHull(contour);
  
  DigitalSet border = object.border().pointSet();
  cout << border << endl << set2d << endl;
  
  // SIGNATURE
  //cout << "bar " << bar<< endl;
  cout << ((float) contour.size() / sqrt((float) set2d.size())) << endl;
  //cout << distFarthestPoint(set2d, bar) /  sqrt((float) set2d.size())<< endl;
  //cout << ((float) ch.size()) << endl; 


  displayEps(set2d, border, contour, ch, bar, image);

  return 0;
}


void displayEps(DigitalSet set2d, DigitalSet border,
				vector<Point> contour, 
                vector<Point> ch, Point bar,
                Image image ){
    //We display the set

  Color red( 255, 0, 0 );
  Color blue( 255, 255, 0 );
  Color green(0,0,200);
  Board2D board;
  board << set2d
        <<image.domain()
        << CustomStyle( bar.className(), new CustomColors( red, red ) );
  /*for(int i=0; i<(contour.size()); i++){
    board << contour[i];  
  }  
  board << CustomStyle( bar.className(), new CustomColors( blue, blue ) ); 
  for(int i=0; i<(ch.size()); i++){
    board << ch[i];
    //cout << ch[i] << endl;
  }
  for(int i=0; i<(ch.size()-1); i++){
    board.drawLine(ch[i][0], ch[i][1], ch[i+1][0], ch[i+1][1]);
  }*/
  //board << CustomStyle( bar.className(), new CustomColors( green,green ) );
  for(DigitalSet::ConstIterator it = border.begin(), itend = border.end();
      it != itend; ++it){
 	 board << *it;
  }
  board.saveEPS("hop-set.eps");
}


