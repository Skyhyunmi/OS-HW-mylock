#!/bin/bash

cat $* | sort -n | uniq -c | awk 'BEGIN {sum = 0} {print $2 " " $1; sum += $1} END {print sum}'
