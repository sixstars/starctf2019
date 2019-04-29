#!/bin/sh

service --status-all | awk '{print $4}'| xargs  -i service {} start

sleep infinity;