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

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/Texture1D.h>
#include "OSGTexture1D.h"

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

void init_OSGTexture1D(void)
 {
  // wrapping osg::Texture1D functionality
  register_field<av::osg::SFTexture1D>("SFTexture1D");
  register_multifield<av::osg::MFTexture1D>("MFTexture1D");
  class_<av::osg::Texture1D, av::Link<av::osg::Texture1D>, bases<av::osg::Texture>, boost::noncopyable >("Texture1D", "docstring", no_init);
 }
