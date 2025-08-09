#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

SEC("tc")
int hello_egress(struct __sk_buff *skb) {
    void *data     = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    // First, get the Ethernet header
    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return BPF_OK;

    // Only handle IPv4 packets
    if (eth->h_proto != __builtin_bswap16(ETH_P_IP))
        return BPF_OK;

    // Now skip Ethernet header to get IP header
    struct iphdr *ip = (void *)(eth + 1);
    if ((void *)(ip + 1) > data_end)
        return BPF_OK;

    // Protocol should now be correct
    __u8 protocol = ip->protocol;
    static const char proto_fmt[] = "Protocol: %d\n";
    bpf_trace_printk(proto_fmt, sizeof(proto_fmt), protocol);

    static const char src_fmt[] = "Src IP: %u\n";
    bpf_trace_printk(src_fmt, sizeof(src_fmt), (__u32)ip->saddr);

    static const char dst_fmt[] = "Dst IP: %u\n";
    bpf_trace_printk(dst_fmt, sizeof(dst_fmt), (__u32)ip->daddr);

    if (protocol == IPPROTO_TCP || protocol == IPPROTO_UDP) {
        __u16 *ports = (void *)(ip + 1);
        if ((void *)(ports + 2) > data_end)
            return BPF_OK;

        __u16 src_port = __builtin_bswap16(ports[0]);
        __u16 dst_port = __builtin_bswap16(ports[1]);

        static const char sport_fmt[] = "Src Port: %d\n";
        bpf_trace_printk(sport_fmt, sizeof(sport_fmt), src_port);

        static const char dport_fmt[] = "Dst Port: %d\n";
        bpf_trace_printk(dport_fmt, sizeof(dport_fmt), dst_port);
    }

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";
