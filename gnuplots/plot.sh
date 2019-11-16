#!/bin/bash

gnuplot << EOF
set terminal pngcairo enhanced font "TeX Gyre Pagella, 12" size 1200,600
set output '/dev/null'
load '$1.plg'
set output '$1.png'
replot
EOF
