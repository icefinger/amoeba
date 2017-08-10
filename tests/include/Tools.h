#ifndef __ICEDCODE_TOOLS__
#define __ICEDCODE_TOOLS__

//a static function for the draw and amoeba
double global_get_value (const double* x_, const double* par_)
{
  return (-100*(exp(-((x_[0]*x_[0]-4))/7200)+exp(-((x_[1]*x_[1]+4))/7200)-sin (x_[0]/20)-sin(x_[1]/20)));
}


#endif
