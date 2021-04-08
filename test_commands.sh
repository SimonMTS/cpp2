#!/bin/bash
# just for copy/pasting
exit 0;

aSteg -df ./data/news.caf
aSteg -df ./data/input24bit.caf
aSteg -e $'Wat tést tékst 👌👌👌\nen🇳🇱wat🇳🇱multibyte🇳🇱chars 👍' -f ./data/news.caf > ./data/output.caf
aSteg -e $'Wat tést tékst 👌👌👌\nen🇳🇱wat🇳🇱multibyte🇳🇱chars 👍' -f ./data/input24bit.caf > ./data/output.caf
aSteg -df ./data/output.caf

aSteg -df ./data/de-oude-schicht.wav
aSteg -df ./data/input24bit.wav
aSteg -e $'Wat tést tékst 👌👌👌\nen🇳🇱wat🇳🇱multibyte🇳🇱chars 👍' -f ./data/ide-oude-schicht.wav > ./data/output.wav
aSteg -e $'Wat tést tékst 👌👌👌\nen🇳🇱wat🇳🇱multibyte🇳🇱chars 👍' -f ./data/input24bit.wav > ./data/output.wav
aSteg -df ./data/output.wav
