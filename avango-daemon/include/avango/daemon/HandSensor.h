#ifndef HANDSENSOR_H
#define HANDSENSOR_H


/**
 * \file
 * \ingroup av_daemon
 */

#include <string>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>
#include <avango/daemon/DeviceService.h>
#include <avango/daemon/DeviceSensor.h>

namespace av
{
  namespace daemon
  {
    /**
     * Communicates with DeviceService and provides input data
     * of the device the associated DeviceService is connected with.
     * Depending on how the corresponding device is configured
     * the data is provided via fields for Values, Buttons, LEDs and
     * a Matrix.
     *
     * Note: A sensor can only be used to readout input data. Setting
     * field values by hand will have no effect. To send commands
     * to a device you should use an actuator.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL HandSensor : public DeviceSensor
    {

      AV_FC_DECLARE();

    public:

      HandSensor();
      virtual ~HandSensor();
      av::gua::SFVec2       mHandPos;

      /* virtual */ void evaluate();

    private:

      void updateHandPos();
    };
  }
}

#endif // HANDSENSOR_H
