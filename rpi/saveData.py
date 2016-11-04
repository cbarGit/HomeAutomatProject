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

import MySQLdb
import sys
import time
import datetime
import getArduino
import six
import mail

#call the "getArduino.py" module to get pulse value from Arduino.
p_val = getArduino.main()

#print('p_val = ', p_val, type(p_val))
try:
	conn = MySQLdb.connect(host= "localhost",
	                  user="user",
	                  passwd="pw",
	                  db="power")
	x = conn.cursor()
except MySQLdb.Error as e:
	conn.rollback()
	print('connection failed')

# check if the response from arduino is an int (so the response went well), 
#if it is save the number of pulses in the data table of database, otherwise 
#save as "NULL" and send a mail of warning (with the error returned from 
#getArduino.py") with the proper "now" timestamp 
if isinstance(p_val, int):
	try:
		x.execute("""INSERT INTO data VALUES (NULL, %s, %s, NULL)""",(str(1),p_val))
		#print('inserted')
		conn.commit()

	except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)
else:
	try:
		x.execute("""INSERT INTO data VALUES (NULL, %s, NULL, NULL)""",(str(1)))
		#print('inserted')
		conn.commit()

	except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)
	print(p_val)
	now = datetime.datetime.now()
	mess = 'Arduino Connection Error' + ' at time ' + now.strftime("%H:%M") + ' of ' + now.strftime("%d-%m-%Y") + ':\n' + str(p_val)
	mail.sendMail('your@mail.com', mess)
	sys.exit(1)

conn.close()
