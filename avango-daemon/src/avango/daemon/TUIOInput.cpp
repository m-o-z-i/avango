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

#include <avango/Logger.h>
#include <avango/daemon/TUIOInput.h>
#include <avango/daemon/TUIOInputListener.h>
#include <avango/daemon/Station.h>
#include <string>
#include <sstream>

#include <TuioClient.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::TUIOInput"));

}

AV_BASE_DEFINE(av::daemon::TUIOInput);

av::daemon::TUIOInput::TUIOInput():
mTUIOInputListener(new TUIOInputListener())
{
  mRequiredFeatures.push_back("port");
}

av::daemon::TUIOInput::~TUIOInput()
{}

void
av::daemon::TUIOInput::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::TUIOInput, true);
  }
}

/* virtual */ void
av::daemon::TUIOInput::startDevice()
{
  if (!parseFeatures())
    return;

  logger.info() << "startDevice: device configured successfully";

  // initialize

  mTUIOClient = boost::shared_ptr<TUIO::TuioClient>(new TUIO::TuioClient(mPort));
  mTUIOClient->addTuioListener(mTUIOInputListener.get());
  mTUIOClient->connect();

  logger.info() << "startDevice: device initialized successfully";
}

/* virtual */ void
av::daemon::TUIOInput::readLoop()
{

  while (mKeepRunning)
  {
    for (auto const& station : mStations) {
      auto cursor(mTUIOInputListener->cursors.find(station.first));

      if (cursor != mTUIOInputListener->cursors.end())
      {
        station.second->setValue(0, cursor->second.x);
        station.second->setValue(1, cursor->second.y);
        station.second->setValue(2, cursor->second.x_speed);
        station.second->setValue(3, cursor->second.y_speed);
        station.second->setValue(4, cursor->second.motion_speed);
        station.second->setValue(5, cursor->second.motion_acceleration);
        station.second->setValue(6, cursor->second.is_moving);
        station.second->setValue(7, cursor->second.state);
        station.second->setValue(8, cursor->second.session_id);
      } else {
        station.second->setValue(0, -1.f);
        station.second->setValue(1, -1.f);
        station.second->setValue(2, 0.f);
        station.second->setValue(3, 0.f);
        station.second->setValue(4, 0.f);
        station.second->setValue(5, 0.f);
        station.second->setValue(6, false);
        station.second->setValue(7, TUIOState::REMOVED);
        station.second->setValue(8, -1.f);
      }
    }
  }
}

/* virtual */ void
av::daemon::TUIOInput::stopDevice()
{
  mTUIOClient->disconnect();
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::TUIOInput::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::TUIOInput::parseFeatures()
{
  std::string port(queryFeature("port"));
  if (port == "")
  {
    logger.warn() << "parseFeatures: feature 'port' not specified";
    return false;
  }
  else
  {
    logger.info() << "parseFeatures: configured feature 'port' = %s", port;
    std::stringstream(port) >> mPort;
  }

  return true;
}
