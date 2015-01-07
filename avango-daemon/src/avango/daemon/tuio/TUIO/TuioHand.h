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

#ifndef INCLUDED_TUIOHAND_H
#define INCLUDED_TUIOHAND_H

#include <math.h>
#include <array>
#include "TuioContainer.h"

namespace TUIO {

    /**
     * The TuioFinger class encapsulates /tuio/_Finger TUIO fingers.
     *
     * @author Janek Bevendorff, Moritz Loos
     * @version 1.4.01
     */
    class TuioHand: public TuioContainer {

    public:
        /**
          * The class of the  hand.
          */
        enum Class { UNKNOWN = 0, LEFT = 1, RIGHT = 2 };

        typedef std::array<long, 5> FingerArray;

        /**
         * This constructor takes a TuioTime argument and assigns it along with the provided
         * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
         *
         * @param	ttime	the TuioTime to assign
         * @param	si	the Session ID  to assign
         * @param   xp  the X coordinate to assign
         * @param   yp  the Y coordinate to assign
         * @param	f1	first finger session ID
         * @param	f2	second finger session ID
         * @param	f3	third finger session ID
         * @param	f4	fourth finger session ID
         * @param	f5	fifth finger session ID
         * @param   x1  the min X coordinate of Bounding Box to assign
         * @param   y1  the min Y coordinate of Bounding Box to assign
         * @param   x2  the max X coordinate of Bounding Box to assign
         * @param   y2  the max Y coordinate of Bounding Box to assign
         * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
         * @param   xa  the arm center X coordinate
         * @param   ya  the arm center Y coordinate
         * @param   mi  the arm minor axis
         * @param   ma  the arm major axis
         * @param   in  the arm inclination

         */
        TuioHand (TuioTime ttime,
                  long si, float xp, float yp,
                  long f1, long f2, long f3, long f4, long f5,
                  float x1, float y1, float x2, float y2,
                  Class hc, float xa, float ya, float mi, float ma, float in) :
            TuioContainer(ttime,si,-1,-1),
            hand_id(si), x_pos(xp), y_pos(yp),
            fingers({f1, f2, f3, f4, f5}),
            bBox_minX(x1), bBox_minY(y1), bBox_maxX(x2), bBox_maxY(y2),
            hand_class(hc),
            x_arm_center(xa), y_arm_center(ya), arm_minor_axis(mi), arm_major_axis(ma), arm_inclination(in)
        {
        }

        /**
         * This constructor takes a TuioTime argument and assigns it along with the provided
         * Session ID, Hand ID, X and Y coordinate to the newly created TuioFinger
         *
         * @param	si	the Session ID  to assign
         * @param   xp  the X coordinate to assign
         * @param   yp  the Y coordinate to assign
         * @param	f1	first finger session ID
         * @param	f2	second finger session ID
         * @param	f3	third finger session ID
         * @param	f4	fourth finger session ID
         * @param	f5	fifth finger session ID
         * @param   x1  the min X coordinate of Bounding Box to assign
         * @param   y1  the min Y coordinate of Bounding Box to assign
         * @param   x2  the max X coordinate of Bounding Box to assign
         * @param   y2  the max Y coordinate of Bounding Box to assign
         * @param	hc	the hand class to assign (UNKNOWN, LEFT,RIGHT)
         * @param   xa  the arm center X coordinate
         * @param   ya  the arm center Y coordinate
         * @param   mi  the arm minor axis
         * @param   ma  the arm major axis
         * @param   in  the arm inclination

         */
        TuioHand (long si, float xp, float yp,
                  long f1, long f2, long f3, long f4, long f5,
                  float x1, float y1, float x2, float y2,
                  Class hc, float xa, float ya, float mi, float ma, float in) :
            TuioContainer(si,-1,-1),
            hand_id(si), x_pos(xp), y_pos(yp),
            fingers({f1, f2, f3, f4, f5}),
            bBox_minX(x1), bBox_minY(y1), bBox_maxX(x2), bBox_maxY(y2),
            hand_class(hc),
            x_arm_center(xa), y_arm_center(ya), arm_minor_axis(mi), arm_major_axis(ma), arm_inclination(in)
        {
        }

