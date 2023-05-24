#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#define MAXCP 2
#define INC 1000

void compute(int id);
void ready_handler(int sig);
void done_handler(int sig);
void compute_handler();
void print_handler(int sig);
void terminate_handler();
void report_ready(int pid);
void report_done(int pid);
void synchronize();
void print_results();

#endif