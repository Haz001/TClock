{
	python -m nuitka --standalone tclock.py
	cp tclock.dist/tclock tclock-deb/usr/share/tclock/tclock
	dpkg --build tclock-deb
	echo "Done"
}||{
	echo "Fail"
}
