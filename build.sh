#!/bin/sh
{
	echo "[ 0/11] - Concatinating python source"
	cat src/menu.py src/config.py src/tclock.py > src/main.py
	echo "[ 1/11] - Copying python source"
	cp -r src/* tclock-deb/usr/share/tclock/
	cp -r src/* install/usr/share/tclock/
	echo "[ 2/11] - deleting debug flag"
	rm tclock-deb/usr/share/tclock/debug
	rm install/usr/share/tclock/debug
	echo "[ 3/11] - making python executable"
	chmod 777 tclock-deb/usr/share/tclock/main.py
	chmod 777 install/usr/share/tclock/main.py
	echo "[ 4/11] - making symbolic link"
	ln -s /usr/share/tclock/main.py ./tclock-deb/usr/bin/tclock
	ln -s /usr/share/tclock/main.py ./install/usr/bin/tclock
	echo "[ 5/11] - Packaging For Debian"
	dpkg --build tclock-deb
	echo "[ 6/11] - Compressing Install.zip"
    zip -r9 build/tclock.zip ./install/
    echo "[ 7/11] - removing symbolic link"
	rm ./tclock-deb/usr/bin/tclock
    rm ./install/usr/bin/tclock
    echo "[ 8/11] - Copying last files to build folder"
	mv tclock-deb.deb build/tclock.deb
	cp LICENSE build/LICENSE
	cd build/
	echo "[ 9/11] - Conveting deb to rpm, slp, lsb and tgz"
	sudo alien --to-rpm tclock.deb;sudo alien --to-slp tclock.deb;sudo alien --to-lsb tclock.deb; sudo alien --to-tgz tclock.deb
	echo "[10/11] - Taking ownership over all files"
	sudo chown haz:haz ./*
	echo "[11/11] - Done"
}||{
	echo "Failed"
}
