# Early Detection of Microbursts - Version 0.2

This version introduces the use of a native eBPF C program, compiled with `clang`, for greater performance and flexibility. It attaches to the **egress traffic control (tc) hook point** to demonstrate basic packet monitoring. The compiled eBPF code prints the message "Hello Packet\!" to the console each time an outgoing packet is detected on a specified network interface.

-----

## Prerequisites

To compile and run this code, you will need the following dependencies installed:

  * **`clang`**: The compiler for the C eBPF program.
  * **`libbpf-dev`**: Development headers and libraries for `libbpf`.
  * **`linux-headers`**: Kernel headers for your specific Linux distribution.
  * **`iproute2`**: The utility suite containing the `tc` command.

-----

## How to run

This project includes a set of helper scripts to compile, attach, and detach the eBPF program. You must run these scripts with `sudo` privileges.

### 1\. Compile the eBPF Program

First, compile the `classifier.c` source code into an eBPF object file (`classifier.o`). The `compile.sh` script automatically detects your system's architecture (`x86_64` or `aarch64`) and sets the correct `clang` flag.

```bash
sudo ./compile.sh
```

### 2\. Attach the eBPF Program

Next, attach the compiled `classifier.o` to your desired network interface. The `attach.sh` script requires you to provide the interface name as a command-line argument.

```bash
sudo ./attach.sh <INTERFACE_NAME>
```

| Parameter | Description |
| :--- | :--- |
| `<INTERFACE_NAME>` | The name of the network interface to monitor (e.g., `eth0` or `enp0s1`). |

### 3\. Clean Up and Detach

To remove the eBPF program and its associated traffic control hooks, use the `cleanup.sh` script, providing the same interface name.

```bash
sudo ./cleanup.sh <INTERFACE_NAME>
```

-----

## Monitoring Output

You can monitor the output from the `bpf_trace_printk` calls in a separate terminal window by using the `sudo cat /sys/kernel/debug/tracing/trace_pipe` command. The output will show the "Hello Packet\!" message for each outgoing packet.
