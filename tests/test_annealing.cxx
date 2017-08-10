#include <annealing.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <ctime>
#include <cmath>
#include <Tools.h>

#ifdef ENABLE_ROOT
void save_visu (const char* file_root_, const icedcode::annealing& am_);
#endif

using namespace std;
using namespace icedcode;

//creating the minimal class class that will use annealing. This class is based on nosy, so it can combine the both methods
class my_amoeba : public annealing {

public:
  my_amoeba (double temp_): annealing (temp_)
  {}
  ~my_amoeba () {}

  //only get_value is absolutly required. It should return the value at the coordinates in coord_ (can be illimited in dimension)
  double get_value (const double_1d& x_)
  {
    double value=global_get_value (x_.data (), 0);

    return value;
  }

};


int main (int argc_, char ** argv_) {

  bool visu=false;
  bool debug=false;
  unsigned int noses=0;
  amoeba::double_1d sigma;
  int iarg = 1;
  double sigma_val=50, temp = 400;
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
          else if (option == "-visu")
            {
              visu = true;
#ifndef ENABLE_ROOT
              cerr << "No visu, compile with the ENABLE_ROOT=TRUE option." << endl;
              return 0;
#endif
            }
	   else if (option == "-n" || option == "--noses")
	     {
	       noses=atoi(argv_[++iarg]);
	     }
	   else if (option=="-s")
	     {
               sigma_val=atof(argv_[++iarg]);
	     }
           else if (option=="-temp")
             {
              temp = atof(argv_[++iarg]);
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
  my_amoeba ma(temp);
  ma.set_debug(debug);
  ma.set_warn(false);
  ma.set_save_steps (visu);
  //the deltas are for the resolution (1°) and the difference between the already scanned values.
  ma.set_delta(0.1,0.1);
  ma.set_starts (starts);
  ma.set_limits (limits);

  cout << "research of the minimum...please wait." << endl;

  ma.set_starts (starts);

  //here we put the noses with the value of sigma. If no sigma has been given, a nose with the distance between the start points as sigma will be created.
  if (sigma.size ())
    ma.set_noses (sigma);

  //the result is directly returned by find_min
  result=ma.find_min ();

#ifdef ENABLE_ROOT
  if (visu)
    save_visu ("simpleDraw", ma);
#endif

  cout << "result: " << result << endl;

  return 0;
}


#ifdef ENABLE_ROOT
#include <Drawer.h>
#include <TGraph.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF2.h>

void save_visu (const char* name_root_file_, const annealing& am_)
{
  TF2* f=new TF2("func", global_get_value, -180, 180, -180, 180);
  f->SetNpx (300);
  TCanvas *canvas = new TCanvas;

  for (size_t i = 0; i<am_.get_saved_steps ().size ();i++)
    {
      f->Draw ();

      //PQR points
      TGraph *PQRgraph = get_PQR_graph (am_, i);
      PQRgraph->Draw ("same");
      //noses
      TGraph *noses_graph = get_noses_graph (am_, i-1);
      if (noses_graph)
        noses_graph->Draw ("same*");

      string name = name_root_file_;
      name = name+to_string (i)+".png";
      canvas->SaveAs (name.data());

      delete (PQRgraph);
      if (noses_graph)
        delete (noses_graph);
    }
}
#endif
