#!/usr/bin/env python3
#  _ __ ___   ___ _ __  _   _   _ __  _   _
# | '_ ` _ \ / _ \ '_ \| | | | | '_ \| | | |
# | | | | | |  __/ | | | |_| |_| |_) | |_| |
# |_| |_| |_|\___|_| |_|\__,_(_) .__/ \__, |
#                              |_|    |___/
# Menu
# MIT License
# Copyright (c) 2019 Haz001
import platform
if(platform.system() == "Linux"):
	import curses
else:
	print("Basic Menu")

class menu:
	class scene:
		def __init__(self,name,title,buttons):
			self.name = name
			self.title = title
			self.element = buttons
			self.felement = 0
		def inc(self,n):
			self.felement += n
			if (self.felement > len(self.element)-1):
				self.felement = 0
			elif(self.felement < 0):
				self.felement = len(self.element)-1
	class button:
		def __init__(self,text,fid,spec="",spect = 0):
			self.text = text
			self.fid = fid
			self.spec = spec
			self.spect = spect
	class runner:

		def __init__(self):
			self.error = ""
			pass
		def run(self,scene):

			screen = curses.initscr()
			curses.start_color()
			curses.noecho()
			curses.cbreak()
			screen.keypad(True)
			x = True
			while x:
				self.draw(screen,scene)
				y = screen.getch()
				if(y == curses.KEY_DOWN):
					scene.inc(1)
				elif (y == curses.KEY_UP):
					scene.inc(-1)
				elif((y == curses.KEY_ENTER)or(y == 10)):
					x = False
				elif((y == 410)):

					self.error="Resizing Window can cause errors"
				else:
					scene.title += (str(y))
			screen.clear()
			curses.nocbreak(); screen.keypad(0); curses.echo()
			curses.endwin()
			return scene.element[scene.felement].fid
		def draw(self,screen,scene):
			screen.clear()
			screen.addstr(scene.title)

			curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)
			curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
			curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_WHITE)
			curses.init_pair(4, curses.COLOR_RED, curses.COLOR_WHITE)
			screen.addstr(self.error+"\n", curses.color_pair(4))
			for i in range(len(scene.element)):

				l = i + len(scene.title.split("\n"))
				if(scene.element[i].spec == ""):
					t = "["+scene.element[i].text+"]"
					if (scene.felement == i):
						screen.addstr(l,0,">"+t, curses.color_pair(1))
					else:
						screen.addstr(l,0,t)
				if(scene.element[i].spec != ""):
					t = "["+scene.element[i].text
					t2 = scene.element[i].spec
					t3 = "]"
					if (scene.felement == i):
						screen.addstr(l,0,">"+t, curses.color_pair(1))
						screen.addstr(l,len(t),t2,curses.color_pair(int(scene.element[i].spect)))
						screen.addstr(l,(len(t)+len(t2)),t3,curses.color_pair(1))
					else:
						screen.addstr(l,0,t)
						screen.addstr(l,len(t),t2,curses.color_pair(int(scene.element[i].spect)))
						screen.addstr(l,len(t)+len(t2),t3,curses.color_pair(0))
			screen.addstr("\n")








# buttonn = 0

#     def keys(msg):
#         screen = curses.initscr()
#         curses.start_color()

#         curses.noecho()
#         curses.cbreak()
#         screen.keypad(True)
#         screen.clear()
#         x = msg.split("|;")
#         curses.init_pair(1, curses.COLOR_RED, curses.COLOR_WHITE)
#         for i in range(len(x)):
#             if(i%2 == 0):
#                 screen.addstr(x[i])
#             else:
#                 screen.addstr(x[i], curses.color_pair(1))
#         try:
#             while True:

#                 char = screen.getch()
#                 if char == curses.KEY_RIGHT:
#                     return 'right'
#                 elif char == curses.KEY_LEFT:
#                     return 'left'
#                 elif char == curses.KEY_UP:
#                     return 'up'
#                 elif char == curses.KEY_DOWN:
#                     return 'down'
#                 elif (char == curses.KEY_ENTER) or (char == 10):
#                     return 'return'
#                 else:
#                     print(char)
#                     return char
#         except:
#             curses.nocbreak(); screen.keypad(0); curses.echo()

#             curses.endwin()


