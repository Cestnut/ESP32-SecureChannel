#!/bin/bash
host_address=$1
messages_number=$2
output=$(ping $host_address -c $messages_number | tail -n 1 | awk '{print $4}' | cut -d '/' -f 2,4 --output-delimiter=" ")
echo $output > "${host_address}_ping_benchmark"