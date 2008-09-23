// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/tools/ProximitySelector.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ProximitySelector"));

  osg::Vec3::value_type
  distance(const osg::Vec3& pos1, const osg::Vec3& pos2)
  {
    return (pos1 - pos2).length();
  }
}

AV_FC_DEFINE(av::tools::ProximitySelector);

AV_FIELD_DEFINE(av::tools::SFProximitySelector);
AV_FIELD_DEFINE(av::tools::MFProximitySelector);

av::tools::ProximitySelector::ProximitySelector()
{
  AV_FC_ADD_FIELD(TargetObjects, MFObject::ContainerType());
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Position, ::osg::Vec3(0.0, 0.0, 0.0));
  AV_FC_ADD_FIELD(PositionTransform, ::osg::Matrix());
  AV_FC_ADD_FIELD(ProximityRadius, 1.0);
  AV_FC_ADD_FIELD(DistanceRadius, 2.0);
  AV_FC_ADD_FIELD(Time, 0.0);
  AV_FC_ADD_FIELD(ProximityLag, 0.0);
  AV_FC_ADD_FIELD(MaxNumberOfTargets, 0u);
}

av::tools::ProximitySelector::~ProximitySelector()
{}

void
av::tools::ProximitySelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::ProximitySelector, true);

    SFProximitySelector::initClass("av::tools::SFProximitySelector", "av::Field");
    MFProximitySelector::initClass("av::tools::MFProximitySelector", "av::Field");
  }
}

