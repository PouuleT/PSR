#ifndef STRUCTURE
#define STRUCTURE

typedef struct node{
  int nb_emited;
	int nb_traited;
	int nb_recieved;
	int nb_destroyed;
	int nb_waiting;
	int avg_waiting;
}node;

typedef struct packet{
  int source;
  int destination;
  int pid;
  int fid;
  double departure;
  double last_time;
  int size;
  int pos;
}packet;

typedef struct mail{
  packet el;
  struct mail *next;
}mail;

typedef mail* chain;


typedef struct log{
  int nb_packets;
  int nb_flux;
	int nb_emited;
	int nb_traited;
	int nb_recieved;
	int nb_destroyed;
}log;

#endif
