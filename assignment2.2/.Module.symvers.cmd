cmd_/home/pi/kdrivers/assignment2.2/Module.symvers := sed 's/\.ko$$/\.o/' /home/pi/kdrivers/assignment2.2/modules.order | scripts/mod/modpost -m -a  -o /home/pi/kdrivers/assignment2.2/Module.symvers -e -i Module.symvers   -T -