#         finally:
#             curses.nocbreak(); screen.keypad(0); curses.echo()
#             curses.endwin()
 #   ____ ___  _   _ _____ ___ ____
 #  / ___/ _ \| \ | |  ___|_ _/ ___|  _ __  _   _
 # | |  | | | |  \| | |_   | | |  _  | '_ \| | | |
 # | |__| |_| | |\  |  _|  | | |_| |_| |_) | |_| |
 #  \____\___/|_| \_|_|   |___\____(_) .__/ \__, |
 #                                   |_|    |___/
 # Config Class
 # MIT License
 # Copyright (c) 2020 Haz001

from pathlib import Path
import os
class config:

	home = str(Path.home())
	def __init__(self, cf, reado=True):
		#Path.home()+"/.config/tclock/"+cf+".conf"
		self.path = self.home+"/.config/tclock/"+cf+".conf"
		self.reado = reado
		self.sr = self.stream(self.path,reado)

	def get(self, name):
		f = self.sr.read(name)

		if(f == None):
			return None
		else:
			return f
	def type(self, name):
		f = self.sr.read(name)
		if (len(f)<2):
			return None
		else:
			return f[1]
	def set(self, name, value):
		if(self.reado):
			pass
		else:
			self.sr.write(str(name),str(value))
	def praw(self):
		f = open(self.path,'r')
		txt = f.read()
		return str(self.sr.uncomp())
	class stream:
		def __init__(self,cpath,readonly):
			y = Path(cpath)

			if(y.exists()):
				self.cpath = cpath
				self.reado = readonly
				x = open(cpath,'r')
				self.data = x.read()
				x.close()
			else:
				z = cpath.split("/")

				for i in range(len(z)+1):

					a = "/".join(z[:(i)])
					if(i == len(z)):
						if(not Path(a).is_file()):

							open(a,'w').close()
					else:
						if( not Path(a).is_dir()):

							os.mkdir(a)


				self.cpath = cpath
				self.reado = readonly
				x = open(cpath)
				self.data = x.read()
				x.close()


		def uncomp(self):
			data = self.data
			result = {}
			vnb = False ## False - name, True - Value
			##count = 0
			qb = False ## Flips when in quote braket
			name = []
			value = []
			tname = ""
			tvalue = ""
			lstchr = ""
			for i in range(len(data)):# starts loop through all characters
				cchar = data[i]# sets cchar as character
				if(qb):# checks if quotes flag is on
					if((cchar=="\"")and not(lstchr == "\\")):# if genuin quotes appears again
						qb = False # set quote flag as off
					else:# anything other than genuin quotes
						if (vnb):# check if value name bool is true
							tvalue += data[i] # write to the value
						else:# else
							tname += data[i]# write to the name
				else: # when no quotes flag
					if((cchar=="\n")or(cchar=="\r")): # check if new line to start new value
						vnb = False # set to read name first again
						name.append(tname)
						tname = ""
						value.append(tvalue)
						tvalue = ""
						##count += 1
					elif((cchar==":")):
						vnb = True # now read value
					elif(cchar=="\""):
						qb = True
				lstchr = cchar



			dic = {}
			if(len(name)>= len(value)):
				count = len(value)
			else:
				count = len(name)
				#print("small error")
			for i in range(count):
				if((name[i] != None)and(value[i] != None)):

					dic[name[i]] = value[i]
					##print("-1-: "+str(i)+" "+str(name[i])+" " +str(value[i]))
			result = dic

			return result
		def read(self,name):
			x = self.uncomp()
			#print("-1-"+str(x))
			result = ""
			if(name in x):
				result = x[name]
			else:
				result = None
			#print("-2-"+str(result))
			return result
		def write(self,name,value):
			dic = self.uncomp()
			##print("-3-: "+str(dic))
			result = ""
			if(name in dic): ## This might be used later, don't optimize yet.
				dic[name] = value
			else:
				dic[name] = value
			x = [ [k,v] for k, v in dic.items() ] ## convets dictionary to list
			##print("-2-: "+str(x))



			lname = []
			lvalue = []
			for i in range(len(x)):
				lname.append(x[i][0])
				lvalue.append(x[i][1])

			txt = ""
			for i in range(len(lname)):
				if not((lname[i] == "")and(lvalue[i] == "")):
					txt+=str("\""+str(lname[i])+"\":\""+str(lvalue[i])+"\"\n")
			self.data = txt
			f = open(self.cpath,'w')
			f.write(txt)
			f.close()
