#ifndef _nosy_
#define _nosy_

#include <amoeba.h>
#include <random>


class nosy : public amoeba
{
 public:
  nosy ();
  virtual ~nosy ();

  void add_nose (const double_2d& limits_, unsigned int nb_=1);
  void add_nose (double *sigma_, unsigned int nb_=1);
  unsigned int nb_noses () const {return __noses.size();}
  void rm_noses () {__noses.clear ();}
  void set_warn (bool w_) {__warn=w_;}

 private:
  void __add_nose (unsigned int nb_);
  virtual double get_value (const double_1d&) = 0;
  bool __check_noses (const double_2d& limits_, unsigned int nb_) const;
  void user_work ();
  virtual bool accept (double, double) {return false;}

 private:
  std::list<point> __noses;
  bool __normal;
  bool __warn;
  double* __sigma;
 protected:
  std::default_random_engine __generator;
  std::normal_distribution<double> __normal_distribution;
  std::uniform_real_distribution<double> __uniform_distribution;
};

#endif
