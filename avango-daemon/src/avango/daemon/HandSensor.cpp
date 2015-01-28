#include <avango/daemon/HandSensor.h>

#include <avango/SingleField.h>
#include <avango/Logger.h>

#include <scm/core/math.h>
#include <gua/math.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::HandSensor"));
}

AV_FC_DEFINE(av::daemon::HandSensor);

av::daemon::HandSensor::HandSensor()
{
  alwaysEvaluate(true);
}

av::daemon::HandSensor::~HandSensor()
{}

void
av::daemon::HandSensor::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::DeviceSensor::initClass();
    AV_FC_INIT(av::daemon::DeviceSensor, av::daemon::HandSensor, true);
  }
}

/* virtual */ void
av::daemon::HandSensor::evaluate()
{
  if (DeviceService.getValue().isValid() && !(Station.getValue().empty()) )
  {
    //update hand values
  }
}