# NOTE: Simple test to see if it can write and read values
def test(self = None):
	file = open(str(Path.home())+"/.config/tclock/"+"testfile"+".conf",'w').close()
	del file
	x = config("testfile",False)
	import random
	try:
		y1 = str(random.randint(0,65535))
		y2 = str(random.randint(0,65535))
		x.set("testvalue1",str(y1))
		x.set("testvalue2",str(y2))
		z1 = x.get("testvalue1")
		z2 = x.get("testvalue2")
		if((z1 == y1) and (z2 == y2)):
			print("success")
		elif(z2 == y2):
			print("Error with Config.py\nInit Test Failed on read 1.\nExpecting '"+y1+"' as '"+str(type(y1))+"' but got '"+z1+"' as '"+str(type(z1))+"'")
			exit()
		elif(z1 == y1):
			print("Error with Config.py\nInit Test Failed on read 2.\nExpecting '"+y2+"' as '"+str(type(y2))+"' but got '"+z2+"' as '"+str(type(z2))+"'")
			exit()
		else:
			print("Error with Config.py\nInit Test Failed on both reads.\nExpecting '"+y1+"' and '"+y2+"' as '"+str(type(y1))+"' and '"+str(type(y1))+"'  but got '"+z1+"' and '"+z2+"' as '"+str(type(z1))+"' and '"+str(type(z1))+"'")
			exit()
	except OSError as e:
		print("OS error: "+str(e))
		exit()
	finally:
		del x
		del y1
		del z1
		del y2
		del z2

test()
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
from pathlib import Path
import time
import os
debug = os.path.isfile("debug")
if(debug):
	from menu import menu
	from config import config
	print("DEBUG version")
else:
	pass
import sys
from time import sleep
from datetime import datetime as dt
from sys import platform
class mode:
	def __init__(self,name,blockstart,blockcent,blockend):
		self.name = name
		self.blockstart = blockstart
		self.blockcent = blockcent
		self.blockend = blockend
class ui:
	invert = False
	title = ""
	block = '\x1b[7;30;39m'+" "+'\x1b[0m'
	modex = None
	modes = {
	"mode0": ## Blocks using spaces and colours
		mode("Space",'\x1b[7;30;39m'," ",'\x1b[0m'),
	"mode1": ## Blocks using unicode
		mode("Block",'',"\u2588",''),
	"mode2": ## Hash (#) and colours
		mode("Hash",'\x1b[7;30;39m',"#",'\x1b[0m'),
	"mode3": ## Dash (-) and colours
		mode("Dash",'\x1b[7;30;39m',"-",'\x1b[0m'),
	"mode4": ## Slash (/) and colours
		mode("Slash",'\x1b[7;30;39m',"/",'\x1b[0m'),
	"mode5": ## Backslash (\) and colours
		mode("BackSlash",'\x1b[7;30;39m',"\\",'\x1b[0m'),
	"mode6": ## Underscore (_) and colours
		mode("UnderScore",'\x1b[7;30;39m',"_",'\x1b[0m'),
	"mode7": ## Tilde (~) and colours
		mode("Tilde",'\x1b[7;30;39m',"~",'\x1b[0m')}
	def chngClass(n):
		if(str(type(n)) == "<class 'int'>"):
			ui.modex = ui.modes["mode0"]
			ui.modex = ui.modes["mode"+str(n)]
			ui.block = ui.modex.blockstart + ui.modex.blockcent + ui.modex.blockend
		else:
			prin("error")



ui.chngClass(0)
class paths:
	script = None
	name = None
	def type(filename):
		x = Path(filename)
		if(x.is_fidrawsle()):
			return "file" # it is a file
		elif(x.is_dir()):
			return "dir"  # it is a directory
		elif(x.exists()):
			return "unkn" # something is there, unknown (may be unaccessible)
		else:
			return "none" # nothing is there
	def getScript():
		y  = ""
		if(paths.script == None):
			x = str(os.path.realpath(__file__)).split("/")
			x.pop(len(x)-1)
			y = '/'.join(x)+'/'
			paths.scripts = y
		else:
			y = paths.script
		return y
x = open(paths.getScript()+"data/title",'r')
ui.title = x.read()
x.close()
class grid:
	def __init__(self,w,h):
		self.grid = {}
		self.width = w
		self.height = h
		for x in range(self.width):
			for y in range(self.height):
				self.grid[str(x)+" - "+str(y)] = 0
	def getPix(self, x,y):
		if(str(x)+" - "+str(y)) in self.grid:
			return self.grid[str(x)+" - "+str(y)]
	def setPix(self,x,y,v):
		self.grid[str(x)+" - "+str(y)] = str(v)
	def printGrid(self,t=0):
		for i in range((self.height)):
			s = ""
			for j in range((self.width)):
				if(ui.invert):
					if(self.getPix(j,i) == "1"):
						s+=ui.block
					else:
						s+=" "
				else:
					if(self.getPix(j,i) == "1"):
						s+=" "
					else:
						s+=ui.block
			print(s)
			sleep(t)
