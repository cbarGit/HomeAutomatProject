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

import smtplib
import datetime
import confMail

# send mail to for alarm with smtp protocol, passing through gmail
# "confMail.py" is used to set the user and pw of the sender email
def sendMail(account, text):
	user = str(confMail.username)
	pw = str(confMail.pw)
	date = datetime.datetime.now().strftime( "%d/%m/%Y %H:%M" )
	subj = "RPi Alarm"
	from_addr = "RPi Alarm"
	to_addr = account
	message_text = text
	msg = "From: %s\nTo: %s\nSubject: %s\nDate: %s\n\n%s" % ( from_addr, to_addr, subj, date, message_text )
	print(date)

	server = smtplib.SMTP('smtp.gmail.com:587')
	server.starttls()
	server.login(user, pw)

	server.sendmail(from_addr, to_addr, msg)
	server.quit()
