#include <Drawer.h>


#include <TGraph.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF2.h>

#include <vector>

using namespace std;


namespace icedcode
{
  TGraph* get_PQR_graph (const amoeba& am_, size_t step_)
  {
    const vector<const amoeba::point*> &saved_points = am_.get_saved_steps ();
    if (step_ >= saved_points.size ())
      return 0;

    TGraph *graph = new TGraph;
    graph->SetLineWidth (2);
    size_t i=0;

    const auto& it = saved_points[step_];
    graph->SetPoint (i++, it[0][0],it[0][1]);
    graph->SetPoint (i++, it[1][0],it[1][1]);
    graph->SetPoint (i++, it[2][0],it[2][1]);
    graph->SetPoint (i++, it[0][0],it[0][1]);

    return graph;
  }

  TGraph* get_noses_graph (const nosy& no_, size_t step_)
  {
    const vector<const amoeba::point*>& saved_noses = no_.get_saved_noses ();
    if (step_ >= saved_noses.size ())
      return 0;

    TGraph *graph = new TGraph;

    for (size_t i=0; i<no_.nb_noses (); i++)
      {
        const auto& it = saved_noses[step_];
        graph->SetPoint (i, it[i][0],it[i][1]);
      }
    return graph;


  }

}
