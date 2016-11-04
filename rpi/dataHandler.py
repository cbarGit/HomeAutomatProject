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

import datetime
import json
import MySQLdb
import sys
import time
import os

# value needed for calculations (read down for clarity)
imp = 60

#set the relative path 
dir = os.path.dirname(__file__)
#"buff.json" is used as a temporary buffer for information used by the
#algorithm. It is a json with two item of key/value pair.
#"estimate" key is for the cumulative estimate value of the past calculation. 
#"past" key is for the number of increment of pulses.
filename = os.path.join(dir, 'buff.json')
last = os.path.join(dir, 'last.json')

# get "past" and "estimate"
with open(filename) as data_file:    
    estimate = json.load(data_file)

# connect to database
try:
	conn = MySQLdb.connect(host= "localhost",
	                  user="user",
	                  passwd="pw",
	                  db="power")
	#print('connected')
	x = conn.cursor()
except MySQLdb.Error as e:
	conn.rollback()
	print('connection failed')

# get data from database: the last 10 min values (and timestamp)
try:
	x.execute ("""SELECT value,tstamp FROM data WHERE tstamp > NOW() - INTERVAL 5 MINUTE""")
	conn.commit()

except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)

# elaborate data from 'tuple of tuples' to 'list of tuples'
data = list(x.fetchall())

# check if we have enough records or if the last value is NULL (and then None)
# If it is, save NULL in power table and exit.
if(len(data) <= 1 or data[-1][0] == None):
	alarm = "ERROR!: No enough records\n"
	print(alarm)
	try:
		x.execute ("""INSERT INTO power VALUES (NULL, %s, NULL, NULL)""",(str(1)))
		conn.commit()
	except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)

	print('Set NULL in data table')
	sys.exit(1)
else:
	print('Records fetched: ', len(data))

# list for pulses
pulses = []

# get pulses and timestamp
for i in range(1,3):
	pulses.append(data[-i][0])
	print('Pulses:',data[-i][0], 'at', data[-i][1])

# get from estimate
for key in estimate.keys():
	print(key,'->',estimate[key])
# est -> cumulative estimate
# past -> num of incremented pulses
est = estimate["estimate"]
past = estimate["past"]

# if the values of pulses is 0, set power to zero
if(pulses[1] == 0 and pulses[0] == 0):
	power = 0

# calc power: [last - (last-1)] * 60 
# 60 is the num of seconds that rappresent the consume of 1 W of energy
# if there is no diff of pulses set pwer to 0
print('pulses [0]:',pulses[0],', pulses [1]:',pulses[1])
if(abs(pulses[1]-pulses[0]) != 0 ):
	power = abs(pulses[1]-pulses[0])*imp
else:
	power = 0

#print('power not weight:',power)

# final calculation:
# case 1: when the cumulative estimate is 0.
# case 2: when there is no difference of pulses between the last two values.
# case 3: all the other conditions
if(estimate["estimate"]==0):
	print('case 1')
	FINAL = int(power)
	estimate["past"] = 1
elif(power==0):
	print('case 2, no impulse diff')
	FINAL = int(power)
	estimate["past"] = 0
else:
	print('case 3')
	FINAL = int((power + est)/(past+1))
	estimate["past"] = past+1

print(FINAL)
# if power was setted to 0, set "estimate" to 0, otherwise sum it to the
#present value
if(power == 0):
	estimate["estimate"] = 0
	estimate["past"] = 0
else:
	estimate["estimate"] = est+power

# write to json
with open(filename, "w") as update_json:
    update_json.write(json.dumps(estimate))

#write to last.json file the new max value registered (obviously only if it's greater the last one)
if(FINAL > 0):
	with open(last) as last_file:    
		lastVal = json.load(last_file)
		if (lastVal['power'] < FINAL):
			with open(last, "w") as update_last_json:
				update_last_json.write(json.dumps({"power":FINAL}))

# get the timestamp of last value
t = data[-1][1].strftime('%Y-%m-%d %H:%M:%S')

#print(t)

# save the appropriate values to the power database
try:
	x.execute ("""INSERT INTO power VALUES (NULL, %s, %s, %s)""",(str(1), str(FINAL), t))
	#x.execute ("""UPDATE power SET power=%s, tstamp=%s;""",(str(FINAL), t))
	conn.commit()
	print('power table updated')

except MySQLdb.Error as e:
		conn.rollback()
		print('query failed:', e)

#print('closed')
conn.close()

