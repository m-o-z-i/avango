/*
 TUIO C++ Library - part of the reacTIVision project
 http://reactivision.sourceforge.net/

 Part of avango for Guacamlole.

 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
               2014 Janek Bevendorff <janek.bevendorff@uni-weimar.de>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_TUIOFINGER_H
#define INCLUDED_TUIOFINGER_H

#include <math.h>
#include "TuioContainer.h"

namespace TUIO {

    /**
     * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
     *
     * @author Janek Bevendorff, Moritz Loos
     * @version 1.4.01
     */
    class TuioFinger: public TuioContainer {

    public:

        enum Type { UNKNOWN = 0, THUMB = 1, INDEX = 2, MIDDLE = 3, RING = 4 , LITTLE = 5};


        /**
        * This constructor takes a TuioTime argument and assigns it along with the provided
        * Session ID, Finger ID, X and Y coordinate to the newly created TuioFinger
        *
        * @param	ttime	the TuioTime to assign
        * @param	si	the Session ID  to assign
        * @param	xp	the X coordinate to assign
        * @param	yp	the Y coordinate to assign
        * @param    xv  the X velocity to assign
        * @param    yv  the Y velocity to assign
        * @param    ac  the acceleration to assign
        * @param    ft  the finger type to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
        */
        TuioFinger (TuioTime ttime,
                    long si,
                    float xp, float yp,
                    float xv, float yv,
                    float ac,
                    Type ft) :
            TuioContainer(ttime,si,xp,yp),
            finger_id(si),
            x_speed(xv), y_speed(yv),
            acceleration(ac),
            type(ft)
        {
        }

        /**
        * This constructor takes the provided Session ID, Cursor ID, X and Y coordinate
        * and assigs these values to the newly created TuioCursor.
        *
        * @param	si	the Session ID  to assign
        * @param	xp	the X coordinate to assign
        * @param    yp	the Y coordinate to assign
        * @param    xv  the X velocity to assign
        * @param    yv  the Y velocity to assign
        * @param    ac  the acceleration to assign
        * @param    ft  the finger type to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
        */
        TuioFinger (long si,
                    float xp, float yp,
                    float xv, float yv,
                    float ac,
                    Type ft) :
            TuioContainer(si,xp,yp),
            finger_id(si),
            x_speed(xv), y_speed(yv),
            acceleration(ac),
            type(ft)
        {
        }

        /**
        * Update finger and corresponding 2DCursors.
        *
        * @param   xp  the X coordinate to assign
        * @param   yp  the Y coordinate to assign
        * @param   xv  the X velocity to assign
        * @param   yv  the Y velocity to assign
        * @param   ac  the acceleration to assign
        * @param   ft  the finger type to assign (UNKNOWN, THUMB, INDEX, MIDDLE, RING, LITTLE)
        */
        void update(float xp, float yp,
                    float xv, float yv,
                    float ac,
                    Type ft)
        {
            TuioContainer::update(TuioTime::getSessionTime(), xp, yp);
            x_speed = xv;
            y_speed = yv;
            acceleration = ac;
            type = ft;
        }

        /**
         * Returns the Finger ID of this TuioFinger.
         * @return	the Finger ID of this TuioFinger
         */
        int getFingerID() {
            return finger_id;
        }

        /**
         * Returns the X velocity of this TuioFinger.
         * @return	the X velocity of this TuioFinger
         */
        float getXSpeed() {
            return x_speed;
        }

        /**
         * Returns the Y velocity of this TuioFinger.
         * @return	the Y velocity of this TuioFinger
         */
        float getYSpeed() {
            return y_speed;
        }

        /**
         * Returns the acceleration of this TuioFinger.
         * @return	the acceleration of this TuioFinger
         */
        float getAcceleration() {
            return acceleration;
        }

        /**
         * Get the Type of this finger.
         * @return the finger type
         */
        Type getFingerType() {
            return type;
        }

    protected:
        /**
         * The individual finger ID number that is assigned to each TuioFinger.
         */
        int finger_id;
        float x_speed;
        float y_speed;
        float acceleration;
        Type type;
    };
}
#endif
