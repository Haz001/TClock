#!/usr/bin/env python3
import time
import os
import sys
from time import sleep
char = 9608
blockstart = '\x1b[7;30;39m'
block = " "
blockend = '\x1b[0m'
leng = 80
heig = 12
x = True
sdelay = 0
dtype = 1
clearc = ""
from sys import platform
if platform == "linux" or platform == "linux2":
	clearc ="clear"
elif platform == "darwin":
	clearc ="clear"
elif platform == "win32":
	clearc ="cls"
def menu():
	x = True
	while x:
		print("1 - Flash [Default]\n2 - Scroll Slow\n3 - Scroll Own Delay\n4 - Scroll No Delay\n5 - Settings\n40- Quit")
		dtype = int(input(">"))
		if (dtype == 3):
			sdelay = float(input("Scroll Delay"))
		if(dtype >= 1 and dtype < 5):
			x = False
		elif(dtype == 0):
			exit()
	main()
class grid(object):
	grid = {}
	width = 0
	height = 0
	def setup(self,w,h):
		width = w
		height = h
		for x in range(w):
			for y in range(h):
				self.grid[str(x)+" - "+str(y)] = True
	def get(self,x,y):
		return self.grid[str(x)+" - "+str(y)]
	def set(self,x,y,b):
		self.grid[str(x)+" - "+str(y)] = b
def draw(scr):
	string = str("")
	for y in range(heig):
		for x in range(leng):
			if(scr.get(x,y)):
				string += blockstart+block+blockend
				string  = string.replace(blockend+blockstart,"")
			else:
				string+=" "
		if (dtype == 1):
			string +="\n"
		elif (dtype >= 2 and dtype != 5):
			print(string)
			string=""
			if (dtype == 3):
				sleep(0.01)
			else:
				sleep(sdelay)
	if (dtype == 1):
		os.system(clearc)
		print(string)

	elif(dtype == 5):
		print(string)
		print("dtype=5? dtype="+str(dtype))
		exit()
def number(scr,x,y,num):
	fill(scr,x,y,10,10,False)
	if (num == 1):
		fill(scr,x+3,y+1,3,1,True)
		fill(scr,x+5,y+1,1,8,True)
	elif (num == 2):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+8,y+1,1,4,True)
		fill(scr,x+1,y+4,8,1,True)
		fill(scr,x+1,y+4,1,4,True)
		fill(scr,x+1,y+8,8,1,True)
	elif (num == 3):
		fill(scr,x+8,y+1,1,8,True)
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+4,8,1,True)
		fill(scr,x+1,y+8,8,1,True)
	elif (num == 4):
		fill(scr,x+1,y+1,1,4,True)
		fill(scr,x+1,y+5,8,1,True)
		fill(scr,x+6,y+1,1,8,True)
		fill(scr,x+1,y+1,1,1,True)
	elif (num == 5):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+1,1,4,True)
		fill(scr,x+1,y+4,8,1,True)
		fill(scr,x+8,y+4,1,4,True)
		fill(scr,x+1,y+8,8,1,True)
	elif (num == 6):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+1,1,8,True)
		fill(scr,x+1,y+8,8,1,True)
		fill(scr,x+8,y+4,1,4,True)
		fill(scr,x+1,y+4,8,1,True)
	elif (num == 7):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+8,y+2,1,1,True)
		fill(scr,x+7,y+3,1,1,True)
		fill(scr,x+6,y+4,1,1,True)
		fill(scr,x+5,y+5,1,1,True)
		fill(scr,x+4,y+6,1,1,True)
		fill(scr,x+3,y+7,1,1,True)
		fill(scr,x+2,y+8,1,1,True)
	elif (num == 8):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+1,1,8,True)
		fill(scr,x+8,y+1,1,8,True)
		fill(scr,x+1,y+8,8,1,True)
		fill(scr,x+1,y+4,8,1,True)
	elif (num == 9):
		fill(scr,x+1,y+8,8,1,True)
		fill(scr,x+8,y+1,1,8,True)
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+1,1,4,True)
		fill(scr,x+1,y+4,8,1,True)
	elif (num == 0):
		fill(scr,x+1,y+1,8,1,True)
		fill(scr,x+1,y+1,1,8,True)
		fill(scr,x+8,y+1,1,8,True)
		fill(scr,x+1,y+8,8,1,True)
def fill(scr, x,y,w,h,b):
	x = int(x)
	y = y
	h = h
	w = int(w)
	for i in range(w):
		for j in range(h):
			scr.set(x+i,j+y,b)
def main():#
	print(dtype)
	from datetime import datetime as dt
	print(dtype)
	g = grid()
	g.setup(leng,heig)
	now = None
	print(dtype)
	input()
	while True:
		if(now != dt.now().second):
			hi = int((heig-10)/2)
			hour = dt.now().hour
			h1 = str(hour)[0]
			if len(str(hour))>1:
				h2 = str(hour)[1]
			else:
				h2 = h1
				h1 = 0
			h1 = int(h1)
			h2 = int(h2)
			number(g,2,hi,h1)
			number(g,14,hi,h2)
			minu = dt.now().minute
			m1 = str(minu)[0]
			if len(str(minu))>1:
				m2 = str(minu)[1]
			else:
				m2 = m1
				m1 = 0
			m1 = int(m1)
			m2 = int(m2)
			number(g,29,hi,m1)
			number(g,41,hi,m2)
			seco = dt.now().second
			s1 = str(seco)[0]
			if len(str(seco))>1:
				s2 = str(seco)[1]
			else:
				s2 = s1
				s1 = 0
			s1 = int(s1)
			s2 =int(s2)
			number(g,56,hi,s1)
			number(g,68,hi,s2)
			draw(g)
			now = dt.now().second
			print(dtype)
			if(dtype == 5):
				exit()
	print(dt.now().hour)
from pathlib import Path
def init():
	home = str(Path.home())
	conf = home+"/.config/tClock/"
	args = (sys.argv)
	print(len(args))
	if(len(args) <= 1):
		main()
	elif(len(args) == 2):

		if((args[1] == "-h")or(args[1] == "--help")):
			print("""Usage: tclock [OPTION] [FILE, DATE]...\n\nno optin\t\tDefault settings\n-m\t\t\tCLI Menu\n-g\t\t\tGitHub Link\n\x1b[1;10;41m\nIn Development\n-i\t\t\tOne Instance (good for 'watch' command or just to check time) [InDevelopment]\n-w [FILE]\t\tWrite out to file [InDevelopment]\n-c [DATE]\t\tCount Down to that date [InDevelopment]\n-g\t\t\tGraphical Mode [InDevelopment]\n\x1b[0m""")
		elif(args[1] == "-i"):
			dtype = 5
			print(dtype)
			main()
			

		elif(args[1] == "-m"):
			menu()

init()
def col():
	for a in (range(0,20)):
		for i in range(10,50):
			s=""
			for j in range(10,50):
				s+=("\x1b["+str(a)+";"+str(i)+";"+str(j)+"m"+str(a)+";"+str(i)+";"+str(j)+"\x1b[0m")+"|"
			print(s)