class number:
	def __init__(self,name,filep=None):
		self.grid = {}
		self.Name = None
		self.height = 7
		self.width = 7
		self.name = name
		if (filep!= None):
			self.openPix(filep)
	def getPix(self,x,y):
		rt = 0
		ind = str(x)+" - "+str(y)
		if(x <= self.width):
			if(y <= self.height):
				if(ind in self.grid):
					rt =  self.grid[ind]
		return rt
	def setPix(self,x,y,pix,m=""):
		if(x <= self.width):
			if(y <= self.height):
				self.grid[str(x)+" - "+str(y)] = pix
				if(m != ""):
					print(m)
	def openPix(self,filepath):
		file = open(filepath)
		lines = file.read().split()
		file.close()
		for i in range(len(lines)):
			for j in range(len(lines[i])):
				self.setPix(j,i,lines[i][j])
	def printNum(self):
		for i in range((self.height)):
			s = ""
			for j in range((self.width)):
				if(self.getPix(i,j) == "1"):
					s+="#"
				else:
					s+=" "
			print(s)
		print("\n")
class nums:
	height = 7
	width = 7
	zero  = number("zero",paths.getScript()+"data/num/0")
	#zero.printNum()
	one   = number("one",paths.getScript()+"data/num/1")
	#one.printNum()
	two   = number("two",paths.getScript()+"data/num/2")
	#two.printNum()
	three = number("three",paths.getScript()+"data/num/3")
	#three.printNum()
	four  = number("four",paths.getScript()+"data/num/4")
	#four.printNum()
	five  = number("five",paths.getScript()+"data/num/5")
	#five.printNum()
	six   = number("six",paths.getScript()+"data/num/6")
	#six.printNum()
	seven = number("seven",paths.getScript()+"data/num/7")
	#seven.printNum()
	eight = number("eight",paths.getScript()+"data/num/8")
	#eight.printNum()
	nine  = number("Nine",paths.getScript()+"data/num/9")
	#nine.printNum()
	def getNum(s):
		sx = str(s)
		if(sx == "0"):
			return nums.zero
		elif(sx == "1"):
			return nums.one
		elif(sx == "2"):
			return nums.two
		elif(sx == "3"):
			return nums.three
		elif(sx == "4"):
			return nums.four
		elif(sx == "5"):
			return nums.five
		elif(sx == "6"):
			return nums.six
		elif(sx == "7"):
			return nums.seven
		elif(sx == "8"):
			return nums.eight
		elif(sx == "9"):
			return nums.nine
		else:
			return nums.zero
