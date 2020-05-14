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
