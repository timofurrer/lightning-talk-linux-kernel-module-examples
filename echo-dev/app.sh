#!/usr/bin/env bash

read -r input
echo -n "$input" > /dev/echo
cat /dev/echo
