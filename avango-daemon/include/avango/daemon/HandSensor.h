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
    class AV_DAEMON_DLL HandSensor : public FieldContainer
    {

      AV_FC_DECLARE();

    public:

      HandSensor();
      virtual ~HandSensor();

      /**
       * Name of device service to communicate with.
       */
      SFDeviceService DeviceService;

      /**
       * Name of station to connect with.
       */
      SFString Station;

      /**
       * Reset station values to '0' after read.
       */
      SFBool   ResetValuesOnRead;

      /**
       * Transformation matrix provided by given station (read-only).
       */
      av::gua::SFMatrix  Matrix;

      /**
       * Read-only fields: Value states of connected station.
       */
      SFFloat Value0;
      SFFloat Value1;
      SFFloat Value2;
      SFFloat Value3;
      SFFloat Value4;
      SFFloat Value5;
      SFFloat Value6;
      SFFloat Value7;
      SFFloat Value8;
      SFFloat Value9;
      SFFloat Value10;
      SFFloat Value11;
      SFFloat Value12;
      SFFloat Value13;
      SFFloat Value14;
      SFFloat Value15;
      SFFloat Value16;
      SFFloat Value17;

      /* virtual */ void evaluate();

    private:

      /**
       * Update value states if changed.
       */
      void updateValues();

      /**
       * Readout state of a specified Value provided by a specific device station.
       */
      float getValue(int v);

      static const int sMaxValues = 18;

    };
  }
}

#endif // HANDSENSOR_H
