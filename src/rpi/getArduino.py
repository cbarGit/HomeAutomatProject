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

import json
import requests

# url of Arduino connected to the impulse counter
url = "http://192.168.1.10"

#get json and treat it as a dict, then get and return the value of pulses
#otherwise return the error 
def main():
	try:
		req = requests.get(url).json()
		#print(req)
		#print(type(req))
		return int(req['pulses'])
	except requests.exceptions.ConnectionError as e:
		print(e.args[0].args[1])
		return e.args[0].args[1]


if __name__ == "__main__":
    main()
