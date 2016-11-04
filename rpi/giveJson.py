#!/usr/bin/python

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

from bottle import route, run
import datetime
import json
import MySQLdb
import sys
import os

dir = os.path.dirname(__file__)

#input = sys.argv[1]

@route('/', method='GET')
def response_json():
	try:
		conn = MySQLdb.connect(host= "localhost",
		                  user="user",
		                  passwd="pw",
		                  db="power")
		print('connected')
		x = conn.cursor()
	except MySQLdb.Error as e:
		conn.rollback()
		print('connection failed')

	# get data from database: the last entry record
	try:
		x.execute ("""SELECT power FROM power ORDER BY id DESC LIMIT 1""")
		#x.execute ("""SELECT power FROM power WHERE id = 15""")
		conn.commit()

	except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)

	print('closed')
	conn.close()

	data = list(x.fetchall())
	print(data)
    
    #check if the value of pulses is "NULL" or absent and if it is set "power" to 0
    #then create the response
	if(len(data) == 0 or data[0][0] == None):
		resp = json.dumps({"power" : 0})
	else:
		resp = json.dumps({"power" : int(data[0][0])})


	return resp

#to run the server with bottle framework
run(host='0.0.0.0', port=8080, debug=True)




