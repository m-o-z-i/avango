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
  AV_FC_ADD_FIELD(mHandPos,                  ::gua::math::vec2());

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
    updateHandPos();
  }
}

void
av::daemon::HandSensor::updateHandPos()
{
    float x = getValue(1);
    float y = getValue(2);
    ::gua::math::vec2 pos(x, y);
    if(mHandPos.getValue() != pos){
        mHandPos.setValue(pos);
    }
}
