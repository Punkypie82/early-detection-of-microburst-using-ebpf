#!/bin/sh

# Determine the system's architecture
ARCH=$(uname -m)

# Set the BPF target architecture based on the system's architecture
if [ "$ARCH" = "aarch64" ]; then
    TARGET_ARCH="-D__TARGET_ARCH_arm64"
else
    # Default to x86_64 for all other architectures
    TARGET_ARCH="-D__TARGET_ARCH_x86"
fi

clang -O2 -g -target bpf $TARGET_ARCH -c classifier.c -o classifier.o
