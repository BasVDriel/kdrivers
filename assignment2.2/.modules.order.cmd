cmd_/home/pi/kdrivers/assignment2.2/modules.order := {   echo /home/pi/kdrivers/assignment2.2/module.ko; :; } | awk '!x[$$0]++' - > /home/pi/kdrivers/assignment2.2/modules.order
