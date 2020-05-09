#!/usr/bin/env python3
#   ____ _     ___
#  / ___| |   |_ _|  _ __  _   _
# | |   | |    | |  | '_ \| | | |
# | |___| |___ | | _| |_) | |_| |
#  \____|_____|___(_) .__/ \__, |
#                   |_|    |___/
# Menu
# MIT License
# Copyright (c) 2019 Haz001
import platform
import os
if(os.name()=="poxi"):
    import curses
else:
    print("Basic Menu")
class drawer:
    def __init__():
        if(os.name()=="poxi"):



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
