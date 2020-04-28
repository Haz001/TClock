#!/bin/sh
{
	echo "[0/9] - Concatinating python source"
	cat src/menu.py src/config.py src/tclock.py > src/main.py
	echo "[1/9] - Copying python source"
	cp -r src/* tclock-deb/usr/share/tclock/
	cp -r src/* install/usr/share/tclock/
	echo "[2/9] - deleting debug flag"
	rm tclock-deb/usr/share/tclock/debug
	rm install/usr/share/tclock/debug
	echo "[3/9] - making python executable"
	chmod 777 tclock-deb/usr/share/tclock/main.py
	chmod 777 install/usr/share/tclock/main.py
	echo "[4/9] - Packaging For Debian"
	dpkg --build tclock-deb
	
	echo "[5/9] - Compressing Install.zip"
    zip -r9 build/tclock.zip ./install/
    echo "[6/9] - Copying last files to build folder"
	mv tclock-deb.deb build/tclock.deb
	cp LICENSE build/LICENSE
	echo "[7/9] Conveting deb to rpm, slp, lsb and tgz"
	sudo alien --to-rpm tclock.deb;sudo alien --to-slp tclock.deb;sudo alien --to-lsb tclock.deb; sudo alien --to-tgz tclock.deb
	echo "[8/9] Taking ownership over all files"
	sudo chown harry:harry ./*
	echo "[9/9] - Done"
}||{
	echo "Failed"
}
