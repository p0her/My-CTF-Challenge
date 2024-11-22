#!/bin/bash
/bin/bash
socat TCP-LISTEN:8080,reuseaddr,fork EXEC:'su kinga -c /home/kinga/main'
sleep infinity
