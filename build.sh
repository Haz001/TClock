#!/bin/sh
{
	echo "[0/7] - Concatinating python source"
	cat src/menu.py src/config.py src/tclock.py > src/main.py
	echo "[1/7] - Copying python source"
	cp -r src/* tclock-deb/usr/share/tclock/
	cp -r src/* install/usr/share/tclock/
	echo "[2/7] - deleting debug flag"
	rm tclock-deb/usr/share/tclock/debug
	rm install/usr/share/tclock/debug
	echo "[3/7] - making python executable"
	chmod 777 tclock-deb/usr/share/tclock/main.py
	chmod 777 install/usr/share/tclock/main.py
	echo "[4/7] - Packaging For Debian"
	dpkg --build tclock-deb
	
	echo "[5/7] - Compressing Install.zip"
    zip -r9 build/tclock.zip ./install/
    echo "[6/7] - Copying last files to build folder"
	mv tclock-deb.deb build/tclock.deb
	cp LICENSE build/LICENSE
	echo "[7/7] - Done"
}||{
	echo "Failed"
}
