#include <nosy.h>
#include <cstdlib>
#include <iostream>
#include <TF3.h>
#include <string>
#include <sys/time.h>
#include <ctime>
#include <TH1.h>
#include <TFile.h>
#include <cmath>
using namespace std;

class my_amoeba : public nosy {

public:
  my_amoeba () {}
  ~my_amoeba () {}

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
  string outfile="./test.root";
  amoeba::double_1d sigma;
  int iarg = 1;
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
	  else if ((option == "-n" || option == "--noses") && iarg+1<argc_)
	     {
	       noses=atoi(argv_[++iarg]);
	     }
	   else if (option=="-o" && ++iarg<argc_)
	     {
	       outfile=argv_[iarg];
	     }
	   else if (option=="-s" )
	     {
               double sigma_val=atof(argv_[iarg]);
               for (size_t it = 0; it<noses; it++)
                 sigma.push_back (sigma_val);
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

  //testing for 2d->1d function
  amoeba::point result;

  //for 2d needs 2x2d limits to the amoeba
  //                       n,dim      minx,maxx     miny,maxy
  amoeba::double_2d limits(2,2,       -180.,180.,     -180.,180.);

  //for 2d needs 3x2d legs for the amoeba
  //                       n,dim   x1,y1    x2,y2    x3,y3
  amoeba::double_2d starts(3,2,    -100.,-100.,  100.,30.,   10.,30.);

  my_amoeba ma;
  ma.set_debug(debug);
  ma.set_warn(false);
  ma.set_delta(1,1);
  ma.set_starts (starts);
  ma.set_limits (limits);

  if (sigma.size ())
    ma.set_noses (sigma);
  else
    ma.set_noses (limits);

  result=ma.find_min ();
  cout << "result: " << result << endl;
  return 0;
}
