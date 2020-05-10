#!/bin/sh
{
    echo "[0/1] Copying files to computer"
    cp -r ./usr/ /
    echo "[1/1] Install Complete"
}||{
    echo "Failed, please make sure you run `install.sh` with super user privliges or merge ./usr with /usr"
}
