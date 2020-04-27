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
        def __init__(self,text,fid):
            
            self.text = text
            self.fid = fid
    class runner:
        def __init__(self):
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
                    
                else:
                    scene.title += (str(y))
                # if char == curses.KEY_RIGHT:
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
                
            screen.clear()
            curses.nocbreak(); screen.keypad(0); curses.echo()
            curses.endwin()
            return scene.element[scene.felement].fid
            


        def draw(self,screen,scene):
           
            
            screen.clear()
            screen.addstr(scene.title)
            screen.addstr("\n")
            curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)

            for i in range(len(scene.element)):
                screen.addstr("\n")
                t = "["+scene.element[i].text+"]"
                if (scene.felement == i):
                    screen.addstr(">"+t, curses.color_pair(1))
                else:
                    screen.addstr(t)
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

        self.path = self.home+"/.config/tclock/"+cf+".conf"
        self.reado = reado
        self.sr = self.stream(self.path,reado)

    def get(self, name):
        f = self.sr.read(name)

        if(f == None):
            return None
        else:
            if (f[0] == None):
                return None
            else:
                return f[0]
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
            result = ""
            part = 0 ## 0 - name,1 - value,  2 - type
            count = 0
            quotes = False
            name = {}
            tname = ""
            value = {}
            tvalue = ""
            t = {}
            tt = ""
            lstchr = ""
            for i in range(len(data)):
                cchar = data[i]
                if(quotes):
                    if((cchar=="\"")and not(lstchr == "\\")):
                        quotes = False
                    else:
                        if (part == 0):
                            tname += data[i]
                        elif (part == 1):
                            tvalue += data[i]
                        elif (part == 2):
                            tt += data[i]
                else:
                    if((cchar=="\n")or(cchar=="\r")):
                        part = 0
                        name[count] = tname
                        tname = ""
                        value[count] = tvalue
                        tvalue = ""
                        if((tt == "")or(tt == None)):
                            tt = "s"
                        t[count] = tt
                        tt = ""
                        count += 1
                    elif((cchar==":")):
                        part += 1
                    elif(cchar=="\""):
                        quotes = True
                lstchr = cchar
            result = (name, value, t)
            dic = {}
            for i in range(count):
                if((name[i] != None)and(name[i] != None)):
                    if(t[i] == ""):
                        dic[name[i]] = [value[i]]
                    else:
                        dic[name[i]] = [value[i],t[i]]
            result = dic
            return result
        def read(self,name):
            x = self.uncomp()
            result = ""
            if(name in x):
                result = x[name]
            else:
                result = None
            return result
        def write(self,name,value):
            dic = self.uncomp()
            result = ""
            if(name in dic): ## This might be used later, don't optimize yet.
                dic[name] = value
            else:
                dic[name] = value
            x = [ [k,v] for k, v in dic.items() ] ## convets dictionary to list
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
    x = config("testfile",False)
    import random
    try:
        y = str(random.randint(0,65535))
        x.set("testvalue",str(y))
        z = x.get("testvalue")
        if(z == y):
            pass
        else:
            print("Error with Config.py\nInit Test Failed.\nExpecting '"+y+"' as '"+str(type(y))+"' but got '"+z+"' as '"+str(type(z))+"'")
            exit()
    except OSError as e:
        print("OS error: "+str(e))
        exit()
    finally:
        del x
        del y
        del z
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
debug = True
if(debug):
    from menu import menu
    from config import config
    print("debug")
else:
    pass

import sys
from time import sleep
from datetime import datetime as dt
from sys import platform
import curses
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
    modes = { "mode0":
        mode("Space",'\x1b[7;30;39m'," ",'\x1b[0m'),
    "mode1":
        mode("Block",'',"\u2588",''),
    "mode2":
        mode("Hash",'\x1b[7;30;39m',"#",'\x1b[0m'),
    "mode3":
        mode("Dash",'\x1b[7;30;39m',"-",'\x1b[0m'),
    "mode4":
        mode("Slash",'\x1b[7;30;39m',"/",'\x1b[0m'),
    "mode5":
        mode("BackSlash",'\x1b[7;30;39m',"\\",'\x1b[0m'),
    "mode6":
        mode("UnderScore",'\x1b[7;30;39m',"_",'\x1b[0m'),
    "mode7":
        mode("Tilde",'\x1b[7;30;39m',"~",'\x1b[0m')}
    def chngClass(n):

        modex = ui.modes["mode0"]
        modex = ui.modes["mode"+str(n)]
        # if(n == 1):
        #     modex = mode1
        # elif(n == 2):
        #     modex = mode2
        # elif(n == 3):
        #     modex = mode3
        # elif(n == 4):
        #     modex = mode4
        # elif(n == 5):
        #     modex = mode5
        # elif(n == 6):
        #     modex = mode6
        # elif(n == 7):
        #     modex = mode7

        ui.block = modex.blockstart + modex.blockcent + modex.blockend

ui.chngClass(0)
class paths:
    script = None
    name = None
    def type(filename):
        x = Path(filename)
        if(x.is_file()):
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
        mainm = menu.scene("main","Main Menu",[menu.button("Flash (Default)",0),menu.button("Scroll",1),menu.button("Settings",2),menu.button("Quit",3)])
        scrollm = menu.scene("scroll","Scroll Menu",[menu.button("Slow",4),menu.button("Medium",5),menu.button("Fast",6),menu.button("Custom",7),menu.button("Back",8)])
        settingm = menu.scene("settings","Settings Menu", [ menu.button("Invert - " + str(ui.invert),9) , menu.button("Back",10) ] )
        modem = menu.scene("mode","Mode Menu",[menu.button("Spcae",11),menu.button("Back",8)])
        cmenu = menu.runner()
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
                settingm = menu.scene("settings","Settings Menu", [ menu.button("Invert - " + str(ui.invert),9) , menu.button("Back",10) ] )
            elif(y == 10):
                fun.mnu.scene = 0
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
        t = file.read()
        file.close()
        print(t)
    def inst():
        fun.draw()
    def getFileOptions():
        x = config("settings",False)
        y = x.get("display")
        if (y == None):
            x.set("display","default")
        else:
            pass
        del y
        y = x.get("invert")
        if (y == None):
            x.set("invert","false")
            ui.invert = False
        else:
            
            if(y == "true"):
                ui.invert = True
            else:
                ui.invert = False



fun.getFileOptions()
args = (sys.argv)


if (len(args) == 1):
    fun.default()
elif(len(args) == 2):
    if(args[1] == "-h"):
        fun.help()
    elif(args[1] == "-git"):
        fun.github()
    elif(args[1] == "-m"):
        fun.menu()
    elif(args[1] == "-i"):
        fun.inst()
