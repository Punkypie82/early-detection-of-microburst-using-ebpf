#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

// Define a per-CPU array map with 1 element for the counter
struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, __u64);
} packet_counter SEC(".maps");

SEC("tc")
int hello_egress(struct __sk_buff *skb) {
    __u32 key = 0;

    // Get pointer to the counter
    __u64 *counter = bpf_map_lookup_elem(&packet_counter, &key);
    if (!counter)
        return BPF_OK;

    // Increment the counter atomically (per-CPU so atomic not strictly needed, but safe)
    __sync_fetch_and_add(counter, 1);

    void *data     = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return BPF_OK;

    if (eth->h_proto != __builtin_bswap16(ETH_P_IP))
        return BPF_OK;

    struct iphdr *ip = (void *)(eth + 1);
    if ((void *)(ip + 1) > data_end)
        return BPF_OK;

    __u8 protocol = ip->protocol;
    static const char proto_fmt[] = "Packet #%llu Protocol: %d\n";
    bpf_trace_printk(proto_fmt, sizeof(proto_fmt), *counter, protocol);

    static const char src_fmt[] = "Packet #%llu Src IP: %u\n";
    bpf_trace_printk(src_fmt, sizeof(src_fmt), *counter, (__u32)ip->saddr);

    static const char dst_fmt[] = "Packet #%llu Dst IP: %u\n";
    bpf_trace_printk(dst_fmt, sizeof(dst_fmt), *counter, (__u32)ip->daddr);

    if (protocol == IPPROTO_TCP || protocol == IPPROTO_UDP) {
        __u16 *ports = (void *)(ip + 1);
        if ((void *)(ports + 2) > data_end)
            return BPF_OK;

        __u16 src_port = __builtin_bswap16(ports[0]);
        __u16 dst_port = __builtin_bswap16(ports[1]);

        static const char sport_fmt[] = "Packet #%llu Src Port: %d\n";
        bpf_trace_printk(sport_fmt, sizeof(sport_fmt), *counter, src_port);

        static const char dport_fmt[] = "Packet #%llu Dst Port: %d\n";
        bpf_trace_printk(dport_fmt, sizeof(dport_fmt), *counter, dst_port);
    }

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";
