/**
 * @file   annealing.h
 * @author Christophe Hugon <hugon@cppm.in2p3.fr>
 * @date   Fri Jun 16 17:14:15 2017
 *
 * @brief  Annealing process file.
 *
 *
 */

#ifndef _ICEDCODE_ANNEALING_
#define _ICEDCODE_ANNEALING_

#include <nosy.h>

namespace icedcode
{

  /**
   * \class annealing
   * \brief Anneling method on the amoeba. It allows the current minimal point to escape from a small local minimal, the algorithm is "cooling down" slowly, and becom a normal amoeba with one nose when it's cold.
   */
  class annealing : public nosy
  {
  public:
    annealing (double temperature_ = -1); /**< constructor with an estimation of minimal and maximal value. The temperature is then calculated to optimize the probability to escape from a local minimum without slowing down too much the full calculus. If no value are put the temperature is estimated with the starting points and can evolve in function of minimum or maximum that the amoeba find out. */
    virtual ~annealing ();

    void set_global_temperature (double &t_);

    class hot_nose: public nose
    {
    public:
      hot_nose (nosy* nosy_, double temperature_=-1);
      ~hot_nose () {}

      double get_temperature () const {return __temperature;}
      void set_temperature (const double& t_) {__temperature=t_;} /**< Manually force a temperature. */
      double get_decrease () const {return __decrease_fact;}
      void set_decrease (const double& d_) {__decrease_fact=d_;} /**< Change the speed of cooling down */

    private:
      void __evol_temperature (double val1_, double val2_);

    private:
      bool user_work (const point&, const point&);

    private:
      bool __evol;
      double __temperature;
      double __decrease_fact;
      static size_t __nb_hot_noses;

      std::default_random_engine __generator;
      std::normal_distribution<double> __normal_distribution;
      std::uniform_real_distribution<double> __uniform_distribution;
    };

  private:
    virtual void __add_nose (unsigned int nb_);
    void __init_annealing ();
    virtual bool user_accept_ending ();
    double __global_temperature = -1;
    static double __mean_temperature;
    double __global_decrease_fact;
  private:
    bool __init;
    bool __evol;
    static point __minimal_met;

  };
}
#endif
