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

Point p0;




bool comp(Point x, Point y){
  return (orientation(p0, x, y) == 1);
}

vector<Point> convexHull(vector<Point> contour){
  Point bottomleft = contour[0];
  vector<Point>::iterator it;
  int rank = 0;
  for(it = contour.begin()+1; it != contour.end(); ++it){
    if(bottomleft[0] >= (*it)[0] && bottomleft[1] >= (*it)[1]){
      bottomleft = *it;
      rank = (it - contour.begin())/sizeof(Point); 
    }
  }
  p0 = bottomleft;

  vector<Point> tmp(contour);
  sort(tmp.begin(), tmp.end(), comp);

  vector<Point> ch(tmp.begin(), tmp.begin() + 2);
  int n = 2; // size of ch
  //cout << ch << endl;

  for(int i = 2; i<tmp.size(); i++){
    while(orientation(ch[n-1], ch[n], tmp[i]) == -1){
      ch.pop_back();
      n--;
    }
    ch.push_back(tmp[i]);
    n++;
  }

  return ch;
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
    pp = nextpoint(pin - pout);
    while(not(set2d(pout + pp)) && c < 100000){
      c++;
      pp = nextpoint(pp);
    }
    //cout << " -- vers : " << pp << endl;

    pin = pout;
    pout += pp;
    contour.push_back(pin);
  }
  if (c == 100000)
    return -1;

  vector<Point> ch = convexHull(contour);
  //cout << " bordure : entre " << pout << " et " << contour[0] << endl;
  //cout << "nb de points dans l'image : " << set2d.size() << endl;
  //cout << "nb de points dans le contour : " << contour.size() << endl;
  cout << ((float) contour.size() / sqrt((float) set2d.size())) << endl;
  cout << distFarthestPoint(set2d, barycentre(set2d)) /  sqrt((float) set2d.size())<< endl;
  cout << ((float) ch.size())/(float) contour.size() << endl;
  /*for(int i =0; i< contour.size(); i++){
    cout << "   -- "<< contour[i] << endl;
  }*/
  
  //We display the set

  Color red( 255, 0, 0 );
  Board2D board;
  board << set2d
        << CustomStyle( p0.className(), new CustomColors( red, red ) )
        <<image.domain() ;
  for(int i=0; i<(ch.size()-1); i++){
    board.drawLine(ch[i][0], ch[i][1], ch[i+1][0], ch[i+1][1]);
  }


  board.saveEPS("hop-set.eps");


  return 0;
}
