{
	echo "[0/3] - Copying python source"
	cp -r src/* tclock-deb/usr/share/tclock/
	echo "[1/3] - making python executable"
	chmod 771 tclock-deb/usr/share/tclock/tclock.py
	echo "[2/3] - Packaging For Debian"
	dpkg --build tclock-deb
	echo "[3/3] - Done"
}||{
	echo "Fail"
}
