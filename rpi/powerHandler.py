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
import sys
import os
import MySQLdb
import setBatt

dir = os.path.dirname(__file__)
last = os.path.join(dir, 'last.json')
state = os.path.join(dir, 'state.json')

# pc url
urlUnc = "http://192.168.1.2:8080/pc/data/dataFromFile.php?val=unc"
urlPv = "http://192.168.1.2:8080/pc/data/dataFromFile.php?val=pv"
urlBatt = "http://192.168.1.2:8080/pc/data/dataFromFile.php?val=batt"
urlDso = "http://192.168.1.2:8080/pc/data/dataFromFile.php?val=dso"

#Functions for turn off and on the load (through TCP/Http GET Request)
#and various functions for logging (see logging.php in pc_simulator)
def turnOff():
    urlRelay = "http://192.168.1.11/?RELAY1=OFF"
    reqOff = requests.get(urlRelay).json()

def turnOn():
    urlRelay = "http://192.168.1.11/?RELAY1=ON"
    reqOn = requests.get(urlRelay).json()

def logPV():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=0"
    reqLog = requests.get(urlLog)
    print('Asked Print0')

def logBatt():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=1"
    reqLog = requests.get(urlLog)
    print('Asked Print1')
    
def logPV_DSO():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=2"
    reqLog = requests.get(urlLog)
    print('Asked Print2')

def logOverUncLast():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=3"
    reqLog = requests.get(urlLog)
    print('Asked Print3')

def logBatt_DSO():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=4"
    reqLog = requests.get(urlLog)
    print('Asked Print4')
    
def logReqOverDso():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=5"
    reqLog = requests.get(urlLog)
    print('Asked Print5')

def logUncLastLessDSO():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=6"
    reqLog = requests.get(urlLog)
    print('Asked Print6')

def logOVER():
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=7"
    reqLog = requests.get(urlLog)
    print('Asked Print7')

def logUncProb():
    turnOFF()
    urlLog = "http://192.168.1.2:8080/pc/data/logging.php?val=8"
    reqLog = requests.get(urlLog)
    print('Asked Print8')

#get json from pc
try:
    unc = requests.get(urlUnc).json()
    pv = requests.get(urlPv).json()
    batt = requests.get(urlBatt).json()
    dso = requests.get(urlDso).json()
    topBound = dso['power']
    print('\n---')
    print('batt: ', batt['power'])
    print('unc: ', unc['power'])
    print('pv: ', pv['power'])
    print('dso:', dso['power'])
    print('---\n')

except requests.HTTPError as e:
    print('HTTP ERROR %s occured' % e.code)
    print(e.args[0].args[1])
    sys.exit(1)
except requests.exceptions.ConnectionError as e:
    print(e.args[0].args[1])
    sys.exit(1)


try:
	# connect to database
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
	x.execute ("""SELECT power FROM power ORDER BY id DESC LIMIT 1""")
	conn.commit()

except MySQLdb.Error as e:
	conn.rollback()
	print('query failed:', e)

conn.close()

dataLamp = list(x.fetchall())

with open(last) as lastfile:
	lastPos = json.load(lastfile)

with open(state) as statefile:
	stateVal = json.load(statefile)

#get value of lamp from dict, "None" case is from NULL of Database query
if(dataLamp[0][0] == None):
    lamp = 0
else:
    lamp = dataLamp[0][0]

requestPower = unc['power']+lamp

print('lamp:', lamp)
print('req:', requestPower)
print('\n')

