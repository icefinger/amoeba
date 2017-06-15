#include <nosy.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>


using namespace std;

nosy::nosy ()
{
  __warn=true;
  __normal=false;
  __sigma=NULL;
  struct timeval tv;
  gettimeofday(&tv, NULL);

  __generator.seed (1000000*tv.tv_sec+tv.tv_usec);
}

nosy::~nosy ()
{
}

void nosy::add_nose (const double_2d &limits_, unsigned int nb_)
{
  if (!__check_noses(limits_, nb_))
    exit (1);

  set_limits(limits_);

  __add_nose(nb_);

  __normal=false;

  user_work ();
}

void nosy::add_nose (double *sigma_, unsigned int nb_)
{
  if (sigma_==0)
    return;

  __sigma=sigma_;
  __normal=true;
  __add_nose(nb_);

}

void nosy::__add_nose (unsigned int nb_)
{
  for (unsigned int nb=0; nb<nb_; nb++)
    {
      point tmp;
      tmp.set_amoeba(this);
      __noses.push_back(tmp);
    }
}

void nosy::user_work ()
{

  for (list<point>::iterator nit=__noses.begin();
       nit!=__noses.end();
       nit++)
    {
      double_1d pos;
      if (__normal && __sigma)
	{
	  int sit=0;
	  for (double_1d_cit lmean=get_PQR(2).begin(); lmean!=get_PQR(2).end (); lmean++)
	    {
              normal_distribution<double>::param_type p(*lmean, __sigma[sit]);
              __normal_distribution.param(p);
	      double val= __normal_distribution (__generator);
	      pos.push_back (val);
	      sit++;
	    }
	}
      else
	for (double_2d_cit lit=get_limits().begin(); lit!=get_limits().end(); lit++)
	{
	  double range=(*lit).back()-(*lit).front();
	  double min=(*lit).front();
	  double val;
          std::uniform_real_distribution<double>::param_type p(min, range);
          __uniform_distribution.param (p);
	  val= __uniform_distribution (__generator);
	  pos.push_back (val);
	}
      (*nit).set_pos(pos);
      double rnit=(*nit).get_r ();
      double rPQR2=get_PQR(2).get_r();
      double rPQR0=get_PQR(0).get_r();
      double valit=(*nit).get_value();
      double valPQR2=get_PQR(2).get_value();
      if ( fabs(rnit - rPQR2) > fabs(rPQR2-rPQR0) &&
	   ((*nit).get_value()<get_PQR(2).get_value () ||
	    accept (valit,valPQR2)))
	{
	  set_PQR(*nit,2);
	}
    }

}

bool nosy::__check_noses (const double_2d &limits_, unsigned int nb_) const
{
  if (!PQR_size ())
    {
      cerr << "ERROR:nosy: You should add noses after the normal feets.\n";
      return false;
    }

  if (limits_.size() != get_PQR(0).size())
	{
	  cerr << "ERROR:nosy: bad dim for noses.\n";
	  return false;
	}

  for (double_2d_cit lit=limits_.begin(); lit!=limits_.end(); lit++)
    {
      if ((*lit).size() != 2)
	{
	  cerr << "ERROR:nosy: bad number of limits for noses. it should be 2.\n";
	  return false;
	}

      if ((*lit).front () >= (*lit).back())
	{
	  cerr << "ERROR:nosy: the front should be the min and different to the back.\n";
	}

    }
  if (__warn && nb_ > PQR_size ())
    clog << "WARNING: There is a lot of noses. It could burden the amoeba.\n";

  return true;

}
