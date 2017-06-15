#include <amoeba.h>
#include <limits>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstdarg>

using namespace std;

amoeba::amoeba ()
{
  __delta_val=1e-5;
  __delta_pos=1e-5;
  __extrapol=2.;
  __contract=2.;
  __debug=false;
  __has_limits=false;
  __counter=0;
  __init=false;
  __PQRsize=0;
}

amoeba::~amoeba ()
{
  if (__init)
    delete [] __PQR;
}

void amoeba::set_limit (double min_, double max_, unsigned int pos_)
{

  if (pos_>__limits.size())
    return;

  double_2d_it d2it = __limits.begin();
  for (unsigned int it=0; it<pos_; it++)
    d2it++;
  (*d2it).front()=min_;
  (*d2it).back()=max_;
  __has_limits=true;

}

void amoeba::set_limits (const double_2d& limits_)
{

  if (limits_.size() == __limits.size())
    __limits=limits_;
  __has_limits=true;
}

void amoeba::set_PQR (const point& p_, unsigned int pos_)
{
  __PQR[pos_]=p_;
  __check_limits ();
  __classify();
}

void amoeba::set_starts (const double_2d& starts_)
{

  if(! __check_starts (starts_))
    exit (1);

  __init_PQR(starts_);

}

amoeba::point amoeba::find_min ( )
{
  __counter=0;

  while (  (__PQR[0].get_value () - __PQR[2].get_value () > __delta_val &&
	    fabs(__PQR[0].get_r () - __PQR[2].get_r ()) > __delta_pos) &&
	   __counter<1000*__PQRsize)
    {
      point M_QR=__reflectP ();

      if (__PQR[0]< __PQR[2])
	{
	  __extrapolateP (M_QR);
	}
      if (__PQR[0] >= __PQR[1])
	{
	  double tmp=__PQR[0].get_value ();
	  __contractP(M_QR);
	  if (__PQR[0].get_value ()>tmp)
	    {
	      __contractToR ();
	    }

	}
      __classify();
      user_work ();
      //TODO
      // if (user_work ())
      //   __classify();

      __counter++;

    }

  if (__debug)
    clog << "Needed "<< __counter << " steps.\n";

  return __PQR[2];

}

bool amoeba::__check_starts(const double_2d &starts_) const
{
  unsigned int feet=starts_.size();
  unsigned int dim=(*(starts_.begin())).size();
  if (dim>feet+1)
    {
      cerr << "ERROR:amoeba::find_min: needs at least Ndim+1 starts.\n";
      return false;
    }
  for (double_2d_cit sit=starts_.begin(); sit!=starts_.end(); sit++)
    {
      if ((*sit).size()!=dim)
	{
	  cerr << "ERROR:amoeba::find_min: unconstant dim in starts."<<dim << ' ' << (*sit).size()<<"\n";
	  return false;
	}
    }

  return true;

}

void amoeba::__classify ()
{

  bool has_next=true;

  while (has_next)
    {
      has_next=false;
      point ptmp;
      for (unsigned int pqrit=0; pqrit<__PQRsize; pqrit++)
	{
	  if (__PQR[pqrit].get_value() >__PQR[0].get_value())
	    {
	      ptmp=__PQR[0];
	      __PQR[0]=__PQR[pqrit];
	      __PQR[pqrit]=ptmp;
	      has_next=true;
	    }else if (pqrit!=0 && __PQR[pqrit].get_value() >__PQR[1].get_value())
	    {
	      ptmp=__PQR[1];
	      __PQR[1]=__PQR[pqrit];
	      __PQR[pqrit]=ptmp;
	      has_next=true;
	    }
	  if (__PQR[pqrit].get_value() <__PQR[2].get_value())
	    {
	      ptmp=__PQR[2];
	      __PQR[2]=__PQR[pqrit];
	      __PQR[pqrit]=ptmp;
	      has_next=true;
	    }
	}
    }

  return;

}

