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

#include <boost/bind.hpp>

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  av::Logger& logger(av::getLogger("TestFieldConnectionChangeInNotify"));

  class MyObject : public av::IntObject
  {
    AV_FC_DECLARE();

  public:

    MyObject();
    void valueChanged(const av::SFInt::ChangedEvent event);

  };

  AV_FC_DEFINE(MyObject);

  /* static */ void MyObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::IntObject::initClass();
      AV_FC_INIT(av::IntObject, MyObject, true);
    }
  }

  MyObject::MyObject()
  {
    Value.addChangedCallback(boost::bind(&MyObject::valueChanged, this, _1));
  }

  void MyObject::valueChanged(const av::SFInt::ChangedEvent event)
  {
    if (event.getTriggeredFromField() && Value.getValue() == 1)
    {
      logger.info() << "fieldHasChanged: disconnecting from source field";
      Value.enableNotify(false);
      Value.disconnectFrom(event.getTriggeredFromField());
      Value.enableNotify(true);
    }
    if (event.getTriggeredFromField() && Value.getValue() == 3)
    {
      logger.info() << "fieldHasChanged: disconnecting all fields from source field";
      Value.enableNotify(false);
      event.getTriggeredFromField()->disconnectAuditors();
      Value.enableNotify(true);
    }
  }

  TEST(fieldConnectionChangeInNotify)
  {
    MyObject::initClass();

    av::Link<av::IntObject> src(new av::IntObject);
    av::Link<MyObject> dst1(new MyObject);
    av::Link<av::IntObject> dst2(new av::IntObject);

    src->Value.setValue(0);
    dst1->Value.setValue(0);
    dst2->Value.setValue(0);

    dst1->Value.connectFrom(&src->Value);
    dst2->Value.connectFrom(&src->Value);

    src->Value.setValue(1);
    CHECK_EQUAL(1, dst1->Value.getValue());
    CHECK_EQUAL(1, dst2->Value.getValue());

    src->Value.setValue(2);
    CHECK_EQUAL(1, dst1->Value.getValue());
    CHECK_EQUAL(2, dst2->Value.getValue());

    dst1->Value.connectFrom(&src->Value);

    src->Value.setValue(3);
    CHECK_EQUAL(3, dst1->Value.getValue());
    CHECK_EQUAL(3, dst2->Value.getValue());

    src->Value.setValue(4);
    CHECK_EQUAL(3, dst1->Value.getValue());
    CHECK_EQUAL(3, dst2->Value.getValue());

    av::ApplicationInstance::get().evaluate();
  }
}
