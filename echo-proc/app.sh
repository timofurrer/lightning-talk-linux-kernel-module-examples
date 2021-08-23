#!/usr/bin/env bash

read -r input
echo -n "$input" > /proc/echo
cat /proc/echo
