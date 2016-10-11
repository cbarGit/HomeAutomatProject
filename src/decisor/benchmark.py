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
import numpy as np
import matplotlib.pyplot as plt
import sys


#####PRINT#####
def pp(ob):
	for i in range(0,rows):
		print(i, matrix[i])

######################################################
############## data json input ###################

with open('params.json') as data_file:    
    data = json.load(data_file)

######################################################
############### data creation ###############

summPerc1 = 0
summPerc2 = 0
summPerc3 = 0
summPerc4 = 0
weight = 1
val = 0
rows = len(data.keys())

orig_stdout = sys.stdout
f = open('results.txt', 'a')###open file to write(append)
sys.stdout = f

########### import data from json #############

k = []
ki = []
k = list(data.keys())
for i in k:
	ki.append(int(i))
ki.sort()


for n,p in enumerate(range(1,3301)):
	power = p
	
	########### create data arrays ###############
	matrix = np.ndarray((rows,12),dtype = object)
	matrix[:,0:] = int(0)
	matrix[:,2] = float(0)

	for key,a in zip(ki,matrix):
		a[0] = data[str(key)]
	
	#### power calculation ######
	for a in matrix:
		a[1] = a[0] * power


	##### pulse calculation at seconds ##########
	for a,b in zip(matrix[1:],matrix):
		a[2] = float("{0:.3f}".format(b[2] + a[1]/60))

	##### pulse calculation at minutes ##########
	for a,b in zip(matrix[1:],matrix):
		a[3] = int(b[2] + a[1]/60)

	#############################################
	##################stima1#####################

	for a,b in zip(matrix[1:],matrix):
		if(a[0] == 1):
			a[4] = (a[3] - b[3])*60

	for i in matrix:
		if(i[0]>0):
			i[5] = abs(i[4]-i[1])

	#####################################
	#############stima2#################


	pos = 6
	for i in range(0,rows):
		diff = 0
		if (i >= 6):
			#for a in range(-5, 0):
			diff = matrix[i][3] - matrix[i-pos][3]
			matrix[i][6] = diff*60/6

	for i in matrix:
		if(i[0]>0):
			i[7] = abs(i[6]-i[1])



	##################################
	############ stima3 ##############

	buff = []
	for i in range(0, rows):
		summ = 0
		if( matrix[i][3] - matrix[i-1][3] != 0 ):
			if(i > 1 and (matrix[i][3] != 0)):
				buff.append((matrix[i][3] - matrix[i-1][3])*60)
			posi = list(filter(lambda x: x > 0, buff))
			summ = sum(posi)
			if(len(posi)!=0):
				matrix[i][8] = summ/len(posi)
		else:
			buff = []
			matrix[i][8] = 0

	for i in matrix:
		if(i[0]>0):
			i[9] = abs(i[8]-i[1])


	##########################################################
	##################### stima 4 ############################

	c = 1
	temp = 0
	flag = 0
	max_counter = 1
	for i in range(0,rows):
		if(matrix[i][3]!=0):
			
			if( matrix[i][3] != matrix[i-1][3] ):
				c = 0
				temp = matrix[i][8]
				if(matrix[i-1][3] == 0):
					flag = 1
				else:
					flag = 0
					matrix[i][10] = temp/max_counter
				

			else:
				if(flag == 1):
					max_counter += 1

				matrix[i][10] = temp/max_counter
				c += 1

				if(c> max_counter):
					matrix[i][10] = 0

			
	for i in matrix:
		if(i[0]>0):
			i[11] = abs(i[10]-i[1])

	############################################################
	############### final calculation ##########################
	real = []
	stima1 = []
	stima2 = []
	stima3 = []
	stima4 = []

	for i in matrix:
		real.append(i[1])
		stima1.append(i[5])
		stima2.append(i[7])
		stima3.append(i[9])
		stima4.append(i[11])

	tot1=0
	tot2=0
	tot3=0
	tot4=0
	for i in stima1:
		tot1 += abs(i)
	for i in stima2:
		tot2 += abs(i)
	for i in stima3:
		tot3 += abs(i)
	for i in stima4:
		tot4 += abs(i)

	stima1Err = float("{0:.3f}".format(tot1/120))
	stima2Err = float("{0:.3f}".format(tot2/120))
	stima3Err = float("{0:.3f}".format(tot3/120))
	stima4Err = float("{0:.3f}".format(tot4/120))

	stima1ErrPerc = float("{0:.3f}".format((stima1Err/power)*100))
	stima2ErrPerc = float("{0:.3f}".format((stima2Err/power)*100))
	stima3ErrPerc = float("{0:.3f}".format((stima3Err/power)*100))
	stima4ErrPerc = float("{0:.3f}".format((stima4Err/power)*100))

	if (p <= 49):
		weight = 1
	
	if((p > 49) and (p % 100 == 0)):
		weight += 1

	avarageWeightErr1 = float("{0:.3f}".format(stima1ErrPerc * weight))
	avarageWeightErr2 = float("{0:.3f}".format(stima2ErrPerc * weight))
	avarageWeightErr3 = float("{0:.3f}".format(stima3ErrPerc * weight))
	avarageWeightErr4 = float("{0:.3f}".format(stima4ErrPerc * weight))

	if(avarageWeightErr1 > 100):
		avarageWeightErr1 = 0
	if(avarageWeightErr2 > 100):
		avarageWeightErr2 = 0
	if(avarageWeightErr3 > 100):
		avarageWeightErr3 = 0
	if(avarageWeightErr4 > 100):
		avarageWeightErr4 = 0

	summPerc1 += avarageWeightErr1
	summPerc2 += avarageWeightErr2
	summPerc3 += avarageWeightErr3
	summPerc4 += avarageWeightErr4

	############################################################
	###### data output  ################################


	print(n+1, stima1Err, stima2Err, stima3Err, stima4Err, stima1ErrPerc,
		stima2ErrPerc,stima3ErrPerc,stima4ErrPerc,'##',weight,'##',avarageWeightErr1,avarageWeightErr2,avarageWeightErr3,avarageWeightErr4)


	val += weight


############################################################
################ decisor ###################################


print('The final values are:')
print(summPerc1/val,summPerc2/val,summPerc3/val,summPerc4/val)

ind = np.argmin([summPerc1/val,summPerc2/val,summPerc3/val,summPerc4/val]) 

print('...and the winner is')

if (ind == 0):
	print(' -> STIMA 1 <- ')
elif (ind == 1):
	print(' ->  STIMA 2  <- ')
elif (ind == 2):
	print(' -> STIMA 3 <- ')
else:
	print(' -> STIMA 4 <- ')

sys.stdout = orig_stdout
f.close()
