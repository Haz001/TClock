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
