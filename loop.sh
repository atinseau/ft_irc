#!/bin/bash

let i=0

nc -c localhost 10000 <<EOF

NICK thomas

USER thomas * 0 thomas

PASS 06112001

HELP

INFO

PRIVMSG arthur salut mon gars ! 

EOF

# NICK arthur\nUSER arhutr 0 * arthu" | nc -c localhost 10000

# while true
# do
# 	echo "^C" | nc -C localhost 10000 
# 	sleep 0.2
# done