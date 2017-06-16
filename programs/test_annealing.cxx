#include <annealing.h>
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

class my_amoeba : public annealing {

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
  unsigned int loop=1;
  unsigned int dim=1;
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
	   else if (option == "-n" || option == "--noses")
	     {
	       noses=atoi(argv_[++iarg]);
	     }
	   else if ((option == "-p"))
	     {
	       loop=atoi(argv_[++iarg]);
	     }
	   else if (option=="-dim" && ++iarg<argc_)
	     {
	       dim=atoi(argv_[iarg]);
	     }
	   else if (option=="-o" && ++iarg<argc_)
	     {
	       outfile=argv_[iarg];
	     }
	   else if (option=="-s")
	     {
	       for (unsigned int it=0; it < dim &&  ++iarg<argc_; it++)
		 {
		   sigma.push_back(atof(argv_[iarg]));
		 }
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


  TFile *file=0;

  TH1F* hefficace=0,*htime=0;
  if (loop > 1)
    {
      file=new TFile(outfile.data(),"recreate");
      htime=new TH1F("htime","time;noses;us",100,0,100);
      hefficace=new TH1F("hefficacity","efficacity;noses;percent",100,0,100);
    }


  amoeba::point result;
  amoeba::double_1d tmp;
  amoeba::double_2d starts;
  amoeba::double_2d limits;
  TF3* f=NULL;
  if (dim <= 1)
    f=(TF3*)(new TF1 ("f","TMath::Exp(-(x+50)*(x+50)/4000)*(x-60)*(x-60)*sin(x)", -100,100));
  if (dim == 2)
    f= (TF3*)(new TF2("f","TMath::Exp(-(x+50)*(x+50)/4000)*(x-60)*(x-60)*sin(x)+(y-2)*(y-2)", -100,100,-100,100));
  if (dim >= 3)
    f=new TF3 ("f","TMath::Exp(-(x+50)*(x+50)/4000)*(x-60)*(x-60)*sin(x)+(y-2)*(y-2)+(z+20)*(z+20)", -100,100,-100,100,-100,100);


  tmp.push_back (70);
  if (dim >= 2)
    tmp.push_back (70);
  if (dim >=3)
    tmp.push_back (70);
  starts.push_back (tmp);
  tmp.clear ();
  tmp.push_back (-25);
  if (dim >= 2)
    tmp.push_back (20);
  if (dim >=3)
    tmp.push_back (0);
  starts.push_back (tmp);
  tmp.clear ();
  if (dim >= 2)
    {
      tmp.push_back (-70);
      tmp.push_back (-70);
      if (dim >=3)
	tmp.push_back (-70);
      starts.push_back (tmp);
      tmp.clear ();

      if (dim >=3)
	{
	  tmp.push_back (-10);
	  tmp.push_back (10);
	  tmp.push_back (30);
	  starts.push_back (tmp);
	  tmp.clear ();

	}

    }

  tmp.push_back (-100);
  tmp.push_back (100);
  for (unsigned int it = 0; it<dim; it++)
    limits.push_back (tmp);


  my_amoeba ma;
  ma.set_debug(debug);
  ma.set_warn(false);
  ma.set_delta(1e-7,1e-7);

  cout << "research of the minimum at more than 99.9999% of confidence...please wait." << endl;
  double sureminx=0, sureminy=0, sureminz=0;

  ma.set_starts (starts);

  if (sigma.size ())
    ma.set_noses (sigma);
  else
    ma.set_noses (limits);

  result=ma.find_min ();
  sureminx=result.front ();
  {
    amoeba::double_1d_cit rit=result.end();
    if ( result.size()>=3)
      sureminz=*(--rit);
    if ( result.size()>=2)
      sureminy=*(--rit);
  }
  return 0;
}
