#!/usr/bin/env python3
#  _______ _____ _            _
# |__   __/ ____| |          | |
#    | | | |    | | ___   ___| | __
#    | | | |    | |/ _ \ / __| |/ /
#    | | | |____| | (_) | (__|   <
#    |_|  \_____|_|\___/ \___|_|\_\
# The Terminal Clock
# MIT License
# Copyright (c) 2019 Haz001
# NOTE: Test code
import cli
graphics = cli.graphics()
graphics.drawloop()
run = True
import random as rn

class ui:
	class button:
		def __init__(self, w, h, text):
			self.width = w
			self.height = h
			self.text = text

		def draw(self,g,x,y,act):
			g.drawrec(x,y,13,3," ",0)
			g.drawstr(x+1,y+1,text,0)

class menu:
	a = 0
	b = 0
	title = "tClock"
	def background(g):
		g.move(0,0)
		for i in range(g.width*g.height):
			h = str(rn.randint(0,24))
			if(len(h) == 1):
				h = "0"+h
			m = str(rn.randint(0,60))
			if(len(m) == 1):
				m = "0"+m
			g.write(str(h)+":"+str(m)+" "*rn.randint(1,5),0)
	def form(g):
		l = 57
		h = 19
		menu.a = (g.width-l)/2-1
		menu.b = (g.height-h)/2 -1
		g.drawrec(menu.a,menu.b,l+2,h+2," ",4)
		g.drawstr(menu.a+1,menu.b,menu.title,4)
		g.drawrec(menu.a+1,menu.b+1,57,9,"#",0)
	def drawmenu(n):
		if(n == 0):# Main Menu
			pass
	def main(g):

		l = 57
		h = 19
		a = (g.width-l)/2+1
		b = (g.height-h)/2 -1 + 11
		for i in range(8):
			c = a+14*i
			d = b
			while (c+13)>(a+l):
				c -= 14*4
				d +=4
			g.drawrec(c,d,13,3," ",0)
			if(mv.btn == i):
				g.drawstr(c+1,d+1,"Active",0)
			else:
				g.drawstr(c+1,d+1,"Button",0)
import time
class mv:
	btn = 0
	scn = 0
while run:
	menu.background(graphics)
	menu.form(graphics)
	menu.main(graphics)
	graphics.movetobottom()
	z = graphics.grep()
	time.sleep(0.1)
	if(z == 10):
		run = False
	elif(z == 410):
		graphics.screenupdate()
	elif(z == 258):#d
		if(mv.btn < 4):
			mv.btn +=4
		else:
			mv.btn -=4
		menu.title = str(mv.btn)+"d"
	elif(z == 259):#u
		if(mv.btn >= 4):
			mv.btn -=4
		else:
			mv.btn +=4
		menu.title = str(mv.btn)+"u"
	elif(z == 260):#l
		mv.btn-=1
		if(mv.btn < 0):
			mv.btn +=1
	elif(z == 261):#r
		mv.btn+=1
	else:
		menu.title += "- "+str(z)
	#drawMenuBase(graphics)
