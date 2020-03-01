{
	echo "[0/5] - Concatinating python source"
	cat src/menu.py src/tclock.py > src/main.py
	echo "[1/5] - Copying python source"
	cp -r src/* tclock-deb/usr/share/tclock/
	echo "[2/5] - deleting debug flag"
	rm tclock-deb/usr/share/tclock/debug
	echo "[3/5] - making python executable"
	chmod 771 tclock-deb/usr/share/tclock/tclock.py
	echo "[4/5] - Packaging For Debian"
	dpkg --build tclock-deb
	echo "[5/5] - Done"
}||{
	echo "Failed"
}
