pi = '3.1415926535...'


def main():
    print('Welcome to PiGame!')
    pigame()
    while True:
        yn = input('Play again? y/n ')
        if yn == 'y':
            pigame()
        else: return
def getch():
    import termios
    import sys, tty
    def _getch():
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch
    return _getch()

def pigame():

    n=0

    print('Go!')


    while n<=1000:
        x = getch()
        print(x)
        if x == pi[n]:
            n += 1
        else:
            print('I\'m sorry. The next digit was '+pi[n]+'.')
            print('You got to '+str(n)+' digits!')
            return
    print('You got to 1000! Hooray!')
main()
