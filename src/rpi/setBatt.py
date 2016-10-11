#! /usr/bin/python

# Copyright (C) 2016 Francesco Bartolini
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

import pycurl
import sys

#set new battery values qith pycurl module
def setNew(newBatt):
    c = pycurl.Curl()
    c.setopt(c.URL, 'http://192.168.1.2:8080/pc/data/batt.json.php')
    c.setopt(c.POSTFIELDS, 'batt=%s' % (newBatt))
    c.setopt(c.VERBOSE, False)
    c.perform()

