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

#if !defined(AVANGO_OSG_INIT_H)
#define AVANGO_OSG_INIT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/Typed.h>

#include <avango/osg/Types.h>

namespace av
{
  namespace osg
  {
    /**
     * Initializes the Avango OpenSceneGraph library.
     * Usage: \code av::osg::Init::initClass(); \endcode
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Init
    {
      AV_TYPED_DECLARE_ABSTRACT();
    };
  }
}

#endif
