{
	echo "[0/6] - Compiling python source"
	python -m nuitka --standalone tclock.py
	echo "[1/6] - Copying compiled python"
	cp tclock.dist/tclock tclock-deb/usr/share/tclock/tclock
	echo "[2/6] - Copying python source"
	cp tclock.py tclock-deb/usr/share/tclock/tclock.py
	echo "[3/6] - making compiled executable"
	chmod 771 tclock-deb/usr/share/tclock/tclock
	echo "[4/6] - making python executable"
	chmod 771 tclock-deb/usr/share/tclock/tclock.py
	echo "[5/6] - Packaging For Debian"
	dpkg --build tclock-deb
	echo "[6/6] - Done"
}||{
	echo "Fail"
}
