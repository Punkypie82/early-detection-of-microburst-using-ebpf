# Version 1.0

This version represents a major architectural shift, moving from kernel-based debug printing to a more robust user-space monitoring application. The eBPF program, written in C, now populates a **BPF map** with five-tuple flow information and a packet counter for each unique flow. A separate user-space program, `flow_counter_reader`, reads and displays the contents of this map in a clear, formatted table. This architecture provides a more reliable and efficient way to monitor network flows without the performance overhead of `bpf_trace_printk`.

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

## Usage

This project includes a set of helper scripts and a user-space application to manage the eBPF program. They require `sudo` privileges to run.

### 1\. Compile the eBPF Programs

Compile the C eBPF program into an object file and the user-space C program into an executable.

```bash
# Compile the eBPF program (classifier.c)
sudo ./compile.sh

# Compile the user-space reader program (flow_counter_reader.c)
gcc -o flow_counter_reader flow_counter_reader.c -lbpf
```

### 2\. Attach the eBPF Program and Map

The `attach.sh` script installs the eBPF program on a specified network interface and **pins the BPF map** to the BPF file system. This makes the map accessible to user-space applications.

```bash
sudo ./attach.sh <INTERFACE_NAME>
```

| Parameter | Description |
| :--- | :--- |
| `<INTERFACE_NAME>` | The name of the network interface to monitor (e.g., `eth0` or `enp0s1`). |

### 3\. Monitor Flow Counters

Once the eBPF program is attached, you can run the `flow_counter_reader` executable at any time to get a snapshot of the current flow counters.

```bash
sudo ./flow_counter_reader
```

### 4\. Clean Up and Detach

To remove the eBPF program, its associated traffic control hooks, and unpin the BPF map, use the `cleanup.sh` script with the same interface name.

```bash
sudo ./cleanup.sh <INTERFACE_NAME>
```
