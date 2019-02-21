//default RCP feedback throughput and control interval
#define INIT_FEEDBACK_THPUT 20000
#define INIT_CONTROL_INTERVAL 200

#define NUM_PORTS 64

//Link capacity (per port)
#define C 40*1024*1024*1024

// Total number of bytes received so far.
int bytes_received = 0;

// Current feedback throughput of this router
int curr_feedback_thput = INIT_FEEDBACK_THPUT;

// RCP control interval
int control_interval = INIT_CONTROL_INTERVAL;

//last time when RCP rate was calculated
int last_time = 0;

//RTT per ports
int avg_rtt [NUM_PORTS] = {200};

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

  //Update feedback throughput to the packet
  if (pkt.feedback_thput > curr_feedback_thput) {
    pkt.feedback_thput = curr_feedback_thput;  
  }

  pkt.id = pkt.dport;
  //1 = .98 and 2 = .02 
  avg_rtt[pkt.id] = avg_rtt[pkt.id] * 1 + pkt.rtt * 2;
  
  if ((pkt.time - last_time) < control_interval) {
    bytes_received += pkt.size_bytes; 
  }
  else {
    control_interval = avg_rtt[pkt.id];
    bytes_received = 0;
//RCP stability constants alpha=1 beta=.5
//    curr_feedback_thput = curr_feedback_thput * (1 + (((C - (bytes_received/control_interval))) - ((pkt.queue/2)/control_interval))/C);
  }
}
