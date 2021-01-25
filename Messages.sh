#!/bin/bash

TRANSDIR=translations

LUPDATE=`which lupdate-qt5`
if [ ! -x $LUPDATE ]
then
    LUPDATE=`which lupdate`
fi

if [ -x $LUPDATE ]
then
    echo "Found lupdate at $LUPDATE"
else
    echo "Can neither find lupdate-qt5 nor lupdate"
    exit 1
fi

if [ ! -d $TRANSDIR ]; then
    mkdir $TRANSDIR
    if [ ! -d $TRANSDIR ]; then
        echo "Failed to create translations directory!"
        exit 1
    fi
fi

$LUPDATE -no-obsolete -source-language en -target-language en -locations none Wolkanlin -ts $TRANSDIR/libwolkanlin.ts

