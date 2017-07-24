#include <annealing.h>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

namespace icedcode
{

  annealing::annealing (double temperature_)
  {
    __global_temperature=temperature_;
    __global_decrease_fact=0.9;
     __init=false;
  }

  annealing::~annealing ()
  {
  }

  void annealing::__init_annealing ()
  {
    if (__global_temperature < 0)
      {
        double min=get_PQR(2).get_value();
        double max=get_PQR(0).get_value();
        __global_temperature = max - min;
        __evol=true;
      }else
      __evol=false;

    __minimal_met=get_PQR(2);

    __global_temperature=1.44*(__global_temperature);

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


  bool annealing::user_accept_ending ()
  {
    if (__mean_temperature > (__global_temperature)*0.001)
      return false;
    if (__minimal_met.get_value () <= get_PQR(2).get_value ())
        set_PQR(__minimal_met, 2);

    return true;

  }

  double annealing::annealing::__mean_temperature = 0;
  amoeba::point  annealing::annealing::__minimal_met;



  annealing::hot_nose::hot_nose (nosy* nosy_, double temperature_) : annealing::nose::nose (nosy_)
  {
    __temperature = temperature_;
    __nb_hot_noses++;
  }

  bool annealing::hot_nose::user_work (const point& p1_, const point& p2_)
  {
    if (((annealing*)__nosy)->__init)
      ((annealing*)__nosy)->__init_annealing ();

    double current_temperature = __temperature;

    double val1=p1_.get_value (), val2=p2_.get_value ();

    if (__evol)
      __evol_temperature (val1,val2);

    __temperature*=__decrease_fact;

    if (__id == 0)
      __mean_temperature = 0;

    __mean_temperature += __temperature/__nb_hot_noses;

    std::uniform_real_distribution<double>::param_type p(1,2);
    __uniform_distribution.param(p);
    double alea=__uniform_distribution (__generator);

    if (alea > exp(-(val2-val1)/current_temperature))
      {
        if (__minimal_met.get_value () > val2)
          __minimal_met=p2_;
        return true;
      }

    return false;

  }

  void annealing::hot_nose::__evol_temperature (double val1_, double val2_)
  {
    double new_temperature = fabs (val1_ - val2_)*1.44;

    if (new_temperature > __temperature)
      {
        __temperature = new_temperature;
      }
  }

  size_t annealing::hot_nose::__nb_hot_noses = 0;

}
