# Version 0.1

This is the initial Python BCC version of the eBPF code. It attaches to the **egress traffic control (tc) hook point** to demonstrate basic packet monitoring. The script prints the message "Hello Packet!" to the console each time an outgoing packet is detected on the specified network interface.

---

## Prerequisites

To run this script, you will need the following dependencies installed:

* **Python 3**
* **BCC (BPF Compiler Collection)**
* **`pyroute2`**: A Python library for managing network interfaces and routing tables.
* **`ctypes`**: Python's foreign function library, used to call functions from dynamic libraries.

You can install the Python libraries using pip:
```bash
pip3 install bcc pyroute2
````

-----

## How to Run

Execute the Python script with superuser privileges, providing your network interface name as a command-line argument.

```bash
sudo python3 hello_egress.py <INTERFACE_NAME>
```

| Parameter | Description |
| :--- | :--- |
| `<INTERFACE_NAME>` | The name of the network interface to monitor (e.g., `eth0` or `enp0s1`). |
