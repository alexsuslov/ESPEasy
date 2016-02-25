#!/bin/bash


time curl -v "http://10.0.1.32/api/"
sleep 5

time curl -v "http://10.0.1.32/api/hardware"
sleep 5

time curl -v "http://10.0.1.32/api/config"
sleep 5

time curl -v "http://10.0.1.32/api/devices"
sleep 5

time curl -v "http://10.0.1.32/api/device"
sleep 5

time curl -v "http://10.0.1.32/api/wifiscanner"
sleep 5

# time curl -v "http://10.0.1.32/api/auth"
# sleep 5

time curl -v "http://10.0.1.32/api/log"
sleep 5

# time curl -v "http://10.0.1.32/api/pass"