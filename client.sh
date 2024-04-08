#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <message>"
    exit 1
fi

USER_MESSAGE=$1

SERVER="localhost"
PORT=15000

{
    read -r line
    echo "Received from server: $line"

    while IFS= read -r line
    do
        echo "Received from server: $line"
        if [[ "$line" == "Ready" ]]; then
            break
        fi
    done

    echo "$USER_MESSAGE"
} | nc $SERVER $PORT
