#!/bin/bash

service nginx start
nginx -s reload

while true; do
    sleep 1
done
