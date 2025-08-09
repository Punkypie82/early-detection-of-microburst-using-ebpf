#!/bin/sh
sudo tc qdisc add dev $1 clsact
sudo tc filter add dev $1 egress bpf obj classifier.o sec "tc"
sudo bpftool map pin name flow_counters /sys/fs/bpf/tc/globals/flow_counters
