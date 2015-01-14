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
#include <boost/algorithm/string/predicate.hpp>

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
    for (auto const& station : mHands) {
        if (nullptr == station.second) {
            // we exceeded the maximum number of stations
            break;
        }

        std::string name = station.second->getName();
        //std::cout << "AVANGO: Station Name: " << name << "  ; Anz: " << station.first << "  ; mStation: " << mStations.size() << std::endl;

        if (boost::algorithm::ends_with(name, "#cursor") || name.find("#") == std::string::npos) {
            int sessionID = getSessionIDForHand(station, 0, mTUIOInputListener->cursors);
            auto cursor(mTUIOInputListener->cursors.find(sessionID));
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
        } else if (boost::algorithm::ends_with(name, "#finger")) {
            int sessionID = getSessionIDForHand(station, 1, mTUIOInputListener->fingers);
            auto finger(mTUIOInputListener->fingers.find(sessionID));
            if (finger != mTUIOInputListener->fingers.end()) {
                station.second->setValue(0, finger->second.session_id);
                station.second->setValue(1, finger->second.x);
                station.second->setValue(2, finger->second.y);
                station.second->setValue(3, finger->second.x_speed);
                station.second->setValue(4, finger->second.y_speed);
                station.second->setValue(5, finger->second.acceleration);
                station.second->setValue(6, finger->second.type);

            } else {
                station.second->setValue(0, -1.f);
                station.second->setValue(1, -1.f);
                station.second->setValue(2, -1.f);
                station.second->setValue(3, 0.f);
                station.second->setValue(4, 0.f);
                station.second->setValue(5, 0.f);
                station.second->setValue(6, TUIO::TuioFinger::Type::UNKNOWN);

            }
        } else if (boost::algorithm::ends_with(name, "#hand")) {
            int sessionID = getSessionIDForHand(station, 2, mTUIOInputListener->hands);
            auto hand(mTUIOInputListener->hands.find(sessionID));
            if (hand != mTUIOInputListener->hands.end()) {
                station.second->setValue(0,  hand->second.session_id);
                std::cout << "AVANGO: hand " << hand->second.session_id << "  pos = " << hand->second.x_pos << " arm: " << hand->second.x_arm_center << "  hand_class " << hand->second.hand_class << std::endl;

                station.second->setValue(1,  hand->second.x_pos);
                station.second->setValue(2,  hand->second.y_pos);

                station.second->setValue(3,  hand->second.fingers[0]);
                station.second->setValue(4,  hand->second.fingers[1]);
                station.second->setValue(5,  hand->second.fingers[2]);
                station.second->setValue(6,  hand->second.fingers[3]);
                station.second->setValue(7,  hand->second.fingers[4]);

                station.second->setValue(8,   hand->second.bBox_minX);
                station.second->setValue(9,   hand->second.bBox_minY);
                station.second->setValue(10,  hand->second.bBox_maxX);
                station.second->setValue(11,  hand->second.bBox_maxY);

                station.second->setValue(12,  hand->second.hand_class);

                station.second->setValue(13,  hand->second.x_arm_center   );
                station.second->setValue(14,  hand->second.y_arm_center   );
                station.second->setValue(15,  hand->second.arm_major_axis );
                station.second->setValue(16,  hand->second.arm_minor_axis );
                station.second->setValue(17,  hand->second.arm_inclination);

            } else {
                station.second->setValue(0, -1.f);

                station.second->setValue(1, -1.f);
                station.second->setValue(2, -1.f);

                station.second->setValue(3, -1.f);
                station.second->setValue(4, -1.f);
                station.second->setValue(5, -1.f);
                station.second->setValue(6, -1.f);
                station.second->setValue(7, -1.f);

                station.second->setValue(8, 0.f);
                station.second->setValue(9, 0.f);
                station.second->setValue(10, 0.f);
                station.second->setValue(11, 0.f);

                station.second->setValue(12, TUIO::TuioHand::Class::UNKNOWN);

                station.second->setValue(13, 0.f);
                station.second->setValue(14, 0.f);
                station.second->setValue(15, 0.f);
                station.second->setValue(16, 0.f);
                station.second->setValue(17, 0.f);
            }
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
