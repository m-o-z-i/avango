# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

from avango.menu.widget import PushButton
from avango.script import field_has_changed
import avango.menu.Preferences
import avango.osg

class CheckBox(PushButton):
    CheckState = avango.SFBool()

    def __init__(self):
        self.last_select = False
        self.init_super(PushButton)
        self.CheckState.value = False

    def init_defaults(self):
        self.Title.value = avango.menu.Preferences.checkbox_text
        self.IconFilenames.value = avango.menu.Preferences.checkbox_icons
        self.CheckState.value = False

    @field_has_changed(PushButton.Select)
    def select_changed(self):
        if not self.last_select and self.Select.value:
            self.CheckState.value = not self.CheckState.value
        self.last_select = self.Select.value

    def create_layouter(self):
        layouter = avango.menu.layout.CheckBoxLayouter(Widget=self)
        return layouter

    def cleanup(self):
        self.disconnect_all_fields()
        self.super().pushbutton_cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "CheckBox deleted"