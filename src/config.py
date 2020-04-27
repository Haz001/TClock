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
