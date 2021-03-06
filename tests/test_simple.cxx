#include <amoeba.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <ctime>
#include <cmath>
#include <Tools.h>

#ifdef ENABLE_ROOT
void save_visu (const char* file_root_, const icedcode::amoeba& am_);
#endif

using namespace std;
using namespace icedcode;

//creating the minimal class class that will use amoeba
class my_amoeba : public amoeba {
public:
  my_amoeba ()
  {
  }
  ~my_amoeba () {}

  //only get_value is absolutly required. It should return the value at the coordinates in coord_ (can be illimited in dimension)
  double get_value (const double_1d& x_)
  {
    //double value=-100*(exp(-((x_[0]*x_[0]-4))/7200)+exp(-((x_[1]*x_[1]+4))/7200)-sin (x_[0]/20)-sin(x_[1]/20));
    double value = global_get_value (x_.data (), 0);
    return value;

  }

};


int main (int argc_, char ** argv_) {

  bool debug=false;
  bool visu=false;

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
          if (option == "-visu")
            {
              visu = true;
#ifndef ENABLE_ROOT
              cerr << "No visu, compile with the ENABLE_ROOT=TRUE option." << endl;
              return 0;
#endif
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

  //the return. Point is a double_1d plus a returned value from get_value
  amoeba::point result;

  //for 2d needs 2x2d limits to the amoeba. In this example it's a sphere 180°x360° (theta,phi)
  //                       n,dim       minx,maxx       miny,maxy
  amoeba::double_2d limits(2,2,       -180.,180.,     -180.,180.);

  //for 2d, needs 3x2d legs for the amoeba
  //                       n,dim      x1,y1        x2,y2      x3,y3
  amoeba::double_2d starts(3,2,    -100.,-100.,  100.,30.,   10.,30.);

  //creating and putting the start data
  my_amoeba ma;
  ma.set_debug(debug);
  ma.set_save_steps (visu);
  //the deltas are for the resolution (1°) and the difference between the already scanned values.
  ma.set_delta(0.1,0.1);

  ma.set_starts (starts);
  ma.set_limits (limits);

  //the result is directly returned by find_min
  result=ma.find_min ();

#ifdef ENABLE_ROOT
  if (visu)
    save_visu ("simpleDraw", ma);
#endif

  cout << "results " << result << " in " << ma.get_counter () << " counts" << endl;

  return 0;
}

#ifdef ENABLE_ROOT
#include <Drawer.h>
#include <TGraph.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF2.h>

void save_visu (const char* name_root_file_, const amoeba& am_)
{
  TF2* f=new TF2("func", global_get_value, -180, 180, -180, 180);
  f->SetNpx (300);
  TCanvas *canvas = new TCanvas;

  for (size_t i = 0; i<am_.get_saved_steps ().size ();i++)
    {
      TGraph *graph = get_PQR_graph (am_, i);
      f->Draw ();
      graph->Draw ("same");
      string name = name_root_file_;
      name = name+to_string (i)+".png";
      canvas->SaveAs (name.data());
      delete (graph);
    }
}
#endif