/* virtual */ void
av::tools::ProximitySelector::evaluate()
{
  av::tools::Selector::evaluate();

  // get needed field values
  const MFObject::ContainerType &target_objects = TargetObjects.getValue();
  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  const ::osg::Vec3 pos = Position.getValue() * PositionTransform.getValue();
  const double prox_radius = ProximityRadius.getValue();
  const double dist_radius = DistanceRadius.getValue();
  const double time = Time.getValue();
  const double lag = ProximityLag.getValue();
  const unsigned int max_targets = MaxNumberOfTargets.getValue();

  // remove proximity candidates, which are no longer in the target lists
  {
    TargetTimeList_t::iterator cand = mProxCands.begin();
    while (cand != mProxCands.end())
    {
      if (hasObject(target_objects, cand->first) || av::tools::hasTarget(targets, cand->first))
        ++cand;
      else
        cand = mProxCands.erase(cand);
    }
  }

  // remove distance candidates, which are no longer in the target lists
  {
    TargetTimeList_t::iterator cand = mDistCands.begin();
    while (cand != mDistCands.end())
    {
      if (hasObject(target_objects, cand->first) || av::tools::hasTarget(targets, cand->first))
        ++cand;
      else
      {
        MFTargetHolder::ContainerType::iterator holder = av::tools::find(mSelTargets, cand->first);
        if (holder != mSelTargets.end())
          mSelTargets.erase(holder);
        else
          logger.warn() << "distance candidate was not in selected targets";
        cand = mDistCands.erase(cand);
      }
    }
  }

  // remove selected targets, which are no longer in the target lists
  {
    MFTargetHolder::ContainerType::iterator holder = mSelTargets.begin();
    while (holder != mSelTargets.end())
    {
      const SFObject::ValueType &target = (*holder)->Target.getValue();
      if (hasObject(target_objects, target) || av::tools::hasTarget(targets, target))
        ++holder;
      else
        holder = mSelTargets.erase(holder);
    }
  }

  TargetTimeList_t new_prox_cands, new_dist_cands;
  MFTargetHolder::ContainerType new_sel_targets;

  // check for new proximity candidates in TargetObjects
  for (MFObject::ContainerType::const_iterator target = target_objects.begin();
       target != target_objects.end(); ++target)
  {
    if (!hasTarget(mProxCands, *target) && !av::tools::hasTarget(mSelTargets, *target))
    {
      // we only accept osg nodes to get the absolute transform
      Link<av::osg::Node> node = dynamic_cast<av::osg::Node*>(target->getPtr());
      if (node.isValid() &&
          distance(pos, node->getAbsoluteTransform(this).getTrans()) < prox_radius)
      {
        if (lag > 0.000001)
          new_prox_cands.push_back(TargetTimePair_t(node, time));
        else
        {
          new_sel_targets.push_back(new TargetHolder);
          new_sel_targets.back()->Target.setValue(*target);
          new_sel_targets.back()->Creator.setValue(this);
        }
      }
    }
  }

  // check for new proximity candidates in Targets
  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFObject::ValueType &target = (*holder)->Target.getValue();
    if (!hasTarget(mProxCands, target) && !av::tools::hasTarget(mSelTargets, target))
    {
      // we only accept osg nodes to get the absolute transform
      Link<av::osg::Node> node = dynamic_cast<av::osg::Node*>(target.getPtr());
      if (node.isValid() &&
          distance(pos, node->getAbsoluteTransform(this).getTrans()) < prox_radius)
      {
        if (lag > 0.000001)
          new_prox_cands.push_back(TargetTimePair_t(node, time));
        else
          new_sel_targets.push_back(*holder);
      }
    }
  }

  // check if proximity candidates are out of range or have timed out
  {
    TargetTimeList_t::iterator cand = mProxCands.begin();
    while (cand != mProxCands.end())
    {
      if (distance(pos, cand->first->getAbsoluteTransform(this).getTrans()) > prox_radius)
        cand = mProxCands.erase(cand);
      else if (time - cand->second > lag)
      {
        MFTargetHolder::ContainerType::const_iterator holder =
          av::tools::find(targets, cand->first);
        if (holder != targets.end())
          new_sel_targets.push_back(*holder);
        else
        {
          new_sel_targets.push_back(new TargetHolder);
          new_sel_targets.back()->Target.setValue(cand->first);
          new_sel_targets.back()->Creator.setValue(this);
        }

        cand = mProxCands.erase(cand);
      }
      else
        ++cand;
    }
  }

  // check for updated target holders and if selected targets are out of range
  {
    MFTargetHolder::ContainerType::iterator holder = mSelTargets.begin();
    while (holder != mSelTargets.end())
    {
      const SFObject::ValueType target = (*holder)->Target.getValue();
      MFTargetHolder::ContainerType::const_iterator input_holder = av::tools::find(targets, target);
      if (input_holder != targets.end() && *holder != *input_holder)
        *holder = *input_holder;

      if (hasTarget(mDistCands, target))
        ++holder;
      else
      {
        Link<av::osg::Node> node = dynamic_cast<av::osg::Node*>(target.getPtr());
        if (node.isValid())
        {
          if (distance(pos, node->getAbsoluteTransform(this).getTrans()) > dist_radius)
          {
            if (lag > 0.000001)
            {
              new_dist_cands.push_back(TargetTimePair_t(node, time));
              ++holder;
            }
            else
              holder = mSelTargets.erase(holder);
          }
          else
            ++holder;
        }
        else
        {
          holder = mSelTargets.erase(holder);
          logger.warn() << "non-node target was in selected targets";
        }
      }
    }
  }

  // check if distance candidates are back in range or have have timed out
  {
    TargetTimeList_t::iterator cand = mDistCands.begin();
    while (cand != mDistCands.end())
    {
      if (distance(pos, cand->first->getAbsoluteTransform(this).getTrans()) < dist_radius)
        cand = mDistCands.erase(cand);
      else if (time - cand->second > lag)
      {
        MFTargetHolder::ContainerType::iterator holder = av::tools::find(mSelTargets, cand->first);
        if (holder != mSelTargets.end())
          mSelTargets.erase(holder);
        else
          logger.warn() << "distance candidate was not in selected targets";
        cand = mDistCands.erase(cand);
      }
      else
        ++cand;
    }
  }

  // update lists
  mProxCands.splice(mProxCands.end(), new_prox_cands);
  mDistCands.splice(mDistCands.end(), new_dist_cands);
  mSelTargets.insert(mSelTargets.end(), new_sel_targets.begin(), new_sel_targets.end());

  // check if we have too many targets
  if (max_targets != 0u)
  {
    while (mSelTargets.size() > max_targets && !mDistCands.empty())
    {
      MFTargetHolder::ContainerType::iterator holder =
        av::tools::find(mSelTargets, mDistCands.front().first);
      if (holder != mSelTargets.end())
        mSelTargets.erase(holder);
      else
        logger.warn() << "distance candidate was not in selected targets";
      mDistCands.pop_front();
    }

    if (mSelTargets.size() > max_targets)
    {
      mSelTargets.erase(mSelTargets.begin(),
                        mSelTargets.begin() + (mSelTargets.size() - max_targets));
    }
  }

  // update output field
  if (SelectedTargets.getValue() != mSelTargets)
    SelectedTargets.setValue(mSelTargets);
}
