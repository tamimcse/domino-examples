#define NUM_PORTS 64
#define C 10000000 //Link capacity (per port)

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


struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int time;
  int dport;
  int id; // array index
};

void func(struct Packet pkt) {
  pkt.id = pkt.dport;

  //Update feedback throughput to the packet
  if (pkt.feedback_thput > feedback_rate[pkt.id]) {
    pkt.feedback_thput = feedback_rate[pkt.id];  
  }

  //1 = .98 and 2 = .02 
  avg_rtt[pkt.id] = avg_rtt[pkt.id] * 1 + pkt.rtt * 2;
  
  if ((pkt.time - last_time) < control_intervals[pkt.id]) {
    bytes_received[pkt.id] += pkt.size_bytes; 
  }
  else {
    incoming_rate[pkt.id] = bytes_received[pkt.id]/control_intervals[pkt.id];
    control_intervals[pkt.id] = avg_rtt[pkt.id];
    bytes_received[pkt.id] = 0;
//RCP stability constants alpha=1 beta=.5
//    feedback_rate[pkt.id] = feedback_rate[pkt.id] * (1 + ((C - incoming_rate[pkt.id]) - ((pkt.queue/2)/avg_rtt[pkt.id]))/C);
    feedback_rate[pkt.id] += 2;
  }
}
