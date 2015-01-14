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

#include <avango/daemon/HandBlock.h>

#include <avango/Logger.h>
#include <cstring>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::HandBlock"));
}

av::daemon::HandBlock::HandBlock()
  : mNumHands(0),
    mMutex()
{}

av::daemon::HandBlock::~HandBlock()
{}

av::daemon::Hand*
av::daemon::HandBlock::getHand(const char* name)
{
  // very simple and inefficient for now
  Hand* hand = 0;

  boost::mutex::scoped_lock lock(mMutex);

  int i;

  for (i=0; i<mNumHands; i++)
  {
    if (std::strcmp(name, mHands[i].getName()) == 0)
    {
      hand = &mHands[i];
      LOG_TRACE(logger) << "getHand(): referenced hand '" << name << "', " << hand;

      break;
    }
  }

  if (!hand && i < sMaxHandNum) {
    hand = new (&mHands[i]) Hand;
    hand->setName(name);
    logger.debug() << "getHand(): created hand '" << name << "', " << hand;

    mNumHands++;
  }

  return hand;
}
