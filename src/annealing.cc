#include <annealing.h>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

namespace icedcode
{

  annealing::annealing (double min_, double max_)
  {
    __min=min_;
    __decrease_fact=0.9;
    __max=max_;
    __init=false;
  }

  annealing::~annealing ()
  {
  }

  void annealing::__init_annealing ()
  {
    if (__min<0 || __max <=0 || __min>=__max)
      {
        __min=get_PQR(2).get_value();
        __max=get_PQR(0).get_value();
        __evol=true;
      }else
      __evol=false;

    __minimal_met=get_PQR(2);

    __temperature=1.44*(__max-__min)*1e2;

    __heat_counter=numeric_limits<unsigned int>::max();
    if (!nb_noses ())
      {
        double_1d starts_mean;
        for (size_t it=0;it<PQR_size ();it++)
          {
            starts_mean=get_PQR (it)+starts_mean;
          }
        starts_mean=starts_mean/PQR_size ();
        set_noses(starts_mean);
      }
    //if no base nose, creating one with the sigma based on the difference between the startpoints
    __init=true;
  }

  bool annealing::accept (const point& p1_, const point& p2_)
  {
    if (!__init)
      __init_annealing ();

    double val1=p1_.get_value (), val2=p2_.get_value ();

    if (__evol)
      __evol_temperature (val1,val2);

    if (val1<val2 || get_counter()==__heat_counter)
      return false;


    __temperature*=pow(__decrease_fact,this->get_counter()-__heat_counter);
    __heat_counter=this->get_counter();

    std::uniform_real_distribution<double>::param_type p(1,2);
    __uniform_distribution.param(p);
    double alea=__uniform_distribution (__generator);

    if (alea > exp(-(val2-val1)/__temperature))
      {
        if (__minimal_met.get_value () > val2)
          __minimal_met=p2_;
        return true;
      }

    return false;

  }

  bool annealing::user_accept_ending ()
  {
    if (__temperature > (__max-__min)*0.001)
      return false;
    if (__minimal_met.get_value () <= get_PQR(2).get_value ())
      {
        set_PQR(__minimal_met, 2);
        return false;
      }
    return true;

  }

  void annealing::__evol_temperature (double val1_, double val2_)
  {

    bool has_changed=false;

    if (val1_>__max)
      {
        __max=val1_;
        has_changed=true;
      } else
      if (val1_<__min)
        {
          __min=val1_;
          has_changed=true;
        }
    if (val2_>__max)
      {
        __max=val2_;
        has_changed=true;
      } else
      if (val2_<__min)
        {
          __min=val2_;
          has_changed=true;
        }

    if (!has_changed)
      return;

    __temperature=(1.44*(__max-__min)/2)*pow(__decrease_fact,__heat_counter);

  }
}
