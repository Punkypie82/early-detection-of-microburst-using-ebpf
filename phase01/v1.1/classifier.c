#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

struct flow_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u8 protocol;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, struct flow_key);
    __type(value, __u64);
} flow_counters SEC(".maps");

SEC("tc")
int count_flows(struct __sk_buff *skb) {
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

    struct flow_key key = {};
    key.src_ip = ip->saddr;
    key.dst_ip = ip->daddr;
    key.protocol = ip->protocol;

    __u16 src_port = 0, dst_port = 0;
    void *trans_hdr = (void *)(ip + 1);

    if (key.protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = trans_hdr;
        if ((void *)(tcp + 1) > data_end)
            return BPF_OK;
        src_port = tcp->source;
        dst_port = tcp->dest;
    } else if (key.protocol == IPPROTO_UDP) {
        struct udphdr *udp = trans_hdr;
        if ((void *)(udp + 1) > data_end)
            return BPF_OK;
        src_port = udp->source;
        dst_port = udp->dest;
    }
    key.src_port = src_port;
    key.dst_port = dst_port;

    __u64 *count = bpf_map_lookup_elem(&flow_counters, &key);
    if (count) {
        __sync_fetch_and_add(count, 1);
    } else {
        __u64 initial_count = 1;
        bpf_map_update_elem(&flow_counters, &key, &initial_count, BPF_ANY);
    }

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";
