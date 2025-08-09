# Version 1.1

This version enhances the user-space monitoring application to provide real-time updates of network flow counters. The eBPF program, written in C, populates a **BPF map** with five-tuple flow information and a packet counter for each unique flow. The `flow_counter_reader_auto` user-space program continuously reads and displays the contents of this map, refreshing the output every second until the user stops it. This provides a dynamic view of network traffic patterns.

A **five-tuple** is a set of five values that uniquely identifies a TCP/IP connection: source IP address, destination IP address, source port, destination port, and protocol.

-----

## Prerequisites

To compile and run this project, you will need the following dependencies installed on your system:

  * **`clang`**: The compiler for the C eBPF program.
  * **`libbpf-dev`**: Development headers and libraries for `libbpf`.
  * **`linux-headers`**: Kernel headers for your specific Linux distribution.
  * **`iproute2`**: The utility suite containing the `tc` command.
  * **`gcc`**: The GNU Compiler Collection for building the user-space program.

-----

## How to run

This project includes a set of helper scripts and a user-space application to manage the eBPF program. They require `sudo` privileges to run.

### 1\. Compile the eBPF Programs

Compile the C eBPF program into an object file and the user-space C program into an executable.

```bash
# Compile the eBPF program (classifier.c)
sudo ./compile.sh

# Compile the user-space reader program (flow_counter_reader_auto.c)
gcc -o flow_counter_reader_auto flow_counter_reader_auto.c -lbpf
```

### 2\. Attach the eBPF Program and Map

The `attach.sh` script installs the eBPF program on a specified network interface and **pins the BPF map** to the BPF file system. This makes the map accessible to the user-space application.

```bash
sudo ./attach.sh <INTERFACE_NAME>
```

| Parameter | Description |
| :--- | :--- |
| `<INTERFACE_NAME>` | The name of the network interface to monitor (e.g., `eth0` or `enp0s1`). |

### 3\. Monitor Flow Counters

Once the eBPF program is attached, you can run the `flow_counter_reader_auto` executable at any time to see the continuously updated flow counters.

```bash
sudo ./flow_counter_reader_auto
```

### 4\. Clean Up and Detach

To remove the eBPF program, its associated traffic control hooks, and unpin the BPF map, use the `cleanup.sh` script with the same interface name.

```bash
sudo ./cleanup.sh <INTERFACE_NAME>
```
