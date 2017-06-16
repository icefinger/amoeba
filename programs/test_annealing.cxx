#include <annealing.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <ctime>
#include <cmath>
using namespace std;

//creating the minimal class class that will use annealing. This class is based on nosy, so it can combine the both methods
class my_amoeba : public annealing {

public:
  my_amoeba () {}
  ~my_amoeba () {}

  //only get_value is absolutly required. It should return the value at the coordinates in coord_ (can be illimited in dimension)
  double get_value (const double_1d& list_)
  {
    double x = list_[0]-40;
    double y = list_[1]+44;
    double value=-100*exp(-x*x/7200)-100*exp(-y*y/7200);

    return value;

  }

};


int main (int argc_, char ** argv_) {

  bool debug=false;
  unsigned int noses=0;
  amoeba::double_1d sigma;
  int iarg = 1;
  double sigma_val=5;
  while (iarg < argc_)
    {
      string token = argv_[iarg];

      if (token[0] == '-')
	{
	  string option = token;
	  if ((option == "-d") || (option == "--debug"))
	    {
	      debug = true;
	    }
	   else if (option == "-n" || option == "--noses")
	     {
	       noses=atoi(argv_[++iarg]);
	     }
	   else if (option=="-s")
	     {
               sigma_val=atof(argv_[iarg]);
	     }

	  else
	    {
	      clog << "warning: ignoring option '" << option << "'!" << endl;
	    }
	}
      else
	{
	  string argument = token;
	  /* Here you may add more argument handlers... */
	  {
	    clog << "warning: ignoring argument '" << argument << "'!" << endl;
	  }
	}
      iarg++;
    }

    //putting the number of noses that will scan. In this example a sigma is used, so it will scan around the current minimal value with a normal law. If no sigma is given, it scans the full map. It does only one scan per nose per iteration.
  //if the number of noses>1, it will combine nosy and annealing. If the number of noses is <=1, it will use only the annealing algorithm
  for (size_t it = 0; it<noses; it++)
    sigma.push_back(sigma_val);


  //the return. Point is a double_1d plus a returned value from get_value
  amoeba::point result;

  //for 2d needs 2x2d limits to the amoeba. In this example it's a sphere 180°x360° (theta,phi)
  //                       n,dim       minx,maxx       miny,maxy
  amoeba::double_2d limits(2,2,       -180.,180.,     -180.,180.);

  //for 2d needs 3x2d legs for the amoeba
  //                       n,dim   x1,y1        x2,y2      x3,y3
  amoeba::double_2d starts(3,2, -100.,-100.,  100.,30.,   10.,30.);

  //creating and putting the start data
  my_amoeba ma;
  ma.set_debug(debug);
  ma.set_warn(false);
  //the deltas are for the resolution (1°) and the difference between the already scanned values.
  ma.set_delta(1,1);
  ma.set_starts (starts);
  ma.set_limits (limits);

  cout << "research of the minimum at more than 99.9999% of confidence...please wait." << endl;

  ma.set_starts (starts);

  //here we put the noses with the value of sigma. If no sigma has been given, a nose with the distance between the start points as sigma will be created.
  if (sigma.size ())
    ma.set_noses (sigma);

  //the result is directly returned by find_min
  result=ma.find_min ();
  cout << "result: " << result << endl;

  return 0;
}
