#!/bin/sh
touch ChangeLog

if [ -e config.guess ]
then rm config.guess
fi

if [ -e config.sub ]
then rm config.sub
fi

if [ -e ltmain.sh ]
then rm ltmain.sh
fi

autoreconf -i -s
