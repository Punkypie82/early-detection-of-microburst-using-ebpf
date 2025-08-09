#!/bin/sh
sudo tc qdisc add dev $1 clsact
sudo tc filter add dev $1 egress bpf obj classifier.o sec "tc"