class fun:
	def clear():
		c = ""
		if platform == "linux" or platform == "linux2":
			c ="clear"
		elif platform == "darwin":
			c ="clear"
		elif platform == "win32":
			c ="cls"
		os.system(c)
	def loop(t = None):
		g = grid(nums.width*6+14,nums.height+2)
		sec = ""
		while True:
			if (str(dt.now().second) != sec):
				sec = str(dt.now().second)
				if(t == None):
					fun.clear()
					fun.draw(g)
				else:
					fun.draw(g,t)
			time.sleep(0.01)
	def draw(g,t = 0):
		h = str(dt.now().hour)
		while(len(h)<2):
			h = "0"+h
		m = str(dt.now().minute)
		while(len(m)<2):
			m = "0"+m
		for i in range(len(h)):
			n = nums.getNum(h[i])
			for x in range(nums.width):
				for y in range(nums.height):
					g.setPix((x+1)+(i*(nums.width+2)),y+1,n.getPix(x,y))
		for i in range(len(m)):
			n = nums.getNum(m[i])
			for x in range(nums.width):
				for y in range(nums.height):
					g.setPix((x+2)+((i+2)*(nums.width+2)),y+1,n.getPix(x,y))
		s = str(dt.now().second)
		while(len(s)<2):
			s = "0"+s
		for i in range(len(s)):
			n = nums.getNum(s[i])
			for x in range(nums.width):
				for y in range(nums.height):
					g.setPix((x+3)+((i+4)*(nums.width+2)),y+1,n.getPix(x,y))
		g.setPix(nums.width*2+4,2,1)
		g.setPix(nums.width*2+4,6,1)
		g.setPix(nums.width*4+9,2,1)
		g.setPix(nums.width*4+9,6,1)
		g.printGrid(t)
	class mnu:
		scene = 0
	def menu():
		block = ui.modex.blockcent

		mainm = menu.scene("main","Main Menu",[menu.button("Flash (Default)",0),menu.button("Scroll",1),menu.button("Settings",2),menu.button("Quit",3)])
		scrollm = menu.scene("scroll","Scroll Menu",[menu.button("Slow",4),menu.button("Medium",5),menu.button("Fast",6),menu.button("Custom",7),menu.button("Back",8)])
		settingm = menu.scene("settings","Settings Menu", [ menu.button("Invert - " + str(ui.invert),9), menu.button("Block type - ",12,block,2) , menu.button("Back",10) ] )
		modem = menu.scene("mode","Mode Menu",[menu.button("Space - ",11," ",2),menu.button("Hash - ",11,"#",2),menu.button("Dash - ",11,"-",2),menu.button("Slash - ",11,"/",2),menu.button("Backslash - ",11,"\\",2),menu.button("underscore - ",11,"_",2),menu.button("Tilde - ",11,"~",2),menu.button("Back",8)])
		cmenu = menu.runner()
		mainm.title = ui.title
		while True:
			x = mainm
			if(fun.mnu.scene == 0):
				x = mainm
			elif(fun.mnu.scene == 1):
				x = scrollm
			elif(fun.mnu.scene == 2):
				x = settingm
			elif(fun.mnu.scene == 3):
				x = modem

			y = cmenu.run(x)
			if(y == 0):
				fun.loop()
			elif(y == 1):
				 fun.mnu.scene = 1
			elif(y == 2):
				fun.mnu.scene = 2
			elif(y == 3):
				exit(0)
			elif(y == 4):
				fun.loop(0.2)
			elif(y == 5):
				fun.loop(0.1)
			elif(y == 6):
				fun.loop(0.05)
			elif(y == 7):
				z = True
				while z:
					try:
						a = float(input("Custom Number"))
						z = False
					except:
						z = True
				fun.loop(a)
			elif(y == 8):
				fun.mnu.scene = 0
			elif(y == 9):
				ui.invert = not (ui.invert)
				x = config("settings",False)
				x.set("invert",str(ui.invert).lower())
				del x
				settingm = menu.scene("settings","Settings Menu", [ menu.button("Invert - " + str(ui.invert),9), menu.button("Block type - ",12,block,2) , menu.button("Back",10) ] )
			elif(y == 10):
				fun.mnu.scene = 0
			elif(y == 12):
				fun.mnu.scene = 3
	def default():
		fun.loop()
		input()
	def github():
		file = open(paths.getScript()+"data/github")
		t = file.read()
		file.close()
		print(t)
	def help():
		file = open(paths.getScript()+"data/help")

		t = file.read().replace("\\t","\t")
		file.close()
		print(t)
	def inst():
		g = grid(nums.width*6+14,nums.height+2)
		fun.draw(g)
	def fixConfig(name,value):
		x = config("settings",False)
		x.set(name, value)
		del x
	def getFileOptions():
		x = config("settings",True)
		y = x.get("display")

		if(y in ["0","1","2","3","4","5","6","7","8","9"]):
			pass
		else:
			fun.fixConfig("display","0")
			print("display value error, value reset")
		del y
		y = x.get("invert")
		if(y == 'true'):
			ui.invert = True
		elif(y == 'false'):
			ui.invert = False
		else:
			fun.fixConfig("invert","true")
			print("invert value error, value reset")



fun.getFileOptions()

args = (sys.argv)

class flags:
	help = False
	github = False
	menu = False
	instant = False
if (len(args) == 1):
	fun.default()
for i in range(len(args)):
	arg = args[i]
	if((arg != None) and (len(arg)>1)):
		if(arg[0] == '-'):
			if(arg[1] == '-'):
				if(arg == "--help"):
					flags.help = True
				elif(arg == "--git"):
					flags.github = True
				elif(arg == "--menu"):
					flags.menu = True
				elif(arg == "--inst"):
					flags.instant = True
			else:
				if('h' in arg):
					flags.help = True
				elif('G' in arg):
					flags.github = True
				elif('m' in arg):
					flags.menu = True
				if('i' in arg):
					flags.instant = True




if(flags.help):
	fun.help()
if(flags.github):
	fun.github()
if(flags.instant):
	fun.inst()
if(flags.menu):
	fun.menu()
