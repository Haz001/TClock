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
import sys
from time import sleep
class ui:
    blockstart = '\x1b[7;30;39m'
    block = " "
    blockend = '\x1b[0m'

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
    def setPix(self,x,y,pix):
        if(x <= self.width):
            if(y <= self.height):
                self.grid[str(x)+" - "+str(y)] = pix
    def openPix(self,filepath):
        file = open(filepath)
        lines = file.read().split()
        file.close()
        for i in range(len(lines)):
            for j in range(len(lines[i])):
                self.setPix(i,j,lines[i][j])
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
zero  = number("zero","num/0")
zero.printNum()
one   = number("one","num/1")
one.printNum()
two   = number("two","num/2")
two.printNum()
three = number("three","num/3")
three.printNum()
four  = number("four","num/4")
four.printNum()
five  = number("five","num/5")
five.printNum()
six   = number("six","num/6")
six.printNum()
seven = number("seven","num/7")
seven.printNum()
eight = number("eight","num/8")
eight.printNum()
nine  = number("Nine","num/9")
nine.printNum()
