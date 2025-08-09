from bcc import BPF
import sys
from pyroute2 import IPRoute
import ctypes

# Define the BPF program
bpf_program = """
#define TC_ACT_OK 0

int hello_egress(struct __sk_buff *skb) {
    bpf_trace_printk("Hello Packet!\\n");
    return TC_ACT_OK;
}
"""

# Load the BPF program
b = BPF(text=bpf_program)
fn = b.load_func("hello_egress", BPF.SCHED_CLS)

# Check interface name
if len(sys.argv) != 2:
    print("Usage: sudo python3 hello_egress.py <interface>")
    exit(1)

iface = sys.argv[1]

# Set up pyroute2 and interface index
ipr = IPRoute()
ifidx = ipr.link_lookup(ifname=iface)[0]

# Safe delete of any previous clsact qdisc
try:
    ipr.tc("del", "clsact", ifidx)
except Exception as e:
    pass  # Safe to ignore "not found" or "invalid argument"

# Add clsact qdisc (allows ingress and egress hooks)
ipr.tc("add", "clsact", ifidx)

# Attach BPF program to egress
ipr.tc("add-filter", "bpf", ifidx, ":1", fd=fn.fd,
       name=fn.name, parent="ffff:fff3", classid=1,
       direct_action=True)

print(f"Attached egress hook to interface {iface}. Watching for packets...")

try:
    b.trace_print()
except KeyboardInterrupt:
    print("\nDetaching...")
    try:
        ipr.tc("del", "clsact", ifidx)
    except:
        pass
