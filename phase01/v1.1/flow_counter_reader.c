#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <errno.h>
#include <string.h>

struct flow_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u8 protocol;
};

static const char *protocol_to_str(__u8 proto) {
    switch (proto) {
        case 6:  return "TCP";
        case 17: return "UDP";
        default: return "Unknown";
    }
}

static void print_ip(__u32 ip) {
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip, ip_str, sizeof(ip_str));
    printf("%-18s", ip_str); // Fixed-width formatting for IP alignment
}

int main(int argc, char **argv) {
    const char *map_name = "flow_counters";
    int map_fd;
    struct bpf_map_info info = {0};
    __u32 info_len = sizeof(info);
    struct flow_key key, next_key;
    __u64 count;

    // Open the BPF map
    map_fd = bpf_obj_get("/sys/fs/bpf/tc/globals/flow_counters");
    if (map_fd < 0) {
        fprintf(stderr, "Failed to open BPF map: %s\n", strerror(errno));
        return 1;
    }

    // Get map info
    if (bpf_obj_get_info_by_fd(map_fd, &info, &info_len)) {
        fprintf(stderr, "Failed to get map info: %s\n", strerror(errno));
        close(map_fd);
        return 1;
    }

    // Run indefinitely, refreshing every 2 seconds
    while (1) {
        // Clear the terminal
        system("clear");

        printf("Flow Counters (Map: %s, Max Entries: %u):\n", map_name, info.max_entries);
        printf("%-17s %-17s %-10s %-10s %-10s %-12s\n",
               "Source IP", "Destination IP", "Src Port", "Dst Port", "Protocol", "Packet Count");

        // Reset key for iteration
        memset(&key, 0, sizeof(key));
        // Iterate through all entries in the map
        while (bpf_map_get_next_key(map_fd, &key, &next_key) == 0) {
            if (bpf_map_lookup_elem(map_fd, &next_key, &count) == 0) {
                print_ip(next_key.src_ip);
                print_ip(next_key.dst_ip);
                printf("%-10u %-10u %-10s %-12llu\n",
                       ntohs(next_key.src_port),
                       ntohs(next_key.dst_port),
                       protocol_to_str(next_key.protocol),
                       count);
            }
            key = next_key;
        }

        // Flush output to ensure it displays immediately
        fflush(stdout);

        // Wait for 2 seconds before next refresh
        sleep(1);
    }

    close(map_fd);
    return 0;
}
