#!/bin/sh
sudo tc filter delete dev $1 egress
sudo tc qdisc delete dev $1 clsact
