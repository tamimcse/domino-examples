#define NUM_PORTS 64
#define C 1000 //Link capacity (per port)
#define B 2000 //2 * C

//last time when RCP rate was calculated
int last_time = 0;

int avg_rtt = 200;
int control_intervals = 200;
int feedback_rate = 200;
int bytes_received = 0;
int incoming_rate = 0;
int queue = 0;
int tmp_queue = 0;


struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int time;
  int tick;
  int dport;
  int id; // array index
};

void func(struct Packet pkt) {
  avg_rtt = (avg_rtt * 49 + pkt.rtt)/50;

  if (pkt.feedback_thput > feedback_rate) {
    pkt.feedback_thput = feedback_rate;  
  }
  
  if (pkt.tick % 60 == 0) {
    incoming_rate = C - bytes_received;
    bytes_received = 0;
    queue = tmp_queue;
    tmp_queue = 1000;
    feedback_rate += (C + C - ((queue/avg_rtt)/2) - incoming_rate)/C;
  }
  else {
    bytes_received += pkt.size_bytes; 
  }

  if (tmp_queue > pkt.queue)
    tmp_queue = pkt.queue;

/*  if ((pkt.time - last_time) < control_intervals) {
    if (tmp_queue > pkt.queue)
      tmp_queue = pkt.queue;  
  }*/

  
}
