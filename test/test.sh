#!/bin/bash


time curl -v "http://10.0.1.32/"
sleep 5

time curl -v "http://10.0.1.32/config"
sleep 5

time curl -v "http://10.0.1.32/hardware"
sleep 5

time curl -v "http://10.0.1.32/devices"
sleep 5

time curl -v "http://10.0.1.32/devices?index=1&page=1"
sleep 5

time curl -v "http://10.0.1.32/devices?index=2&page=1"
sleep 5

time curl -v "http://10.0.1.32/tools"
sleep 5

time curl -v "http://10.0.1.32/wifiscanner"
sleep 5

time curl -v "http://10.0.1.32/log"
sleep 5