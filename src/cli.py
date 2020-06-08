#!/usr/bin/env python3
#   ____ _     ___
#  / ___| |   |_ _|  _ __  _   _
# | |   | |    | |  | '_ \| | | |
# | |___| |___ | | _| |_) | |_| |
#  \____|_____|___(_) .__/ \__, |
#                   |_|    |___/
# Command Line Interface Class
# MIT License
# Copyright (c) 2019 Haz001
import os
mode = 0

import platform
if(os.name == "posix"):
	import curses
	mode = 1
else:
	mode = 0


class graphics:

	def screenupdate(self):
		if(platform.system().lower() == "windows"):
			a = os.popen("mode con",'r').read()
			b = a.split("\n")
			c = b[4].split(" ")
			d = ""
			t = 0
			for i in range(len(c)):
				try:
					int(c[i])
					d = c[i]
				except:
					t+=1
					if(t >= 100):
						d = 100
			self.width = d
			self.height = d*0.3
		elif(os.name == "posix"):
			height, width = os.popen('stty size','r').read().split()
			self.width = int(width)
			self.height = int(height)
		else:
			self.width = input("Width of terminal enviroment\n>")
			self.height = input("Height of terminal enviroment\n>")
		self.clear()
	def __init__(self,instantflip = True):
		self.flip = instantflip
		if(platform.system().lower() == "windows"):
			a = os.popen("mode con",'r').read()
			b = a.split("\n")
			c = b[4].split(" ")
			d = ""
			t = 0
			for i in range(len(c)):
				try:
					int(c[i])
					d = c[i]
				except:
					t+=1
					if(t >= 100):
						d = 100
			self.width = d
			self.height = d*0.3
			os.system("call conSize "+str(self.width)+" "+str(self.height)+" 9999")
		elif(os.name == "posix"):
			height, width = os.popen('stty size','r').read().split()
			self.width = int(width)
			self.height = int(height)
		else:
			self.width = input("Width of terminal enviroment\n>")
			self.height = input("Height of terminal enviroment\n>")
		pass
	def drawloop(self):
		self.screen = curses.initscr()
		curses.start_color()
		curses.noecho()
		curses.cbreak()
		self.screen.keypad(True)
		# 0 - WoB
		# 1 - BoW
		# 2 - RoB
		# 3 - BoR
		# 4 - RoW
		# 5 - WoR
		curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_WHITE)
		curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
		curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_RED)
		curses.init_pair(4, curses.COLOR_RED, curses.COLOR_WHITE)
		curses.init_pair(5, curses.COLOR_WHITE, curses.COLOR_RED)


	def draw(self,x,y,char,color):
		x = int(x)
		y = int(y)
		if((x in range(self.width+1)) and (y in range(self.height+1))):
			try:
				self.screen.addstr(y,x,char,curses.color_pair(color))
			except curses.error as e:
				pass

	def drawrec(self,x,y,w,h,char,color):
		for i in range(int(w)):
			for j in range(int(h)):
				self.draw(x+i,y+j,char,color)

	def write(self,char,color):
		try:
			self.screen.addstr(char,curses.color_pair(color))
		except curses.error as e:
			pass
	def drawstr(self,x,y,string,color):
		lines = string.split("\n")
		for i in range(len(lines)):
			line = lines[i]
			for j in range(len(line)):
				self.draw(x+j,y+i,line[j],color)

	def clear(self):
		for x in range(self.width):
			for y in range(self.height):
				self.draw(x,y," ",0)
	def move(self,x,y):
		self.screen.move(y,x)
	def movetobottom(self):
		self.screen.move(self.height-1, self.width-1)
	def grep(self):
		x = self.screen.getch()
		return x
