#!/bin/bash
/bin/bash
socat TCP-LISTEN:8080,reuseaddr,fork EXEC:'su kanna -c /home/kanna/main'
sleep infinity
