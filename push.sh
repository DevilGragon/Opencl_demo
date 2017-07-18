#!/bin/bash
echo "Moving Excutable Files to Phone..."
cd obj/local/arm64-v8a
adb push opencl* /data/local/gaor/caffe_test
cd ../../..
echo "Moving Finished" 
