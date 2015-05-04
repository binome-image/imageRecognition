using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
			    //32bit integers

typedef ImageSelector < Z2i::Domain,uint>::Type Image;

//void distancetransformShow(Image image);

vector<double> distancetransform(Image image, Point barycentre);
