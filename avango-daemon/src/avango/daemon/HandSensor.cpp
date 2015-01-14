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
  AV_FC_ADD_FIELD(DeviceService,            0);
  AV_FC_ADD_FIELD(Station,                  ::std::string());
  AV_FC_ADD_FIELD(ResetValuesOnRead,        false);
  AV_FC_ADD_FIELD(Matrix,                   ::gua::math::mat4::identity());
  AV_FC_ADD_FIELD(Value0,                   0.0f);
  AV_FC_ADD_FIELD(Value1,                   0.0f);
  AV_FC_ADD_FIELD(Value2,                   0.0f);
  AV_FC_ADD_FIELD(Value3,                   0.0f);
  AV_FC_ADD_FIELD(Value4,                   0.0f);
  AV_FC_ADD_FIELD(Value5,                   0.0f);
  AV_FC_ADD_FIELD(Value6,                   0.0f);
  AV_FC_ADD_FIELD(Value7,                   0.0f);
  AV_FC_ADD_FIELD(Value8,                   0.0f);
  AV_FC_ADD_FIELD(Value9,                   0.0f);
  AV_FC_ADD_FIELD(Value10,                  0.0f);
  AV_FC_ADD_FIELD(Value11,                  0.0f);
  AV_FC_ADD_FIELD(Value12,                  0.0f);
  AV_FC_ADD_FIELD(Value13,                  0.0f);
  AV_FC_ADD_FIELD(Value14,                  0.0f);
  AV_FC_ADD_FIELD(Value15,                  0.0f);
  AV_FC_ADD_FIELD(Value16,                  0.0f);
  AV_FC_ADD_FIELD(Value17,                  0.0f);

  alwaysEvaluate(true);
}

av::daemon::HandSensor::~HandSensor()
{}

void
av::daemon::HandSensor::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::daemon::HandSensor, true);
  }
}

/* virtual */ void
av::daemon::HandSensor::evaluate()
{
  if (DeviceService.getValue().isValid() && !(Station.getValue().empty()) )
  {
    updateValues();
  }
}

void
av::daemon::HandSensor::updateValues()
{
  for (int i=0; i<sMaxValues; ++i)
  {
    std::stringstream fieldname;
    fieldname << "Value" << i;
    SingleField<float>* sf = dynamic_cast<SingleField<float>*>(getField(fieldname.str()));

    if ( (sf != 0) && (getValue(i) != sf->getValue()) )
      sf->setValue(getValue(i));
  }
}

float
av::daemon::HandSensor::getValue(int v)
{
  if((v >= 0) && (v < sMaxValues))
  {
    const char* stationstr = Station.getValue().c_str();
    float value = DeviceService.getValue()->getValue(stationstr, v);
    if (ResetValuesOnRead.getValue()) DeviceService.getValue()->setValue(stationstr, v, 0.0f);
    return value;
  }

  return 0.0f;
}
