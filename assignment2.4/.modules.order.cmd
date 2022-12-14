cmd_/home/pi/kdrivers/assignment2.4/modules.order := {   echo /home/pi/kdrivers/assignment2.4/kmod.ko; :; } | awk '!x[$$0]++' - > /home/pi/kdrivers/assignment2.4/modules.order