        /**
          * Update hand.
          *
         * @param   xp  the X coordinate to assign
         * @param   yp  the Y coordinate to assign
         * @param	f1	first finger session ID
         * @param	f2	second finger session ID
         * @param	f3	third finger session ID
         * @param	f4	fourth finger session ID
         * @param	f5	fifth finger session ID
         * @param   x1  the min X coordinate of Bounding Box to assign
         * @param   y1  the min Y coordinate of Bounding Box to assign
         * @param   x2  the max X coordinate of Bounding Box to assign
         * @param   y2  the max Y coordinate of Bounding Box to assign
         * @param   xa  the arm center X coordinate
         * @param   ya  the arm center Y coordinate
         * @param   mi  the arm minor axis
         * @param   ma  the arm major axis
         * @param   in  the arm inclination
          */
        void update(float xp, float yp, long f1, long f2, long f3, long f4, long f5,
                    float x1, float y1, float x2, float y2,
                    float xa, float ya, float mi, float ma, float in) {
            x_pos = xp;
            y_pos = yp;
            fingers[0] = f1;
            fingers[1] = f2;
            fingers[2] = f3;
            fingers[3] = f4;
            fingers[4] = f5;
            bBox_minX = x1;
            bBox_minY = y1;
            bBox_maxX = x2;
            bBox_maxY = y2;
            x_arm_center = xa;
            y_arm_center = ya;
            arm_minor_axis = mi;
            arm_major_axis = ma;
            arm_inclination = in;
        }

        /**
          * Get the class  of this hand.
          * @return the hand class
          */
        Class getHandClass() {
            return hand_class;
        }

        /**
          * Get a std::array of finger IDs for this hand.
          * @return the finger Session IDs
          */
        std::array<long, 5> getFingerIDs() {
            return fingers;
        }

        /**
         * Returns the Hand ID of this TuioHand.
         * @return	the Hand ID of this TuioHand
         */
        int getHandID() {
            return hand_id;
        }

        /**
         * Returns the X coordinate of this Hand.
         * @return	the X coordinate of this Hand
         */
        float getXPos() {
            return x_pos;
        }

        /**
         * Returns the Y coordinate of this Hand.
         * @return	the Y coordinate of this Hand
         */
        float getYPos() {
            return y_pos;
        }

        /**
          * Return the arm center X ccordinate.
          * @return the arm center X ccordinate
          */
        float getArmX() {
            return x_arm_center;
        }

        /**
          * Return the arm center Y ccordinate.
          * @return the arm center Y ccordinate
          */
        float getArmY() {
            return y_arm_center;
        }

        /**
          * Return the arm center major axis.
          * @return the arm center major axis
          */
        float getArmMajor() {
            return arm_major_axis;
        }

        /**
          * Return the arm center minor axis.
          * @return the arm center minor axis
          */
        float getArmMinor() {
            return arm_minor_axis;
        }

        /**
          * Return the arm inclination.
          * @return the arm inclination
          */
        float getArmInclination() {
            return arm_inclination;
        }

        /**
          * Return the min x point of boundingbox.
          * @return the arm inclination
          */
        float getBoundingBoxMinX() {
            return bBox_minX;
        }

        /**
          * Return the min x point of boundingbox.
          * @return the arm inclination
          */
        float getBoundingBoxMaxX() {
            return bBox_maxX;
        }

        /**
          * Return the min x point of boundingbox.
          * @return the arm inclination
          */
        float getBoundingBoxMinY() {
            return bBox_minY;
        }

        /**
          * Return the min x point of boundingbox.
          * @return the arm inclination
          */
        float getBoundingBoxMaxY() {
            return bBox_maxY;
        }



    protected:
        /**
         * The individual finger ID number that is assigned to each TuioFinger.
         */
        int hand_id;

        float x_pos;
        float y_pos;

        FingerArray fingers;

        float bBox_minX;
        float bBox_minY;
        float bBox_maxX;
        float bBox_maxY;

        Class hand_class;

        float x_arm_center;
        float y_arm_center;
        float arm_minor_axis;
        float arm_major_axis;
        float arm_inclination;
    };
}
#endif