void amoeba::__check_limits ()
{
  if(!__has_limits)
    return;

  for (unsigned int PQRit=0; PQRit<__PQRsize; PQRit++)
    {
      double_1d pos;
      bool has_changed=false;
      double_1d_cit PQRposit=__PQR[PQRit].begin();
      for (double_2d_cit d2it=__limits.begin();
	   d2it!=__limits.end() && PQRposit!=__PQR[PQRit].end();
	   d2it++)
	{
	  const double_1d& limits=*d2it;
	  const double& val=*PQRposit;

	  if (val < limits.front())
	    {
	      pos.push_back(limits.front());
	      has_changed=true;
	    }
	  else if (val > limits.back())
	    {
	      pos.push_back(limits.back());
	      has_changed=true;
	    }
	  else
	    {
	      pos.push_back (val);
	    }
	  PQRposit++;
	}
      if (has_changed)
	__PQR[PQRit].set_pos(pos);}

}

void amoeba::__contractToR ( )
{

  for (unsigned int pqrit=0; pqrit<__PQRsize; pqrit++)
    {
      if (pqrit==2)
	continue;

      __PQR[pqrit]=(__PQR[pqrit]-__PQR[2])/__contract + __PQR[2];

    }
  __check_limits();
  __classify ();

  return;

}

void amoeba::__contractP (point &M_QR_)
{
  __PQR[0]=(__PQR[0]-M_QR_)/__contract+M_QR_;
  __check_limits ();
  __classify ();
}

void amoeba::__extrapolateP (point &M_QR_)
{

  __PQR[0]=(__PQR[0]-M_QR_)*__extrapol+M_QR_;
  __check_limits ();
  __classify ();

}

amoeba::point amoeba::__reflectP ()
{

  point Mpoint=__PQR[1];

  for (unsigned int pqrit=2; pqrit<__PQRsize; pqrit++)
    {
      Mpoint=Mpoint+__PQR[pqrit];
    }
  Mpoint=Mpoint/(__PQRsize-1);


  __PQR[0]=(Mpoint*2)-__PQR[0];


  return Mpoint;


}

void amoeba::__init_PQR (const double_2d& starts_)
{

  if (__init)
    {
      delete [] __PQR;
      __PQRsize=0;
      __limits.clear();
      __counter=0;
      __has_limits=false;
    }
  __PQRsize=starts_.size();
  if (__PQRsize==2)
    __PQRsize++;
  __PQR=new point[__PQRsize];
  int countit=0;
  for (double_2d_cit sit=starts_.begin(); sit!=starts_.end(); sit++)
    {
      __PQR[countit].set_amoeba(this);
      __PQR[countit].set_pos (*sit);
      countit++;
    }

  double_1d dtmp (2,numeric_limits<double>::min(), numeric_limits<double>::max());

  for (unsigned int it=0; it<starts_.front().size(); it++)
    __limits.push_back (dtmp);


  if (starts_.size() == 2)
    __PQR[2]=__PQR[1];
  __classify ();

  __init=true;

  return;
}

amoeba::double_1d::double_1d(int n_, ...)
{
  va_list vl;
  va_start(vl,n_);

  for (int i=0;i<n_;i++)
    {
      double val=va_arg(vl,double);
      push_back (val);
    }
  va_end(vl);
}

void amoeba::double_1d::set_pos(int n_, ...)
{
  clear ();
  va_list vl;
  va_start(vl,n_);

  for (int i=0;i<n_;i++)
    {
      double val=va_arg(vl,double);
      push_back (val);
    }
  va_end(vl);
}

double amoeba::double_1d::get_r () const
{
  double sum=0;
  for (auto val: *this)
    sum+=val*val;
  return sqrt(sum);
}

amoeba::double_1d amoeba::double_1d::operator+ (const double_1d& to_add_) const
{
  double_1d toreturn;
  if (size () != to_add_.size ())
    {
      cerr << "try to add double_1d with different dim.\n";
      return toreturn;
    }
  toreturn=*this;
  for (size_t it=0; it<to_add_.size (); it++)
    toreturn[it]+=to_add_[it];
  return toreturn;
}

amoeba::double_1d amoeba::double_1d::operator- (const double_1d& to_sub_) const
{
  double_1d toreturn;
  if (size () != to_sub_.size ())
    {
      cerr << "try to subtract double_1d with different dim.\n";
      return toreturn;
    }
  toreturn=*this;
  for (size_t it=0; it<to_sub_.size (); it++)
    toreturn[it]-=to_sub_[it];
  return toreturn;
}

