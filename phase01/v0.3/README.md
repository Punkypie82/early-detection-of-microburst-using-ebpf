# Version 0.3

This project uses an eBPF C program, compiled with `clang`, to monitor network traffic. It attaches to the **egress traffic control (tc) hook point** to demonstrate basic packet monitoring and classification. For each outgoing packet detected on a specified network interface, the program prints a message containing the packet's protocol id. This version builds on previous iterations by using helper scripts that dynamically handle system architecture and network interfaces.

-----

## Prerequisites

To compile and run this project, you will need the following dependencies installed on your system:

  * **`clang`**: The compiler for the C eBPF program.
  * **`libbpf-dev`**: Development headers and libraries for `libbpf`.
  * **`linux-headers`**: Kernel headers for your specific Linux distribution.
  * **`iproute2`**: The utility suite containing the `tc` command.

-----

## Usage

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

You can monitor the output from the `bpf_trace_printk` calls in a separate terminal window. The output will show a message for each outgoing packet, including its protocol number.

To view the output, run:

```bash
sudo cat /sys/kernel/debug/tracing/trace_pipe
```
