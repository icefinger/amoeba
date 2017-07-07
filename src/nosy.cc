#include <nosy.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>


using namespace std;

namespace icedcode
{

  nosy::nosy ()
  {
    __warn=true;
    __normal=false;
    struct timeval tv;
    gettimeofday(&tv, NULL);

    __generator.seed (1000000*tv.tv_sec+tv.tv_usec);
  }

  nosy::~nosy ()
  {
  }

  void nosy::set_noses (const double_2d &limits_)
  {
    if (!__check_noses(limits_))
      exit (1);

    set_limits(limits_);

    __add_nose(limits_.size ());

    __normal=false;

    user_work ();
  }

  void nosy::set_noses (const double_1d& sigma_)
  {
    __sigma=sigma_;
    __normal=true;
    __add_nose(sigma_.size ());
  }

  void nosy::__add_nose (unsigned int nb_)
  {
    for (unsigned int nb=0; nb<nb_; nb++)
      {
        nose tmp (this);
        tmp.set_amoeba(this);
#ifndef USE_NPROCESS
        __noses.push_back(tmp);
#endif
      }
  }

  bool nosy::user_work ()
  {
    bool has_changed = false;
#ifdef USE_NPROCESS
    cout << "try to launch them " << endl;
    icedcode::NProcess::GetIt()->ProcessAll();
#else
    for (list<nose>::iterator nit=__noses.begin();
         nit!=__noses.end();
         nit++)
      {
        (*nit).Process ();
      }

#endif

    has_changed = (*__noses.begin()).AnyHasChanged ();
    (*__noses.begin ()).Reset ();
    return has_changed;

  }

  bool nosy::__check_noses (const double_2d &limits_) const
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
    if (__warn && limits_.size () > PQR_size ())
      clog << "WARNING: There is a lot of noses. It could burden the amoeba.\n";

    return true;

  }

  nosy::nose::nose (nosy* nose_)
  {
    __nosy=nose_;
    __total_noses++;
    __id = __total_noses;
  }

  void nosy::nose::Process ()
  {
    cout << NProcess::GetIt ()->GetNbRunning () << " running" << endl;
    double_1d pos;
    if (__nosy->__normal && __nosy->__sigma.size ())
      {
        for (double_1d_cit lmean=__nosy->get_PQR(2).begin(); lmean!=__nosy->get_PQR(2).end (); lmean++)
          {
            normal_distribution<double>::param_type p(*lmean, __nosy->__sigma[__id]);
            __nosy->__normal_distribution.param(p);
            double val= __nosy->__normal_distribution (__nosy->__generator);
            pos.push_back (val);
          }
      }
    else
      for (double_2d_cit lit=__nosy->get_limits().begin(); lit!=__nosy->get_limits().end(); lit++)
        {
          double range=(*lit).back()-(*lit).front();
          double min=(*lit).front();
          double val;
          std::uniform_real_distribution<double>::param_type p(min, range);
          __nosy->__uniform_distribution.param (p);
          val= __nosy->__uniform_distribution (__nosy->__generator);
          pos.push_back (val);
        }
    set_pos(pos);
    double rnit=get_r ();
    double rPQR2=__nosy->get_PQR(2).get_r();
    double rPQR0=__nosy->get_PQR(0).get_r();
    if ( fabs(rnit - rPQR2) > fabs(rPQR2-rPQR0) &&
         ((get_value()<__nosy->get_PQR(2).get_value () ||
           __nosy->accept (*this,__nosy->get_PQR(2)))))
      {
        __nosy->set_PQR(*this,2);
        __has_been_changed|=true;
      }
  }


  size_t nosy::nose::__total_noses = 0;
  bool nosy::nose::__has_been_changed = 0;
  nosy* nosy::nose::__nosy;
}
