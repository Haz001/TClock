#!/usr/bin/env python3
#  _ __ ___   ___ _ __  _   _   _ __  _   _
# | '_ ` _ \ / _ \ '_ \| | | | | '_ \| | | |
# | | | | | |  __/ | | | |_| |_| |_) | |_| |
# |_| |_| |_|\___|_| |_|\__,_(_) .__/ \__, |
#                              |_|    |___/
# Menu
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
    def __init__(self):
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
    def drawloop(self):

        self.screen = curses.initscr()
        curses.start_color()
        curses.noecho()
        curses.cbreak()
        self.screen.keypad(True)

        curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)
        curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
        curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_WHITE)
        curses.init_pair(4, curses.COLOR_RED, curses.COLOR_WHITE)
    def draw(self,x,y,char,color):

        x = int(x)
        y = int(y)
        if((x in range(self.width+1)) and (y in range(self.height+1))):
            try:
                self.screen.addstr(y,x,char,curses.color_pair(color))
            except curses.error as e:
                pass
    def grep(self):
        self.screen.getch()
x = graphics()
x.drawloop()

#x.draw(1,x.height-1,'#',3)
#x.draw(x.width-2,x.height-1,'#',3)
s = str(x.width)+" \u2715 "+str(x.height)
for i in range(len(s)):
    w = int((x.width/2)+(i-len(s)/2))
    h = int((x.height/2)-1)
    x.draw(w,h,s[i],3)
x.draw(0,0,' ',2)
x.draw(1,0,' ',2)
x.draw(0,1,' ',2)
x.draw(x.width-1,x.height-1,' ',2)
x.draw(x.width-2,x.height-1,' ',2)
x.draw(x.width-1,x.height-2,' ',2)

x.draw(x.width-2,0,' ',2)
x.draw(x.width-1,1,' ',2)
x.draw(x.width-1,0,' ',2)
x.draw(0,x.height-1,' ',2)
x.draw(1,x.height-1,' ',2)
x.draw(0,x.height-2,' ',2)

#x.draw(x.width,x.height,'#',3)
x.draw(x.width-1,x.height-1,' ',2)
x.grep()
