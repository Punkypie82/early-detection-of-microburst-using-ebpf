# Version 0.4

This version of the project uses an eBPF C program, compiled with `clang`, to monitor network traffic. It attaches to the **egress traffic control (tc) hook point** to demonstrate basic packet monitoring and classification. For each outgoing packet, the program extracts and prints the full **five-tuple** information to the tracing pipe.

A **five-tuple** is a set of five values that uniquely identifies a TCP/IP connection:

1.  Source IP Address
2.  Destination IP Address
3.  Source Port
4.  Destination Port
5.  Protocol (e.g., TCP, UDP)

-----

## Prerequisites

To compile and run this project, you will need the following dependencies installed on your system:

  * **`clang`**: The compiler for the C eBPF program.
  * **`libbpf-dev`**: Development headers and libraries for `libbpf`.
  * **`linux-headers`**: Kernel headers for your specific Linux distribution.
  * **`iproute2`**: The utility suite containing the `tc` command.

-----

## How to run

This project includes three helper scripts to manage the eBPF program. They require `sudo` privileges to run.

### 1\. Compile the eBPF Program

The `compile.sh` script automatically detects your system's architecture (`x86_64` or `aarch64`) and uses the correct `clang` flag to compile `classifier.c` into an eBPF object file (`classifier.o`).

```bash
sudo ./compile.sh
```

### 2\. Attach the eBPF Program

The `attach.sh` script installs the eBPF program on a specified network interface. It requires the interface name as a command-line argument.

```bash
sudo ./attach.sh <INTERFACE_NAME>
```

| Parameter | Description |
| :--- | :--- |
| `<INTERFACE_NAME>` | The name of the network interface to monitor (e.g., `eth0` or `enp0s1`). |

### 3\. Clean Up and Detach

To remove the eBPF program and its associated traffic control hooks, use the `cleanup.sh` script with the same interface name.

```bash
sudo ./cleanup.sh <INTERFACE_NAME>
```

-----

## Monitoring Output

You can monitor the output from the `bpf_trace_printk` calls in a separate terminal window. The output will show the complete five-tuple for each outgoing packet.

To view the output, run:

```bash
sudo cat /sys/kernel/debug/tracing/trace_pipe
```
