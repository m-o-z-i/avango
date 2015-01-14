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

#include <avango/daemon/Hand.h>

#include <gua/math.hpp>
#include <cstring>

AV_BASE_DEFINE(av::daemon::Hand);

av::daemon::Hand::Hand()
  : mName(),
    mMatrix(gua::math::mat4::identity()),
    mValue(),
    mMatrixUsed(false),
    mValuesUsed(0)
{
  mName[sMaxNameLength - 1] = '\0';

  for (int i=0; i<sMaxValues; ++i) {
    mValue[i] = 0;
  }
}

/* virtual */
av::daemon::Hand::~Hand()
{}

void
av::daemon::Hand::initClass()
{
  if (!isTypeInitialized())
  {
    av::Base::initClass();
    AV_BASE_INIT(av::Base, av::daemon::Hand, true);
  }
}

const char*
av::daemon::Hand::getName() const
{
  return mName;
}

const ::gua::math::mat4&
av::daemon::Hand::getMatrix() const
{
  return mMatrix;
}

float
av::daemon::Hand::getValue(int which) const
{
  if (sMaxValues <= which)
    return 0.0f;

  return mValue[which];
}

const std::vector<float>
av::daemon::Hand::getValues() const
{
  return std::vector<float> (mValue, mValue + sMaxValues);
}

void
av::daemon::Hand::setName(const char* name)
{
  ::strncpy(mName, name, sMaxNameLength - 2);

  mName[sMaxNameLength - 1] = '\0';
}

void
av::daemon::Hand::setMatrix(const ::gua::math::mat4& matrix)
{
  mMatrix = matrix;
}

void
av::daemon::Hand::setValue(int which, float val)
{
  if (sMaxValues <= which)
    return;

  mValue[which] = val;
}

bool
av::daemon::Hand::getMatrixUsed() const
{
  return mMatrixUsed;
}

int
av::daemon::Hand::getValuesUsed() const
{
  return mValuesUsed;
}
