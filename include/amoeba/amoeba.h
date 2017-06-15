#ifndef _AMOEBA_
#define _AMOEBA_

#include <list>
#include <vector>
#include <iostream>
//! je test amoeba
class amoeba {


 public:
  class double_1d: public std::vector<double> //put to protected then
   {
   public:
     double_1d () {}
     double_1d (int n_, ...);
     ~double_1d () {}

     void set_pos (int n_, ...);

     double get_r () const;

     double_1d operator+ (const double_1d&) const;
     double_1d operator- (const double_1d&) const;
     double_1d operator/(double) const;
     double_1d operator*(double) const;
     bool operator<(const double_1d&) const;
     bool operator>(const double_1d&) const;
     bool operator<=(const double_1d&) const;
     bool operator>=(const double_1d&) const;
     bool operator==(const double_1d&) const;


   };

 class double_2d: public std::vector<double_1d >
 {
   public:
     double_2d () {}
     double_2d (int n_, ...);
     ~double_2d () {}
 };

 typedef std::vector<double>::iterator double_1d_it;
 typedef std::vector<double_1d >::iterator double_2d_it;

 typedef std::vector<double>::const_iterator double_1d_cit;
 typedef std::vector<double_1d >::const_iterator double_2d_cit;


 class point: public double_1d
 {
 public:
   virtual ~point () {}
   point (bool min_max_=true);


   void set_pos (const double_1d&);
   double get_r () const ;
   double get_value () const {return __val;}
   void reinit (bool min_max_=false);
   void set_amoeba(amoeba* am_) {__am=am_;}
   void out ();

   point& operator=(const double_1d&);
   bool operator<(const point&) const;
   bool operator>(const point&) const;
   bool operator<=(const point&) const;
   bool operator>=(const point&) const;
   bool operator==(const point&) const;

 private:
   double __val;
   amoeba* __am;

 };

 public:
 amoeba ();
 virtual ~amoeba ();

 point find_min ( );

 void set_starts (const double_2d& starts_);
 void set_delta (double delta_val_, double delta_pos_) {__delta_pos=delta_pos_;__delta_val=delta_val_;}
 void set_extrapol (double extra_val_) {__extrapol=extra_val_;}
 void set_contract (double con_val_) {__contract=con_val_;}
 void set_debug (bool debug_) {__debug=debug_;}
 void set_limit (double min_, double max_, unsigned int pos_);
 void set_limits (const double_2d&);
 unsigned int get_counter () const {return __counter;}
 const double_2d& get_limits () const {return __limits;};
 protected:
 void set_PQR (const point& p_, unsigned int pos_);
 const point& get_PQR (unsigned int pos_) const {return __PQR[pos_];}
 unsigned int PQR_size () const {return __PQRsize;}

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
 virtual void user_work () {}

 private:
 double __delta_val;
 double __delta_pos;
 double __extrapol;
 double __contract;
 unsigned int __counter;
 bool __debug;
 bool __has_limits;
 bool __init;
 point* __PQR;
 unsigned __PQRsize;
 double_2d __limits;

};

std::ostream& operator<<(std::ostream &oss_, const amoeba::double_1d& d1_);
std::ostream& operator<<(std::ostream &oss_, const amoeba::point& d1_);


#endif
