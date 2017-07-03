/**
 * @file   nosy.h
 * @author Christophe Hugon <hugon@cppm.in2p3.fr>
 * @date   Fri Jun 16 17:16:01 2017
 *
 * @brief  Random scanning method to avoid local minimums.
 *
 *
 */

#ifndef _nosy_
#define _nosy_

#include <amoeba.h>
#include <random>

namespace icedcode
{
/**
 * \class nosy
 * \brief Improved minimization class. It uses the amoeba method plus a random scanning of the map.
 * It avoids to fall in big local minimum. 2 scanning are available: flat distribution over the full map, and normal distribution around the current minimal point. Each nose can have its own sigma.
 */
  class nosy : public amoeba
  {
  public:
    nosy ();
    virtual ~nosy ();

    void set_noses (const double_2d& limits_); /**< Putting the limit of scanning zone for a flat random distribution. By default the same as the map limits. The size of the double_2d define the number of noses and the 2 value of the contained double_1d define the min and max */
    void set_noses (const double_1d&); /**< Put noses for normal distribution around the minimum point. The size of the double_1d define the number of noses, the values are the sigmas of each one. */
    unsigned int nb_noses () const {return __noses.size();}
    void rm_noses () {__noses.clear ();}
    void set_warn (bool w_) {__warn=w_;}

  private:
    void __add_nose (unsigned int nb_);
    virtual double get_value (const double_1d&) = 0;
    bool __check_noses (const double_2d& limits_) const;
    bool user_work ();
    virtual bool accept (const point&, const point&) {return false;}

  private:
    std::list<point> __noses;
    bool __normal;
    bool __warn;
    double_1d __sigma;
  protected:
    std::default_random_engine __generator;
    std::normal_distribution<double> __normal_distribution;
    std::uniform_real_distribution<double> __uniform_distribution;
  };
}
#endif
