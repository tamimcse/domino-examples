#define NUM_PORTS 64
#define C 1000 //Link capacity (per port)
#define B 2000 //2 * C
#define LEVEL16_SIZE 65536

//last time when RCP rate was calculated
int last_time = 0;

//RTT per ports
int avg_rtt [NUM_PORTS] = {200};
//RCP Control Intervals for each port
int control_intervals [NUM_PORTS] = {200};
//RCP feedback throughput for each port
int feedback_rate [NUM_PORTS] = {200};
//bytes recieved per port
int bytes_received [NUM_PORTS] = {0};
//incoming rate per port
int incoming_rate [NUM_PORTS] = {0};

int def_nh = 1;
int N16[LEVEL16_SIZE] = {0}; 


struct Packet {
  int dport;
  int daddr;
  int idx16;
};

void func(struct Packet pkt) {
//  pkt.ifindex = def_nh;
  pkt.idx16 = pkt.daddr >> 16;

  if (N16[pkt.idx16] != 0) {
    pkt.dport = N16[pkt.idx16]; 
  }

/*
  avg_rtt[pkt.id] = (avg_rtt[pkt.id] * 49 + pkt.rtt)/50;
  feedback_rate[pkt.id] += (B - ((pkt.queue/avg_rtt[pkt.id])/2) - incoming_rate[pkt.id])/C;
  //Update feedback throughput to the packet
  if (pkt.feedback_thput > feedback_rate[pkt.id]) {
    pkt.feedback_thput = feedback_rate[pkt.id];  
  }
  
  if (pkt.tick % 60 == 0) {
    incoming_rate[pkt.id] = C - bytes_received[pkt.id];
    bytes_received[pkt.id] = 0;
  }
  else {
    bytes_received[pkt.id] += pkt.size_bytes; 
  }
*/
  
}
