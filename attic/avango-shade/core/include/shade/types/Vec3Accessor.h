// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#ifndef shade_types_Vec3Accessor_H
#define shade_types_Vec3Accessor_H shade_types_Vec3Accessor_H

#include "TypeAccessor.h"

namespace shade
{
  namespace types
  {
    class Vec3Accessor : public TypeAccessor
    {
    public:
      virtual void set(float x, float y, float z) = 0;
      virtual void get(float& x, float& y, float& z) const = 0;

      /*virtual*/ HashType hash(void) const;
    private:
      static HashValue m_hash;
    };
  }
}


#endif /* shade_types_Vec3Accessor_H */
