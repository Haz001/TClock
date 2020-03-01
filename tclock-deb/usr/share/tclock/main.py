import curses
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

import time
import os
if(os.path.exists("debug")):
    import menu
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

ui.chngClass(input())
class paths:
    script = None
    name = None
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
                    if(self.getPix(j,i) == "0"):
                        s+=" "
                    else:
                        s+=ui.block
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
        sec = ""
        while True:
            if (str(dt.now().second) != sec):
                sec = str(dt.now().second)
                if(t == None):
                    fun.clear()
                    fun.draw()
                else:
                    fun.draw(t)
            time.sleep(0.01)

    def draw(t = 0):
        g = grid(nums.width*6+14,nums.height+2)
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
        if(ui.invert):
            # g.setPix(nums.width*2+4,2,0)
            # g.setPix(nums.width*2+4,6,0)
            # g.setPix(nums.width*4+9,2,0)
            # g.setPix(nums.width*4+9,6,0)
            print(">:-@")
        else:
            g.setPix(nums.width*2+4,2,1)
            g.setPix(nums.width*2+4,6,1)
            g.setPix(nums.width*4+9,2,1)
            g.setPix(nums.width*4+9,6,1)



        g.printGrid(t)
    class mnu:
        scene = 0
        btnn = 0
        msg = 0
    def menu():
        while True:
            msg = ""
            if(fun.mnu.scene == 0):
                msg = "Main Menu\n"+cmenu.button(["Flash (Default)","Scroll","Settings","Quit"],fun.mnu.btnn,True)
            elif(fun.mnu.scene == 1):
                msg = "Scroll Menu\n"+cmenu.button(["Slow","Medium","Fast","Custom","Back"],fun.mnu.btnn,True)
            elif(fun.mnu.scene == 2):
                msg = "Settings Menu\n(Under Development)\n"+cmenu.button(["(Buggy) Invert - "+str(ui.invert),"Back"],fun.mnu.btnn,True)
            elif(fun.mnu.scene == 3):
                msg = cmenu.button(["Back","exit"],fun.mnu.btnn,True)
            elif(fun.mnu.scene == 4):
                msg = "Mode Menu\n(Under Development)\n"+cmenu.button(["Space","","Back"],fun.mnu.btnn,True)
            x = (cmenu.keys(ui.title+msg))

            if(fun.mnu.scene == 0):
                if(x == 'down'):
                    fun.mnu.btnn += 1
                elif(x == 'up'):
                    fun.mnu.btnn -= 1
                elif(x== 'return'):
                    if(fun.mnu.btnn == 0):


                        fun.loop()
                    elif(fun.mnu.btnn == 1):
                        fun.mnu.scene = 1
                        fun.mnu.btnn = 0
                    elif(fun.mnu.btnn == 2):
                        fun.mnu.scene = 2
                        fun.mnu.btnn = 0
                    elif(fun.mnu.btnn == 3):


                        exit()
                if(fun.mnu.btnn < 0):
                    fun.mnu.btnn = 3
                elif(fun.mnu.btnn >3):
                    fun.mnu.btnn = 0
            elif(fun.mnu.scene == 1):
                if(x == 'down'):
                    fun.mnu.btnn += 1
                elif(x == 'up'):
                    fun.mnu.btnn -= 1
                elif(x== 'return'):
                    if(fun.mnu.btnn == 0):
                        fun.loop(0.2)
                    elif(fun.mnu.btnn == 1):
                        fun.loop(0.1)
                    elif(fun.mnu.btnn == 2):
                        fun.loop(0.05)
                    elif(fun.mnu.btnn == 3):
                        z = True
                        while z:
                            try:
                                a = float(input("Custom Number"))
                                z = False
                            except:
                                z = True
                        fun.loop(a)
                    elif(fun.mnu.btnn == 4):
                        fun.mnu.scene = 0
                        fun.mnu.btnn = 0
                    elif(fun.mnu.btnn == 5):
                        exit()

                if(fun.mnu.btnn < 0):
                    fun.mnu.btnn = 4
                elif(fun.mnu.btnn >4):
                    fun.mnu.btnn = 0
            elif(fun.mnu.scene == 2):
                if(x == 'down'):
                    fun.mnu.btnn += 1
                elif(x == 'up'):
                    fun.mnu.btnn -= 1
                elif(x== 'return'):
                    if(fun.mnu.btnn == 0):
                        (ui.invert) = not (ui.invert)
                    elif(fun.mnu.btnn == 1):
                        fun.mnu.scene = 0
                    elif(fun.mnu.btnn == 2):
                        exit()

                if(fun.mnu.btnn < 0):
                    fun.mnu.btnn = 1
                elif(fun.mnu.btnn >1):
                    fun.mnu.btnn = 0
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
class cmenu():
    buttonn = 0

    def keys(msg):
        screen = curses.initscr()
        curses.start_color()

        curses.noecho()
        curses.cbreak()
        screen.keypad(True)
        screen.clear()
        x = msg.split("|;")
        curses.init_pair(1, curses.COLOR_RED, curses.COLOR_WHITE)
        for i in range(len(x)):
            if(i%2 == 0):
                screen.addstr(x[i])
            else:
                screen.addstr(x[i], curses.color_pair(1))
        try:
            while True:

                char = screen.getch()
                if char == curses.KEY_RIGHT:
                    return 'right'
                elif char == curses.KEY_LEFT:
                    return 'left'
                elif char == curses.KEY_UP:
                    return 'up'
                elif char == curses.KEY_DOWN:
                    return 'down'
                elif (char == curses.KEY_ENTER) or (char == 10):
                    return 'return'
                else:
                    print(char)
                    return char
        except:
            curses.nocbreak(); screen.keypad(0); curses.echo()

            curses.endwin()


        finally:
            curses.nocbreak(); screen.keypad(0); curses.echo()
            curses.endwin()


    def button(blst, actn,btbl = False):
        rets = ""
        for i in range(len(blst)):
            if i != actn:
                rets += "[ "+blst[i]+" ]"
            elif i == actn:
                rets += "|;[ "+blst[i]+" ]|;"
            if btbl:
                rets+="\n"
        return rets
# home = str(Path.home())
# conf = home+"/.config/tClock/"

args = (sys.argv)
print(args)
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