if(isinstance(unc['power'], (int))):
    # first check if there is enough PV power
    # turnOn() the lamp
    if(pv['power'] > requestPower):
        turnOn()
        print('\n***\nPV')
        if(stateVal["state"] == 0  or stateVal["type"] != 0):
            logPV()
        print(lastPos['power'],'State: ON')
        with open(state, "w") as statefile:
            statefile.write(json.dumps({"state":1, "type": 0}))
        if(pv['power'] - lamp > 0):
            newBatt = batt['power'] + ((pv['power'] - lamp)/60)
            if(newBatt > 1500):
                setBatt.setNew(1500)
            else:
                setBatt.setNew(newBatt)
    # second check if there is enough Battery power
    # turnOn() the lamp
    # and set the new battery value
    elif((batt['power'] - (requestPower/60)) > 0 ):
        turnOn()
        newBatt = batt['power'] - (requestPower/60)
        if(newBatt >= 0):
            setBatt.setNew(newBatt)
        print('\n***\nBatt')
        if(stateVal["state"] == 0 or stateVal["type"] != 1):
            logBatt()
        print(lastPos['power'],'State: ON')
        with open(state, "w") as statefile:
            statefile.write(json.dumps({"state":1, "type": 1}))
    #third level, check if requested power is higher then dso, in case turnOff(). But before
	#do some checks
    elif(requestPower >= topBound):
		#if pv + dso is higher the requested, then turn on the load
        if(pv['power']+topBound > requestPower):
            turnOn()
            print('\n***\nPV+DSO -> ON')
            if(stateVal["state"] == 0  or stateVal["type"] != 2):
                logPV_DSO()
            print(lastPos['power'],'State: ON')
            with open(state, "w") as statefile:
                statefile.write(json.dumps({"state":1, "type": 2}))
			#if there is an overcome of energy, use it for load the battery
            if(pv['power'] - lamp > 0):
                newBatt = batt['power'] + ((pv['power'] - lamp)/60)
                if(newBatt > 1500):
                    setBatt.setNew(1500)
                else:
                    setBatt.setNew(newBatt)
		#if batt + dso is higher the requested, then turn on the load
        elif( batt['power'] + topBound > requestPower):
            # if request is within limit, then check if it will not pass the top bound
            #with the lamp on (adding last value of power)
            if((unc['power']+lastPos['power']) > topBound):
                turnOff()
                print('\n***\nUNC+Last over bounds -> OFF')
                if(stateVal["state"] == 1 or stateVal["type"] != 3):
                    logOverUncLast()
                print((unc['power']+dataLamp[0][0]),'State: OFF')
                with open(state, "w") as statefile:
                    statefile.write(json.dumps({"state":0, "type": 3}))
            else:
                turnOn() 
                if(batt['power'] - (requestPower/60) > 0):
                    newBatt = batt['power'] - (requestPower/60)
                    setBatt.setNew(newBatt)
                else:
                    newBatt = 0
                    setBatt.setNew(newBatt)
                print('\n***\nBatt+DSO -> ON')
                if(stateVal["state"] == 0 or stateVal["type"] != 4):
                    logBatt_DSO()
                print(lastPos['power'],'State: ON')
                with open(state, "w") as statefile:
                    statefile.write(json.dumps({"state":1, "type": 4}))
		#negative case, so turn off the controlled load
        else:
            turnOff()
            print('\n***\nRequested power greater then DSO.')
            if(stateVal["state"] == 1 or stateVal["type"] != 5):
                logReqOverDso()
            print(dataLamp[0][0],'State: OFF')
            with open(state, "w") as statefile:
                statefile.write(json.dumps({"state":0, "type": 5}))    
    # if request is within limit, then check if it will not pass the top bound
    #with the lamp on (adding last value of power)
    elif((unc['power']+lastPos['power']) < topBound):
        turnOn()
        print('\n***\nUnc+Last < DSO -> ON')
        if(pv['power'] > 0):
            newBatt = batt['power'] + ((pv['power'] - lamp)/60)
            if(newBatt > 1500):
                setBatt.setNew(1500)
            else:
                setBatt.setNew(newBatt)
        if(stateVal["state"] == 0 or stateVal["type"] != 6):
            logUncLastLessDSO()
        print(lastPos['power'],'State: ON')
        with open(state, "w") as statefile:
            statefile.write(json.dumps({"state":1, "type": 6}))
    #If not one of the previous, turnOff lamp.
    else:
        turnOff()
        print('\n***\nDefinitely OverBounds -> OFF')
        if(stateVal["state"] == 1 or stateVal["type"] != 7):
            logOVER()
        print((unc['power']+dataLamp[0][0]),'State: OFF')
        with open(state, "w") as statefile:
            statefile.write(json.dumps({"state":0, "type": 7}))
#when the unc value is NULL or not int
else:
    print('\n***\nUNC not int, ERROR')
    logUncProb()
    reqLog = requests.get(urlLog)
    with open(state, "w") as statefile:
        statefile.write(json.dumps({"state":1, "type": 8}))

