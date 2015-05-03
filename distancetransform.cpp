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
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/io/colormaps/GrayscaleColorMap.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/io/colormaps/TickedColorMap.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "utils.h"
#include "convexhull.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
          //32bit integers
typedef ImageSelector < Z2i::Domain,uint>::Type Image;

vector<double> distancetransform(Image image, Point barycentre){
	typedef functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;
  PointPredicate predicate(image,0);
  //! [DTPredicate]  

  //! [DTCompute]
  typedef  DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L2Metric> DTL2;
  typedef  DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L1Metric> DTL1;
 
 
  DTL2 dtL2(image.domain(), predicate, Z2i::l2Metric);
  DTL1 dtL1(image.domain(), predicate, Z2i::l1Metric);
  //! [DTCompute]

  Domain d = dtL2.domain();
  DTL2::Value maxv2=0;
  vector<Point> sup;
  for(DTL2::Domain::ConstIterator it = d.begin(), itend = d.end(); it != itend; ++it) {
		DTL2::Value v = dtL2(*it);
		if(v>maxv2){
			sup.clear();
			sup.push_back(*it);
			maxv2 = v;
      //cout << "maxv2 " << maxv2 << " at " << *it << endl;
		}
		else if(v==maxv2){
			sup.push_back(*it);
      //cout << "more sup for " << maxv2 << endl;
		}
	}

  // distance moyenne entre le barycentre et les points atteignant maxv2 
  // ( a priori un seul point )
  double dist = 0;
  for(int i=0; i<sup.size(); i++){
    dist += (sup[i]-barycentre).norm();
  }
  dist = dist/ sup.size();

 
  DTL1::Value maxv1=0;
  //We compute the maximum DT value on the L1 map
  for ( DTL1::ConstRange::ConstIterator it = dtL1.constRange().begin(), itend = dtL1.constRange().end();it != itend; ++it)
    if ( (*it) > maxv1)  maxv1 = (*it);

  vector<double> result;
  result.push_back( (double) maxv2 );
  result.push_back( sup[0][0]);
  result.push_back( sup[0][1]);
  result.push_back( (double) maxv1 );
  result.push_back( (double) dtL2(barycentre) );
  result.push_back( d );


  return result;
}


/* Fonction adapté de l'exemple de la librairie DGtal distancetransform2D.cpp
situé dans le dossier examples/geometry/volumes/distance/                   */
void distancetransformShow(Image image) {
	
  trace.beginBlock ( "Example distancetransform2D" );

  /*//! [DTDef]
  Z2i::Point a ( 0, 0 );
  Z2i::Point b ( 127, 127);
  
  //Input image with unsigned char values
  typedef ImageSelector<Z2i::Domain, unsigned int>::Type Image;
  Image image ( Z2i::Domain(a, b ));

  //We fill the image with the 128 value
  for ( Image::Iterator it = image.begin(), itend = image.end();it != itend; ++it)
    (*it)=128;
  //We generate 16 seeds with 0 values.
  randomSeeds(image,16,0);
  //! [DTDef]*/

  //! [DTColormaps]
  //Colormap used for the SVG output
  typedef HueShadeColorMap<long int, 2> HueTwice;
  typedef GrayscaleColorMap<unsigned char> Gray;
  //! [DTColormaps]


  //Input shape output
  Board2D board;
  board.setUnit ( LibBoard::Board::UCentimeter );
  Display2DFactory::drawImage<Gray>(board, image, (unsigned int)0, (unsigned int)129);
  board.saveSVG("inputShape.svg");

  //! [DTPredicate]
  //Point Predicate from random seed image
  typedef functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;
  PointPredicate predicate(image,0);
  //! [DTPredicate]  

  //! [DTCompute]
  typedef  DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L2Metric> DTL2;
  typedef  DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L1Metric> DTL1;
 
 
  DTL2 dtL2(image.domain(), predicate, Z2i::l2Metric);
  DTL1 dtL1(image.domain(), predicate, Z2i::l1Metric);
  //! [DTCompute]


  DTL2::Value maxv2=0;
  //We compute the maximum DT value on the L2 map
  for ( DTL2::ConstRange::ConstIterator it = dtL2.constRange().begin(), itend = dtL2.constRange().end();it != itend; ++it)
    if ( (*it) > maxv2)  maxv2 = (*it);
 
  DTL1::Value maxv1=0;
  //We compute the maximum DT value on the L1 map
  for ( DTL1::ConstRange::ConstIterator it = dtL1.constRange().begin(), itend = dtL1.constRange().end();it != itend; ++it)
    if ( (*it) > maxv1)  maxv1 = (*it);
  
  
  trace.warning() << dtL2 << " maxValue= "<<maxv2<< endl;
  board.clear();
  Display2DFactory::drawImage<HueTwice>(board, dtL2, 0.0, maxv2 + 1);
  board.saveSVG ( "example-DT-L2.svg" );

  trace.warning() << dtL1 << " maxValue= "<<maxv1<< endl;
  board.clear();
  Display2DFactory::drawImage<HueTwice>(board, dtL1, 0.0, maxv1 + 1);
  board.saveSVG ( "example-DT-L1.svg" );

  //Explicit export with ticked colormap
  //We compute the maximum DT value on the L2 map
  board.clear();
  TickedColorMap<double, GradientColorMap<double> > ticked(0.0,maxv2, Color::White);
  ticked.addRegularTicks(5, 0.5);
  ticked.finalize();
  ticked.colormap()->addColor( Color::Red );
  ticked.colormap()->addColor( Color::Black );
  for ( DTL2::Domain::ConstIterator it = dtL2.domain().begin(), itend = dtL2.domain().end();it != itend; ++it)
  {
    board<< CustomStyle((*it).className(),new CustomColors(ticked(dtL2(*it)),ticked(dtL2(*it))));
    board << *it;
  }
  board.saveSVG("example-DT-L2-ticked.svg");
  
  trace.endBlock();

}

