#!/bin/bash
# set -eux
# Start the first process
./process1 &

# Start the second process
./process2 &

# Wait for any process to exit
wait -n

# Exit with status of process that exited first
exit $?
