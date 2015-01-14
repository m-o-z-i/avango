#ifndef HAND_H
#define HAND_H

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

/**
 * \file
 * \ingroup av_daemon
 */

#include <vector>
#include <gua/math/math.hpp>
#include <avango/Base.h>
#include <avango/daemon/windows_specific_daemon.h>

namespace av
{
  namespace daemon
  {
    /**
     * Class representing a hand of Avango NG daemon.
     */
    class AV_DAEMON_DLL Hand : public Base {

      AV_BASE_DECLARE();

    public:

      /**
       * Constructor
       */
      Hand();

      /**
       * Destructor
       */
      virtual ~Hand();

    public:

      const char* getName() const;
      const ::gua::math::mat4& getMatrix() const;

      float getValue(int which) const;
      const ::std::vector<float> getValues() const;

      void setName(const char* name);
      void setMatrix(const ::gua::math::mat4& matrix);

      void setValue(int which, float val);

      bool getMatrixUsed() const;
      int  getValuesUsed() const;

      static const int sMaxNameLength = 128;
      static const int sMaxValues = 256;

    private:
      // retain static memory layout since it gets put into a shared
      // memory segment not autogrowable!
      char     mName[sMaxNameLength];
      ::gua::math::mat4 mMatrix;               // this should work since fpMatrix has the same memory layout float[4][4]
      float    mValue[sMaxValues];
      bool     mMatrixUsed;
      int      mValuesUsed;

      /**
       * Made private to prevent copying construction.
       */
      Hand(const Hand&);

      /**
       * Made private to prevent assignment.
       */
      const Hand& operator=(const Hand&);

    };
  }
}

#endif // HAND_H
