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
  my_amoeba () {f=NULL;}
  ~my_amoeba () {}

  double get_value (const double_1d& list_)
  {

    if (f)
      {
	double_1d_cit lit=list_.begin();
	const double &x=*lit;
	double toreturn=exp(-(x+50)*(x+50)/4000)*((x-60)*(x-60)*sin(x))+x/100*x/100;
	if (list_.size() >=2)
	  {
	    const double &y=*(++lit);
	    toreturn+=(y-2)*(y-2);
	    if (list_.size() >=3)
	      {
		const double &z=*(++lit);
		toreturn+=(z+20)*(z+20);
	      }
	  }
	return toreturn;/*

	double *par=new double[list_.size()];
	int dit=0;
	for (double_1d_cit lit=list_.begin(); lit!=list_.end(); lit++)
	  {
	    par[dit]=*lit;
	    dit++;
	  }
	double toreturn=f->EvalPar (par);
	delete [] par;
	return toreturn;*/
      }
    return 0;
  }

  TF3* f;
};


int main (int argc_, char ** argv_) {

  bool debug=false;
  unsigned int noses=0;
  unsigned int loop=1;
  unsigned int dim=1;
  double *sigma=0;
  string outfile="./test.root";

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
	       sigma=new double[dim];
	       for (unsigned int it=0; it < dim &&  ++iarg<argc_; it++)
		 {
		   sigma[it]=atof(argv_[iarg]);
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
  ma.f=f;

  double minz=0,miny=0,minx=0;
  if (dim >= 3)
    f->GetMinimumXYZ(minx,miny,minz);
  if (dim == 2)
    f->GetMinimumXY(minx,miny);
  if (dim <= 1)
    minx=f->GetMinimumX();

  unsigned int max_noses=noses;
  unsigned int min_noses=noses;
  if (loop>1)
    {
      min_noses=0;
      max_noses=100;
    }

  cout << "research of the minimum at more than 99.9999% of confidence...please wait." << endl;
  double sureminx=0, sureminy=0, sureminz=0;

  ma.set_starts (starts);

  if (sigma!=0)
    ma.add_nose (sigma,20000);
  else
    ma.add_nose (limits,20000);

  result=ma.find_min ();
  sureminx=result.front ();
  {
    amoeba::double_1d_cit rit=result.end();
    if ( result.size()>=3)
      sureminz=*(--rit);
    if ( result.size()>=2)
      sureminy=*(--rit);
  }
  ma.rm_noses();



  cout << endl;

  for (noses=min_noses; noses<=max_noses; noses++)
    {
      if (sigma!=0)
	ma.add_nose (sigma,noses-ma.nb_noses());
      else
	ma.add_nose (limits,noses-ma.nb_noses());
      struct timeval tv;
      gettimeofday(&tv, NULL);

      double success=0;
      long elapsed_time=1000000*tv.tv_sec+tv.tv_usec;
      for (unsigned int lit=0; lit<loop; lit++)
	{
          ma.set_starts (starts);
	  if (sigma==0)
            ma.set_limits (limits);

	  result=ma.find_min ();

	  amoeba::double_1d_cit rit=result.begin();

	  if ((int)(*rit) == (int)(sureminx+.5) &&
	      (result.size()<2 || (int)(*(++rit)+.5) == (int)(sureminy+.5)) &&
	      (result.size()<3 || (int)(*(++rit)+.5) == (int)(sureminz+.5)))
	      success+=1;

	}
      gettimeofday(&tv, NULL);
      elapsed_time=1000000*tv.tv_sec+tv.tv_usec-elapsed_time;
      if (loop>1)
	{
	  htime->Fill (noses,((double)elapsed_time)/loop);
	  hefficace->Fill(noses, success/loop*100);
	  cout << "\rnose nb "<< noses << "    ";cout.flush();
	}
    }

  if (loop>1)
    {
      hefficace->Write ();
      htime->Write ();
      file->Close();
    }
  double* par=new double[result.size()];
  int dit=0;

  cout << "real min : " ;

      par[0]=sureminx;
      cout << par[0] << ";";
      if (result.size()>=2)
	{
	  par[1]=sureminy;
	  cout << par[1] << ";";
	}
      if (result.size()>=3)
	{
	  par[2]=sureminz;
	  cout << par[2] << ";";
	}

      cout<< " f(pos)="<<ma.f->EvalPar(par)<<")"<<endl;

  cout << "amoeba min : ";

  for (amoeba::double_1d_cit rit=result.begin();
       rit!=result.end();
       rit++)
    {
      par[dit]=*rit;
      dit++;
      cout << *rit<<';';
    }
  cout << '='<< result.get_value() << " (f(pos)=" <<ma.f->EvalPar(par)<<")"<<endl;

  cout << "Minuit min : "<< minx <<";";
  if (dim>=2)
    cout << miny <<";";
  if (dim>=3)
    cout << minz <<";";

  cout<<"="<< ma.f->GetMinimum()<<endl;
  cout << "in " << ma.get_counter () << " steps"<<endl;
  delete [] par;
  return 0;
}
