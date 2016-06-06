# heron_water
Clearpath Robotics Heron Water Sampler

This package provides access to the water sampler.

publishing an Int8 on /pump_ctl:
+ve values are sample input
-ve values are sample line flush
pumps 1 to 8 are controlled by publishing that integer.
Any value >8 (or less than -8) will cause all pumps to execute the command (+ve or negative)
publish 0 to stop all pumps

For example:
If pump 4 needs to take a sample:
rostopic pub /pump_ctl std_msgs/Int8 "data: 4" -1

Once enough time has passed, pump can be commanded to stop:
rostopic pub /pump_ctl std_msgs/Int8 "data: 0" -1

If pump 6 needs to pre-fill the line:
rostopic pub /pump_ctl std_msgs/Int8 "data: -6" -1

When button on the device is pressed, pump system goes through a cleaning purge and fill cycle

If installing to a new robot:
execute catkin_make install
copy launch file to /etc/ros/hydro/kingfisher-core.d
add udev rule for ttyS4 and ttyS5 to 0666 on statrup

KERNEL=="ttyS4", MODE="0666"

KERNEL=="ttyS5", MODE="0666"

