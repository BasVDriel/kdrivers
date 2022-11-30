cmd_/home/pi/kdrivers/assignment2.1/modules.order := {   echo /home/pi/kdrivers/assignment2.1/bad.ko; :; } | awk '!x[$$0]++' - > /home/pi/kdrivers/assignment2.1/modules.order
