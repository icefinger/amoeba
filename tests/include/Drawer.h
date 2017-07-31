#ifndef __ICEDCODE_DRAWER__
#define __ICEDCODE_DRAWER__
#ifdef ENABLE_ROOT

#include <annealing.h>
#include <TGraph.h>

namespace icedcode
{

  TGraph* get_PQR_graph (const amoeba& am_, size_t step_);
  TGraph* get_noses_graph (const nosy& am_, size_t step_);
}

#endif
#endif
