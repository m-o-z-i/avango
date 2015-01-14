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

#if !defined(AV_DAEMON_HANDSERVICE_H)
#define AV_DAEMON_HANDSERVICE_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <avango/FieldContainer.h>
#include <avango/Singleton.h>
#include <avango/daemon/windows_specific_daemon.h>

#include <gua/math.hpp>

namespace av
{
  namespace daemon
  {

    class Hand;
    class HandSegment;

    /**
     * HandService, handles shared memory segment used to communicate with
     * Avango Daemon instance.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL HandService : public Base/*, public Singleton<HandService>*/
    {
      AV_BASE_DECLARE();

    public:

      /**
       * Public Constructor, to provide ability to construct own customized services.
       */
      HandService();
      virtual ~HandService();

      /**
       * Connects to the shared memory segment.
       */
      void connectDaemon();

      /**
       * Disconnects from the shared memory segment.
       */
      void disconnectDaemon();

      /**
       * Get matrix of a specified hand.
       */
      const ::gua::math::mat4& getMatrix(const char* hand);

      /**
       * Get value state by specified hand and number.
       */
      float getValue(const char* hand, int which);

      /**
       * Set matrix of a specified hand.
       */
      void  setMatrix(const char* hand, const ::gua::math::mat4& value);

      /**
       * Set a specfic value of a specific hand.
       */
      void  setValue(const char* hand, int which, float value);

      bool  getMatrixUsed(const char* hand);
      int   getValuesUsed(const char* hand);

    protected:

      const ::gua::math::mat4* mIdentityMatrix;
      const char*     mCachedHandName;
      Hand*           mCachedHand;
      HandSegment*    mHandSegment;

      Hand* lookupCachedHand(const char* name);
      void     clearHandCache();
    };

    typedef SingleField<Link<HandService> > SFHandService;
    typedef MultiField<Link<HandService> > MFHandService;
    typedef Singleton<HandService> DevService;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_DAEMON_DLL SingleField<Link<daemon::HandService> >;
  template class AV_DAEMON_DLL MultiField<Link<daemon::HandService> >;
#endif

}

#endif // #if !defined(AV_DAEMON_HANDSERVICE_H)
