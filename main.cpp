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
#include "naivedistance.h"

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers

typedef ImageSelector < Z2i::Domain,uint>::Type Image;
void displayEps(DigitalSet set2d, DigitalSet border,
				vector<Point> contour, 
                vector<Point> ch, Point bar, Point centreins,
                Image image );

int main(int argc, char** argv)
{ 

  std::string filename = argv[argc-1];
	//Image type (image of unsigned int)
	typedef ImageContainerBySTLVector< Domain, unsigned int > Image;
	//We read the PGM file
	Image image = PGMReader<Image>::importPGM(filename);
	//trace.info() << "Image read :"<< image <<std::endl;
	//We convert pixels in ]0,255] into a digital set
	DigitalSet set2d( image.domain() );
	SetFromImage<DigitalSet>::append<Image>(set2d, image, 0, 255);

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

  if (string(argv[1]) =="-ind1") // PERIMETRE
  {cout << ((double) contour.size() / sqrt((double) set2d.size())) << endl;}
  if (string(argv[1]) =="-ind2") // DISQUECIRCONS
  {cout << distFarthestPoint(set2d, bar) /  sqrt((double) set2d.size())<< endl;}
  if (string(argv[1]) == "-ind3") // NBSEG
  {cout << ((double) ch.size()) << endl;}
  if (string(argv[1]) == "-ind4") // MAXSEG
  {cout << indicatorMaxSegment(ch) << endl;}
  if (string(argv[1]) == "-ind5") // VARSEG
  {cout << indicatorVarSegment(ch) << endl;}
  if (string(argv[1]) == "-ind6") // ANGLESEG
  {cout << indicatorAngle(ch) << endl;}
  if (string(argv[1]) == "-ind7") // MAXDTL2
  { cout << DTresults[0] / sqrt((double) set2d.size()) << endl; }
  if (string(argv[1]) == "-ind8") // MAXDTL1
  { cout << DTresults[3] / sqrt((double) set2d.size()) << endl; }  
  if (string(argv[1]) == "-ind9") // BARYCENTRE-DTL2
  { cout << DTresults[4] / sqrt((double) set2d.size()) << endl; }
  if (string(argv[1]) == "-ind10") // BAR-CENTREINSCRIT
  { cout << DTresults[5] / sqrt((double) set2d.size()) << endl; }
  if (argc >=3 && string(argv[2]) == "-disp")
  {displayEps(set2d, border, contour, ch, bar, centreins, image);}
  if (argc >=3 && string(argv[2]) == "-dispDT")
  { cout << "need to uncomment distancetransformShow(image); to work " << endl;}
  //{ distancetransformShow(image);}
  if (string(argv[1]) == "-ND")
  { naiveDistance(image, argv[2]);}
  return 0;
}


void displayEps(DigitalSet set2d, DigitalSet border,
				vector<Point> contour, 
                vector<Point> ch, Point bar, Point centreins,
                Image image ){
    //We display the set

  Color red( 255, 0, 0 );
  Color blue( 0, 0, 255 );
  Color green(0,255,0);
  Board2D board;
  board << set2d
        <<image.domain()
        << CustomStyle( bar.className(), new CustomColors( red, red ) );
  for(int i=0; i<(contour.size()); i++){
    board << contour[i];  
  }  
  board << bar;
  board << CustomStyle( bar.className(), new CustomColors( blue, blue ) ); 
  for(int i=0; i<(ch.size()); i++){
    board << ch[i];
    //cout << ch[i] << endl;
  }
  for(int i=0; i<(ch.size()-1); i++){
    board.drawLine(ch[i][0], ch[i][1], ch[i+1][0], ch[i+1][1]);
  }
  board << CustomStyle( bar.className(), new CustomColors( green, green ) );
  board << centreins;
  /*for(DigitalSet::ConstIterator it = border.begin(), itend = border.end();
      it != itend; ++it){
 	 board << *it;
  }*/
  board.saveEPS("image.eps");
}


