PORT=`ls /dev/cu.usb*`
arduino-cli compile --build-property build.extra_flags=-DMEGA --fqbn arduino:avr:mega --build-path=Mega/sketch Mega
arduino-cli upload -p $PORT --fqbn arduino:avr:mega --input-dir Mega/sketch Mega
