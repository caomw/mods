#include "correspondencebank.hpp"
#include "synth_detection.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

namespace mods {

CorrespondenceBank::CorrespondenceBank()
{
}

int CorrespondenceBank::GetCorrespondencesNumber(std::string desc_name, std::string det_name)
{
  int reg_number = 0;
  std::map<std::string, CorrespondencesMap>::iterator desc_corr_it;
  CorrespondencesMap::iterator dets_it;

  if (desc_name.compare("All") == 0)
    {
      for (desc_corr_it = CorrespondencesMapMap.begin();
           desc_corr_it != CorrespondencesMapMap.end(); desc_corr_it++)
        if (det_name.compare("All") == 0)
          {
            for (dets_it = desc_corr_it->second.begin();
                 dets_it != desc_corr_it->second.end(); dets_it++)
              reg_number +=  dets_it->second.TCList.size();
          }
        else
          {
            dets_it = desc_corr_it->second.find(desc_name);
            if (dets_it != desc_corr_it->second.end() )
              reg_number +=  dets_it->second.TCList.size();

          }
    }
  else
    {
      desc_corr_it = CorrespondencesMapMap.find(desc_name);
      if ( desc_corr_it != CorrespondencesMapMap.end())
        {
          if (det_name.compare("All") == 0)
            {
              for (dets_it = desc_corr_it->second.begin();
                   dets_it != desc_corr_it->second.end(); dets_it++)
                reg_number +=  dets_it->second.TCList.size();
            }
          else
            {
              dets_it = desc_corr_it->second.find(desc_name);
              if (dets_it != desc_corr_it->second.end() )
                reg_number +=  dets_it->second.TCList.size();

            }
        }
    }
  return reg_number;
}
double CorrespondenceBank::GetSpentTime(std::string desc_name, std::string det_name)
{
  double time1 = 0.0;
  std::map<std::string, std::map<std::string, double> >::iterator desc_corr_it;
  std::map<std::string, double>::iterator dets_it;

  if (desc_name.compare("All") == 0)
    {
      for (desc_corr_it = MatchingTimeMapMap.begin();
           desc_corr_it != MatchingTimeMapMap.end(); desc_corr_it++)
        if (det_name.compare("All") == 0)
          {
            for (dets_it = desc_corr_it->second.begin();
                 dets_it != desc_corr_it->second.end(); dets_it++)
              time1 +=  dets_it->second;
          }
        else
          {
            dets_it = desc_corr_it->second.find(det_name);
            if (dets_it != desc_corr_it->second.end() )
              time1 +=  dets_it->second;

          }
    }
  else
    {
      desc_corr_it = MatchingTimeMapMap.find(desc_name);
      if ( desc_corr_it != MatchingTimeMapMap.end())
        {
          if (det_name.compare("All") == 0)
            {
              for (dets_it = desc_corr_it->second.begin();
                   dets_it != desc_corr_it->second.end(); dets_it++)
                time1 +=  dets_it->second;
            }
          else
            {
              dets_it = desc_corr_it->second.find(det_name);
              if (dets_it != desc_corr_it->second.end() )
                time1 +=  dets_it->second;

            }
        }
    }
  return time1;
}
cv::Mat CorrespondenceBank::GetDescriptorsDB(std::string desc_name)
{
  cv::Mat descs;
  std::map<std::string, cv::Mat>::iterator desc_corr_it = DescriptorsDBForSNN.find(desc_name);
  if ( desc_corr_it != DescriptorsDBForSNN.end())
    {
      descs = desc_corr_it->second;
    }
  return descs;
}

TentativeCorrespListExt CorrespondenceBank::GetCorresponcesVector(std::string desc_name, std::string det_name)
{
  unsigned int n_regs = GetCorrespondencesNumber(desc_name,det_name);

  TentativeCorrespListExt corrs;
  corrs.TCList.reserve(n_regs);

  std::map<std::string, CorrespondencesMap>::iterator desc_corr_it;
  CorrespondencesMap::iterator dets_it;
  if (desc_name.compare("All") == 0)
    {
      for (desc_corr_it = CorrespondencesMapMap.begin();
           desc_corr_it != CorrespondencesMapMap.end(); desc_corr_it++)
        if (det_name.compare("All") == 0)
          {
            for (dets_it = desc_corr_it->second.begin();
                 dets_it != desc_corr_it->second.end(); dets_it++)
              {
                TentativeCorrespListExt *currentDescVector = &(dets_it->second);
                for (unsigned int i = 0; i < currentDescVector->TCList.size(); i++)
                  corrs.TCList.push_back(currentDescVector->TCList[i]);
              }
          }
        else
          {
            dets_it = desc_corr_it->second.find(desc_name);
            if (dets_it != desc_corr_it->second.end() )
              {
                TentativeCorrespListExt *currentDescVector = &(dets_it->second);
                for (unsigned int i = 0; i < currentDescVector->TCList.size(); i++)
                  corrs.TCList.push_back(currentDescVector->TCList[i]);
              }
          }
    }
  else
    {
      desc_corr_it = CorrespondencesMapMap.find(desc_name);
      if ( desc_corr_it != CorrespondencesMapMap.end())
        {
          if (det_name.compare("All") == 0)
            {
              for (dets_it = desc_corr_it->second.begin();
                   dets_it != desc_corr_it->second.end(); dets_it++)
                {
                  TentativeCorrespListExt *currentDescVector = &(dets_it->second);
                  for (unsigned int i = 0; i < currentDescVector->TCList.size(); i++)
                    corrs.TCList.push_back(currentDescVector->TCList[i]);
                }
            }
          else
            {
              dets_it = desc_corr_it->second.find(det_name);
              if (dets_it != desc_corr_it->second.end() )
                {
                  TentativeCorrespListExt *currentDescVector = &(dets_it->second);
                  for (unsigned int i = 0; i < currentDescVector->TCList.size(); i++)
                    corrs.TCList.push_back(currentDescVector->TCList[i]);
                }
            }
        }
    }
  return corrs;
}
void CorrespondenceBank::AddCorrespondences(TentativeCorrespListExt& CorrsToAdd,std::string det_name, std::string desc_name)
{
  std::map<std::string, CorrespondencesMap>::iterator desc_corr_it;
  CorrespondencesMap::iterator dets_it;

  desc_corr_it = CorrespondencesMapMap.find(desc_name);
  if ( desc_corr_it != CorrespondencesMapMap.end())
    {
      dets_it = desc_corr_it->second.find(det_name);
      if (dets_it != desc_corr_it->second.end() )
        {
          TentativeCorrespListExt *currentDescVector = &(dets_it->second);
          CorrespondenceBank::AddCorrespondencesToList(*currentDescVector,CorrsToAdd);
        }
      else
        {
          desc_corr_it->second[det_name] = CorrsToAdd;
        }
    }
  else
    {
      std::map<std::string, TentativeCorrespListExt> new_desc;
      new_desc[det_name] = CorrsToAdd;
      CorrespondencesMapMap[desc_name] = new_desc;
    }
}
void CorrespondenceBank::ClearCorrespondences(std::string det_name, std::string desc_name)
{
  std::map<std::string, CorrespondencesMap>::iterator desc_corr_it;
  CorrespondencesMap::iterator dets_it;

  desc_corr_it = CorrespondencesMapMap.find(desc_name);
  if ( desc_corr_it != CorrespondencesMapMap.end())
    {
      dets_it = desc_corr_it->second.find(det_name);
      if (dets_it != desc_corr_it->second.end() )
        dets_it->second.TCList.clear();
    }
}
void CorrespondenceBank::AddCorrespondences(std::map<std::string, TentativeCorrespListExt> & CorrsToAddMap,std::string desc_name)
{
  CorrespondencesMap::iterator dets_it;

  for (dets_it = CorrsToAddMap.begin();
       dets_it != CorrsToAddMap.end(); dets_it++)
    AddCorrespondences(dets_it->second,desc_name,dets_it->first);
}

void CorrespondenceBank::AddCorrespondencesToList(TentativeCorrespListExt& BaseCorrs, TentativeCorrespListExt& CorrsToAdd)
{
  int size = (int)BaseCorrs.TCList.size();
  unsigned int new_size = size + CorrsToAdd.TCList.size();
  std::vector<TentativeCorrespExt>::iterator ptr = CorrsToAdd.TCList.begin();
  for (unsigned int i=size; i< new_size; i++, ptr++)
    BaseCorrs.TCList.push_back(*ptr);
}

template <typename T>
bool is_in_map(const string & q, const map<string, T> &m, T *v) {
  if (m.find(q) != m.end()) {
    *v = m.at(q);
    return true;
  }
  return false;
}

template <typename param_t>
void match_img_rep(const AffineRegionVector &queries, const AffineRegionVector &trains, TentativeCorrespListExt *tents, const MatchPars &par, const param_t &sv_par, const string &curr_desc) {
  MatchPars match_par = par;
  double v;
  if (is_in_map(curr_desc, sv_par.FGINNThreshold, &v)) {
    match_par.currMatchRatio = v;
    if (v > 0)
      MatchFlannFGINN(queries, trains, *tents, match_par);
  }
  if (is_in_map(curr_desc, sv_par.DistanceThreshold, &v)) {
    match_par.matchDistanceThreshold = v;
    if (v > 0)
      MatchFLANNDistance(queries, trains, *tents, match_par);
  }
}

int CorrespondenceBank::MatchImgReps(ImageRepresentation &imgrep1, ImageRepresentation &imgrep2,
                                     IterationViewsynthesisParam &synth_par, const WhatToMatch WhatToMatchNow,
                                     const MatchPars &par, const DescriptorsParameters &desc_pars)
{
  unsigned int n_group_desc = WhatToMatchNow.group_descriptors.size();
  unsigned int n_sep_det = WhatToMatchNow.separate_detectors.size();
  /// Grouped
#ifdef _OPENMP
  omp_set_nested(1);
#endif
#pragma omp parallel for schedule (dynamic,1)
  for (unsigned int gdesc = 0; gdesc < n_group_desc; gdesc++)
    {
      std::string curr_desc = WhatToMatchNow.group_descriptors[gdesc];
      ClearCorrespondences("Group",curr_desc);
      AffineRegionVector queries, trains;
      TentativeCorrespListExt current_tents;
      for (unsigned int gdet = 0; gdet < WhatToMatchNow.group_detectors.size(); gdet++)
        {
          std::string curr_det = WhatToMatchNow.group_detectors[gdet];

          AffineRegionVector tempRegs = imgrep2.GetAffineRegionVector(curr_desc,curr_det);
          AddRegionsToList(trains,tempRegs);

          tempRegs= imgrep1.GetAffineRegionVector(curr_desc,curr_det);
          AddRegionsToList(queries,tempRegs);
        }
      //Parameters
      match_img_rep(queries, trains, &current_tents, par, par, curr_desc);
      AddCorrespondences(current_tents,"Group",curr_desc);

    }
  ///Individual detectors
#pragma omp parallel for schedule (dynamic,1)
  for (unsigned int sdet = 0; sdet < n_sep_det; sdet++)
    {
      std::string curr_det = WhatToMatchNow.separate_detectors[sdet];
      ViewSynthParameters current_VS_params;

      IterationViewsynthesisParam::const_iterator thresh_it;
      thresh_it = synth_par.find(curr_det);
      if (thresh_it != synth_par.end() && (thresh_it->second.size() > 0))
        {
          current_VS_params = thresh_it->second[0];
#pragma omp parallel for schedule (dynamic,1)
          for (unsigned int s_desc = 0; s_desc < WhatToMatchNow.separate_descriptors.size(); s_desc++)
            {
              std::string curr_desc = WhatToMatchNow.separate_descriptors[s_desc];
              ClearCorrespondences(curr_det,curr_desc);

              AffineRegionVector queries, trains;
              TentativeCorrespListExt current_tents;

              AffineRegionVector tempRegs=imgrep2.GetAffineRegionVector(curr_desc,curr_det);
              AddRegionsToList(trains,tempRegs);

              tempRegs=imgrep1.GetAffineRegionVector(curr_desc,curr_det);
              AddRegionsToList(queries,tempRegs);

              match_img_rep(queries, trains, &current_tents, par, current_VS_params, curr_desc);

              std::cerr << "Matching det = " << curr_det << ", desc = " << curr_desc << std::endl;
              std::cerr << "num_descs1 = " << queries.size() << " num_descs2 = " << trains.size() << std::endl;
              AddCorrespondences(current_tents,curr_det,curr_desc);
            }
        }
    }
  return 0;

}

} //namespace mods
