import time
import os
from time import sleep
char = 9608
leng = 78
heig = 12
x = True
sdelay = 0
while x:
	print("0 - Flash\n1 - Scroll Slow\n2 - Scroll Own Delay\n3 - Scroll No Delay\n4 - Quit")


	dtype = int(input(">"))
	if (dtype == 2):
		sdelay = float(input("Scroll Delay"))
	if(dtype >= 0 and dtype < 4):
		x = False
	elif(dtype == 4):
		exit()
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
				string+=chr(char)
			else:
				string+=" "
		if (dtype == 0):
			string +="\n"
		elif (dtype >= 1):
			print(string)
			string=""
			if (dtype == 1):
				sleep(0.01)
			else:
				sleep(sdelay)
	if (dtype == 0):
		os.system("clear")
		print(string)
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
from datetime import datetime as dt
g = grid()
g.setup(leng,heig)
now = None
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
		number(g,28,hi,m1)
		number(g,40,hi,m2)
		seco = dt.now().second
		s1 = str(seco)[0]
		if len(str(seco))>1:
			s2 = str(seco)[1]
		else:
			s2 = s1
			s1 = 0
		s1 = int(s1)
		s2 =int(s2)
		number(g,54,hi,s1)
		number(g,66,hi,s2)
		draw(g)
		now = dt.now().second
print(dt.now().hour)

# number(g,2,2,1)
# number(g,14,2,2)
# number(g,26,2,3)
# number(g,38,2,4)
# number(g,50,2,5)
# number(g,62,2,6)
# number(g,74,2,7)
# number(g,86,2,8)
# number(g,98,2,9)
# number(g,110,2,0)



# while True:
# 	sleep(0.1)
# 	draw(g)
