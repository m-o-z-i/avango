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

import avango.script
import unittest

class DoubleValue(avango.script.Script):
    Input = avango.SFInt()
    Output = avango.SFInt()

    def evaluate(self):
        self.Output.value = 2 * self.Input.value

class TestContainer(avango.script.Container):
    Input = avango.SFInt()
    Output = avango.SFInt()

    def __init__(self):
        self.init_super(avango.script.Container)

        node1 = self.register_internal_node(DoubleValue())
        node2 = self.register_internal_node(DoubleValue())
        node2.Input.connect_from(node1.Output)

        node1.Input.connect_from(self.Input, False)
        self.Output.connect_from(node2.Output, False)


class ContainerTestCase(unittest.TestCase):

    def testEvaluation(self):
        container = TestContainer()
        container.Input.value = 5
        avango.evaluate()
        self.assertEqual(20, container.Output.value)


def Suite():
    suite = unittest.TestSuite()

    ContainerTests= [
        'testEvaluation',
    ]
    suite.addTests(map(ContainerTestCase, ContainerTests))

    return suite
