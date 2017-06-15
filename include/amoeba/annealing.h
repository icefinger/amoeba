#ifndef _annealing_
#define _annealing_

#include <nosy.h>

class annealing : public nosy
{
 public:
  annealing (double min_=-1., double max_=-1);
  virtual ~annealing ();

  double get_temperature () const {return __temperature;}
  void set_temperature (const double& t_) {__temperature=t_;}
  double get_decrease () const {return __decrease_fact;}
  void set_decrease (const double& d_) {__decrease_fact=d_;}

 private:
  void __evol_temperature (double val1_, double val2_);

 private:
  void __init_annealing ();
  bool accept (double, double);

 private:
  bool __evol;
  double __temperature;
  double __decrease_fact;
  unsigned int __heat_counter;
  double __min;
  double __max;
  bool __init;

  std::default_random_engine __generator;
  std::normal_distribution<double> __normal_distribution;
  std::uniform_real_distribution<double> __uniform_distribution;

};

#endif
