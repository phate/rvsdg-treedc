#!/bin/bash

# load extra colors if exsits
colorbrewer_src=$HOME/etc/gnuplot-colorbrewer/qualitative/Dark2.plt
[ -e $HOME/etc/gnuplot-colorbrewer ] &&
    colorbrewer="load '$colorbrewer_src'"

gnuplot <<EOF
set terminal pngcairo enhanced font "TeX Gyre Pagella, 12" size 1200,600
set output '/dev/null'
$colorbrewer
load '$1.plg'
set output '$1.png'
replot
EOF
