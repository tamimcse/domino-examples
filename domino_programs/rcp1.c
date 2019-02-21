//default RCP feedback throughput and control interval
#define INIT_FEEDBACK_THPUT 20000
#define INIT_CONTROL_INTERVAL 200

//Link capacity (per port)
#define C 40*1024*1024*1024

// Total number of bytes received so far.
int bytes_received = 0;

// Sum of rtt so far
int sum_rtt = 0;

// Number of packets with a valid RTT
int num_pkts_seen = 0;

// Current feedback throughput of this router
int curr_feedback_thput = INIT_FEEDBACK_THPUT;

// RCP control interval
int control_interval = INIT_CONTROL_INTERVAL;

//last time when RCP rate was calculated
int last_time = 0;

struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int time;	
};

void func(struct Packet pkt) {

  //Update feedback throughput to the packet
  if (pkt.feedback_thput > curr_feedback_thput) {
    pkt.feedback_thput = curr_feedback_thput;  
  }
  
  if ((pkt.time - last_time) < control_interval) {
    sum_rtt += pkt.rtt;
    num_pkts_seen += 1;
    bytes_received += pkt.size_bytes; 
  }
  else {
//    control_interval = sum_rtt/num_pkts_seen;
    num_pkts_seen = 0;
    sum_rtt = 0;
    bytes_received = 0;
//RCP stability constants alpha=1 beta=.5
//    curr_feedback_thput = curr_feedback_thput * (1 + (((C - (bytes_received/control_interval))) - ((pkt.queue/2)/control_interval))/C);
  }
}
