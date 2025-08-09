#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("tc")
int hello_egress(struct __sk_buff *skb) {
    char msg[] = "Hello Packet!";
    bpf_trace_printk(msg, sizeof(msg));
    return BPF_OK;
}

char _license[] SEC("license") = "GPL";
