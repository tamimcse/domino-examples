int avg_rtt = 0;
int sum_rtt = 0;
int num_pkts_seen = 5;

struct Packet {
  int tick;	
};

void func(struct Packet pkt) {
//    avg_rtt = sum_rtt + num_pkts_seen;
    avg_rtt = sum_rtt / 200;//num_pkts_seen;
}
