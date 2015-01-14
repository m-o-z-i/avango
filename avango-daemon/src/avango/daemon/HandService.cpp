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

// includes, system
#include <algorithm>
#include <cstdlib>
#include <list>

// includes, project
#include <avango/daemon/HandService.h>
#include <avango/daemon/Hand.h>
#include <avango/daemon/HandSegment.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::HandService"));

  typedef std::list<av::daemon::HandSegment*> hand_segment_list_type;
  hand_segment_list_type hand_segment_list;

  extern "C" void remove_remaining_hand_segments()
  {
    while (!hand_segment_list.empty())
    {
      logger.debug() << "remove_remaining_hand_segments: destructing 'fpHandSegment' @"
                     << static_cast<const void*> (*(hand_segment_list.begin()));

      delete *(hand_segment_list.begin());
      hand_segment_list.pop_front();
    }
  }
}

AV_BASE_DEFINE(av::daemon::HandService);

AV_FIELD_DEFINE(av::daemon::SFHandService);
AV_FIELD_DEFINE(av::daemon::MFHandService);

av::daemon::HandService::HandService()
  : mIdentityMatrix(new ::gua::math::mat4),
    mCachedHandName(0),
    mCachedHand(0),
    mHandSegment(0)
{
  connectDaemon();
}

/* virtual */
av::daemon::HandService::~HandService()
{
  disconnectDaemon();
}

/* static */ void
av::daemon::HandService::initClass()
{
  if (!isTypeInitialized())
  {
    Base::initClass();
    AV_BASE_INIT(av::Base, av::daemon::HandService, true);
  }
}

void
av::daemon::HandService::connectDaemon()
{
  disconnectDaemon();
  mHandSegment = new HandSegment;
  hand_segment_list.push_back(mHandSegment);
}

void
av::daemon::HandService::disconnectDaemon()
{
  hand_segment_list.remove(mHandSegment);
  delete mHandSegment;
  mHandSegment = 0;
  clearHandCache();
}

void
av::daemon::HandService::clearHandCache()
{
  mCachedHandName = 0;
  mCachedHand = 0;
}

av::daemon::Hand*
av::daemon::HandService::lookupCachedHand(const char* hand_name)
{
  if (hand_name == mCachedHandName)
  {
    logger.trace() << "lookupCachedHand(): " << this << " cache hit for hand " << hand_name;
    return mCachedHand;

  }
  else if (mHandSegment)
  {
    logger.trace() << "lookupCachedHand(): " << this << " cache miss for hand " << hand_name;
    Hand* hand = mHandSegment->getHand(hand_name);

    if (hand)
    {
      mCachedHandName = hand_name;
      mCachedHand = hand;
      return hand;
    }
    else
    {
      clearHandCache();
      logger.warn() << "lookupCachedHand(): hand " << hand_name << " not found; cache cleared.";
      return 0;
    }
  }
  else
  {
    logger.warn() << "lookupCachedHand(): no hand segment initialized.";
  }

  return 0;
}

const gua::math::mat4&
av::daemon::HandService::getMatrix(const char* hand_name)
{
  if (mHandSegment)
  {
    const Hand* hand = lookupCachedHand (hand_name);

    if (hand)
    {
      logger.trace() << "getMatrix(): " << this << " succeeded for hand " << hand_name;
      return hand->getMatrix();
    }
    else logger.warn() << "getMatrix(): " << this << " failed for hand " << hand_name << " (no hand)";
  }
  else logger.warn() << "getMatrix(): " << this << " failed for hand " << hand_name << " (no segment)";

  return *mIdentityMatrix;
}

float
av::daemon::HandService::getValue(const char* hand_name, int which)
{
  if (mHandSegment)
  {
    const Hand* hand = lookupCachedHand(hand_name);

    if (hand) return hand->getValue(which);
  } else logger.warn() << "getValue(): no hand segment initialized.";

  return 0.0f;
}

void
av::daemon::HandService::setMatrix(const char* hand_name, const ::gua::math::mat4& value)
{
  if (mHandSegment)
  {
    Hand* hand = lookupCachedHand (hand_name);

    if (hand)
    {
      hand->setMatrix(value);
      logger.info() << "setMatrix(): " << this << " succeeded for hand " << hand_name;
    }
    else logger.warn() << "setMatrix(): " << this << " failed for hand " << hand_name << " (no hand)";
  }
  else logger.warn() << "setMatrix(): " << this << " failed for hand " << hand_name << " (no segment)";
}


void
av::daemon::HandService::setValue(const char* hand_name, int which, float value)
{
  if (mHandSegment)
  {
    Hand* hand = lookupCachedHand(hand_name);

    if (hand) hand->setValue(which, value);
  } else logger.warn() << "setValue(): no hand segment initialized.";
}


bool
av::daemon::HandService::getMatrixUsed(const char* hand_name)
{
  if (mHandSegment)
  {
    const Hand* hand = lookupCachedHand(hand_name);

    if (hand) return hand->getMatrixUsed();
  } else logger.warn() << "getMatrixUsed(): no hand segment initialized.";

  return false;
}

int
av::daemon::HandService::getValuesUsed(const char* hand_name)
{
  if (mHandSegment)
  {
    const Hand* hand = lookupCachedHand(hand_name);

    if (hand) return hand->getValuesUsed();
  } else logger.warn() << "getValuesUsed(): no hand segment initialized.";

  return 0;
}
