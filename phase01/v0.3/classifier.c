#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <bpf/bpf_helpers.h>

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

SEC("tc")
int hello_egress(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    struct ethhdr *eth = data;
    if ((void*)(eth + 1) > data_end)
        return BPF_OK;

    if (eth->h_proto != __constant_htons(ETH_P_IP))
        return BPF_OK;

    struct iphdr *ip = (void*)(eth + 1);
    if ((void*)(ip + 1) > data_end)
        return BPF_OK;

    // Protocol number from IP header
    __u8 protocol = ip->protocol;

//    bpf_trace_printk("Hello Packet!", 13);
    static const char fmt[] = "Hello Packet! Proto: %d\n";
    bpf_trace_printk(fmt, sizeof(fmt), protocol);

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";
