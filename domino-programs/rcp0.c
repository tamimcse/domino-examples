// rcp.c
#define C 1000 //Capacity of the line card
#define T 50 //Control Interval
#define A 20 //1000/T

//State variables
int avg_rtt = 200;
int feedback_rate = 200;
int bytes_received = 0;
int incoming_rate = 0;
int queue = 0;
int tmp_queue = 0;

//Packet headers and meta-data
struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int tick;
};

void func(struct Packet pkt) {
  //Calculate running average of RTT
  avg_rtt = (avg_rtt * 49 + pkt.rtt)/50;

  //Write the throughput in packet header
  if (pkt.feedback_thput > feedback_rate)
    pkt.feedback_thput = feedback_rate;  
  
  //Control interval has expired, so
  // calculate the feeback throughput
  // and reset the state variables
  if (pkt.tick % T == 0) {
    incoming_rate = C - bytes_received * A;
    bytes_received = 0;
    queue = tmp_queue;
    tmp_queue = 1000;
    feedback_rate += (C + (((C - \
      ((queue/avg_rtt)/2) - \
      incoming_rate) * T)/avg_rtt))/C;
  }
  else {
    bytes_received += pkt.size_bytes; 
  }

  //Keep track of the minimum queue size
  if (tmp_queue > pkt.queue)
    tmp_queue = pkt.queue;
}
