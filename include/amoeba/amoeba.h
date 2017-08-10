/**
 * @file   amoeba.h
 * @author Christophe Hugon <hugon@cppm.in2p3.fr>
 * @date   Fri Jun 16 16:33:17 2017
 *
 * @brief  Fast minimization algorithm. It avoid most of local minimum thanks to it's multipoint algorithm. It's main feature is to minimize at the maximum the number of calculations.
 *
 *
 */
#ifndef _AMOEBA_
#define _AMOEBA_

#include <list>
#include <vector>
#include <iostream>

namespace icedcode {

  /**
   * \class amoeba
   * \brief main basic class. It's used by inheritence. The reasearch domain here is called a map and is not limited in dimension. The calculus is based on three main points, PQR, which are the two minimus and the maximum among the tested values.
   */
  class amoeba {
  public:
    /**
     * \class double_1d
     * \brief  basic multi-dimensional vector. It's generally used for coordinates. It contains most of the basic needed operations
     */
    class double_1d: public std::vector<double>
    {
    public:
      double_1d () {}
      double_1d (int n_, ...);   /**< Constructor using as arguments (dimension, val1, val2...) */
      ~double_1d () {}

      void set_pos (int n_, ...); /**< Replacing the current values using (dimension, val1, val2...) */

      double get_r () const;     /**< distance from the origin (0,0...) */

      double_1d operator+ (const double_1d&) const;
      double_1d operator- (const double_1d&) const;
      double_1d operator/(double) const;
      double_1d operator*(double) const;
      bool operator<(const double_1d&) const; /**< comparing the the distance (get_r)*/
      bool operator>(const double_1d&) const; /**< comparing the the distance (get_r)*/
      bool operator<=(const double_1d&) const; /**< comparing the the distance (get_r)*/
      bool operator>=(const double_1d&) const; /**< comparing the the distance (get_r)*/
      bool operator==(const double_1d&) const;

    };

  /**
   * \class double_2d
   * \brief bi-dimentional vector. Can be illimited in dimension
   */

  class double_2d: public std::vector<double_1d >
  {
  public:
    double_2d () {}
    double_2d (int n_, ...);   /**< constructor using (dimension1, dimension2,val11,val12,val21..). */
    ~double_2d () {}
  };

  //some useful typedef
  typedef std::vector<double>::iterator double_1d_it;
  typedef std::vector<double_1d >::iterator double_2d_it;

  typedef std::vector<double>::const_iterator double_1d_cit;
  typedef std::vector<double_1d >::const_iterator double_2d_cit;

  /**
   * \class point
   * \brief a double_1d with an output value. Used for the results.
   */
  class point: public double_1d
  {
  public:
    virtual ~point () {}
    point (bool min_max_=true);
    point (const point& pt_);
    void set_pos (const double_1d&); /**< Putting pos using directly the mother class */
    double get_value () const {return __val;} /**< Getting the current value. Calculated when the coordinate are changed. */
    void reinit (bool min_max_=false);
    void set_amoeba(amoeba* am_) {__am=am_;} /**< Putting the current main class that does the calculations for the value. */


    point& operator=(const double_1d&);
    bool operator<(const point&) const; /**< comparing the value instead of the distance */
    bool operator>(const point&) const; /**< comparing the value instead of the distance */
    bool operator<=(const point&) const; /**< comparing the value instead of the distance */
    bool operator>=(const point&) const; /**< comparing the value instead of the distance */
    bool operator==(const point&) const;

  private:
    double __val;
    amoeba* __am;

  };

  public:
  amoeba ();
  virtual ~amoeba ();

  point find_min ( );            /**< The method to call to get the minimal value. */

  void set_starts (const double_2d& starts_); /**< start coordinate for the research of minimal value. the double_2d is a collection of double_1d with the dimension of the map. starts_ should contain the same number of coordinates than the map dimension with a minimum of 3.*/
  void set_delta (double delta_val_, double delta_pos_) {__delta_pos=delta_pos_;__delta_val=delta_val_;} /**< delta_val_ limit the research on the difference of the calculate value. delta_pos_ limit it on the distance between PQR. */
  void set_extrapol (double extra_val_) {__extrapol=extra_val_;} /**< Modify the extrapolation value used when the amoeba tries to expand (2 by default)*/
  void set_contract (double con_val_) {__contract=con_val_;} /**< Modify the contraction value used when the amoeba tries to reduce itself (2 by default) */
  void set_debug (bool debug_) {__debug=debug_;}
  void set_limit (double min_, double max_, unsigned int pos_); /**< Put map limit for the dimension pos_ */
  void set_limits (const double_2d& limits_); /**< Put the limits to the map. limits_ is in the dimension of the map, containing double_1d with the minimal and maximal values. */
  unsigned int get_counter () const {return __counter;}
  const double_2d& get_limits () const {return __limits;};

  void set_save_steps (bool b_=true) {__save_steps=b_;}
  bool get_save_steps () const {return __save_steps;}
  const std::vector<const point*>& get_saved_steps () const {return __saved_steps_list;}
  void flush_saved_steps ();
  protected:
  void set_PQR (const point& p_, unsigned int pos_);
  unsigned int PQR_size () const {return __PQRsize;}
  const point& get_PQR (unsigned int pos_) const {return __PQR[pos_];}

  private:
  virtual double get_value (const double_1d&) = 0;
  void __init_PQR (const double_2d& starts_);
  point __reflectP ();
  void __extrapolateP (point&);
  void __contractP (point&);
  void __contractToR ();
  void __classify ();
  void __check_limits ();
  bool __check_starts (const double_2d &starts_) const;
  void __save_current_step ();

  //for inerited classes. For complementary works in find_min
  virtual bool user_work () {return false;}
  //for checking if it goes on with find mins
  virtual bool user_accept_ending () {return true;}

  private:
  double __delta_val;
  double __delta_pos;
  double __extrapol;
  double __contract;
  unsigned int __counter;
  bool __debug;
  bool __has_limits;
  bool __init;
  bool __save_steps = false;
  point* __PQR;
  unsigned __PQRsize;
  double_2d __limits;

  std::vector<const point*> __saved_steps_list;

  };

}
std::ostream& operator<<(std::ostream &oss_, const icedcode::amoeba::double_1d& d1_);
std::ostream& operator<<(std::ostream &oss_, const icedcode::amoeba::point& d1_);


#endif
