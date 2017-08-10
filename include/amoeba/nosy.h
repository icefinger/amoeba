/**
 * @file   nosy.h
 * @author Christophe Hugon <hugon@cppm.in2p3.fr>
 * @date   Fri Jun 16 17:16:01 2017
 *
 * @brief  Random scanning method to avoid local minimums.
 *
 *
 */

#ifndef _ICEDCODE_NOSY_
#define _ICEDCODE_NOSY_

#include <amoeba.h>
#include <random>

#ifdef USE_NPROCESS
#include <NProcess.h>
#endif

namespace icedcode
{
/**
 * \class nosy
 * \brief Improved minimization class. It uses the amoeba method plus a random scanning of the map.
 * It avoids to fall in big local minimum. 2 scanning are available: flat distribution over the full map, and normal distribution around the current minimal point. Each nose can have its own sigma.
 */
  class nosy : public amoeba
  {
  protected:
    class nose;
  public:
    nosy ();
    virtual ~nosy ();

    virtual void set_noses (const double_2d& limits_); /**< Putting the limit of scanning zone for a flat random distribution. By default the same as the map limits. The size of the double_2d define the number of noses and the 2 value of the contained double_1d define the min and max */
    virtual void set_noses (const double_1d&); /**< Put noses for normal distribution around the minimum point. The size of the double_1d define the number of noses, the values are the sigmas of each one. */
    size_t nb_noses () const;
    void rm_noses ();
    void set_warn (bool w_) {__warn=w_;}

    const std::vector<const point*>& get_saved_noses () const {return __saved_noses;}
  private:
    virtual void __add_nose (unsigned int nb_);
    virtual double get_value (const double_1d&) = 0;
    virtual bool __check_noses (const double_2d& limits_) const;
    virtual bool user_work ();
    //bool __one_nose_work (nose& nose_, size_t sit_);

  protected:
    /**
     * \class nose
     * \brief scanner class. Use the NProcess syntax for multi-threading.
     *
     */
    class nose: public point
#ifdef USE_NPROCESS
    ,public NProcess::Object
#endif
  {
  public:
    nose (nosy* nosy_ = NULL);
    ~nose () {};
    void Reset () {__has_been_changed = false;}
    bool AnyHasChanged () {return __has_been_changed;}
    void Process ();
    void Run () {}
    void SetSigma (double sigma_) {__sigma = sigma_;}
    double GetSigma () const {return __sigma;}

    nose& operator=(const nose&);

  protected:
    /**
     * virtual function that can be overloaded by the user to decide if the point is kept among the PQR, even if it's non smaller than the other values.
     *
     *
     * @return
     */
    virtual bool user_work (const point&, const point&) {return false;}
  protected:
    size_t        __id = 0;
    double        __sigma = -1;
    static size_t __total_noses;
    static bool   __has_been_changed;
    static nosy*  __nosy;

    std::default_random_engine __generator;
    std::normal_distribution<double> __normal_distribution;
    std::uniform_real_distribution<double> __uniform_distribution;

  };

  void __save_noses ();

  std::list<nose*> __noses;
  std::vector<const point*> __saved_noses;
  bool __normal;
  bool __warn;
  double_1d __sigma;
  };
}
#endif