amoeba::double_1d amoeba::double_1d::operator* (double factor_) const
{
  double_1d toreturn=*this;
  for (size_t it=0; it<size (); it++)
    toreturn[it]*=factor_;
  return toreturn;
}

amoeba::double_1d amoeba::double_1d::operator/ (double divisor_) const
{
  double_1d toreturn=*this;
  for (size_t it=0; it<size (); it++)
    toreturn[it]/=divisor_;
  return toreturn;
}

bool amoeba::double_1d::operator< (const double_1d& to_comp_) const
{
  return get_r ()<to_comp_.get_r ();
}

bool amoeba::double_1d::operator> (const double_1d& to_comp_) const
{
  return get_r ()>to_comp_.get_r ();
}

bool amoeba::double_1d::operator<= (const double_1d& to_comp_) const
{
  return get_r ()<=to_comp_.get_r ();
}

bool amoeba::double_1d::operator>= (const double_1d& to_comp_) const
{
  return get_r ()>=to_comp_.get_r ();
}

bool amoeba::double_1d::operator== (const double_1d& to_comp_) const
{
  if (size () != to_comp_.size ())
    {
      return false;
    }

  for (size_t it=0; it<size (); it++)
    {
      if (this->at(it) != to_comp_[it])
        return false;
    }
  return true;
}
amoeba::double_2d::double_2d(int n_, ...)
{
  va_list vl;
  va_start(vl,n_);
  int dim=va_arg(vl,int);

  for (int i=0;i<n_;i++)
    {
      double_1d topush;
      for (int dimit=0; dimit<dim; dimit++)
        {
          double value = va_arg(vl,double);
          topush.push_back (value);
        }
      push_back (topush);
    }
  va_end(vl);
}

amoeba::point::point (bool min_max_)
{
  __am=NULL;
  if (min_max_)
    __val=numeric_limits<double>::min();
  else
    __val=numeric_limits<double>::max();
}

void amoeba::point::set_pos (const double_1d& pos_)
{
  const double_1d& doubleThis=*((const double_1d*)this);

  if (doubleThis==pos_)
    return;
  *this=pos_;
  if (__am) __val=__am->get_value ( pos_ );
}
void amoeba::point::reinit (bool min_max_)
{
  if (min_max_)
    __val=numeric_limits<double>::min();
  else
    __val=numeric_limits<double>::max();
}

double amoeba::point::get_r () const
{
  double tmp=0;
  for (auto val: *this)
    {
      tmp+=val*val;
    }
  return sqrt(tmp);
}

bool amoeba::point::operator> (const point& to_oper_) const
{
  return (__val>to_oper_.__val);
}

amoeba::point& amoeba::point::operator= (const double_1d& to_copy_)
{
  (*(double_1d*)(this))=to_copy_;
  if (__am) __val=__am->get_value ( to_copy_);
  return *this;
}

bool amoeba::point::operator>= (const point& to_oper_) const
{
  return (__val>=to_oper_.__val);
}

bool amoeba::point::operator<= (const point& to_oper_) const
{
  return (__val<=to_oper_.__val);
}

bool amoeba::point::operator< (const point& to_oper_) const
{
  return (__val<to_oper_.__val);
}

bool amoeba::point::operator==(const point& to_oper_) const
{
  const double_1d& doubleThis=*((const double_1d*)this);
  const double_1d& doubleOper=*((const double_1d*)&to_oper_);
  return (doubleThis == doubleOper && __val == to_oper_.__val);
}


std::ostream& operator<<(std::ostream &oss_, const amoeba::double_1d& d1_)
{
  oss_ << "(";
  size_t it=0;
  for (; it<d1_.size ()-1; it++)
    oss_<<d1_[it]<<",";
  oss_<<d1_[++it]<<")";
  return oss_;
}

std::ostream& operator<<(std::ostream &oss_, const amoeba::point& d1_)
{
  oss_ << *((const amoeba::double_1d*)&d1_);
  oss_ << "="<<d1_.get_value ();
  return oss_;
}
