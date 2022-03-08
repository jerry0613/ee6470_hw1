#ifndef GB_FILTER_H_
#define GB_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class GB_Filter : public sc_module {
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in<unsigned char> i_r;
  sc_fifo_in<unsigned char> i_g;
  sc_fifo_in<unsigned char> i_b;
  sc_fifo_out<unsigned char> o_result_r;
  sc_fifo_out<unsigned char> o_result_g;
  sc_fifo_out<unsigned char> o_result_b;

  SC_HAS_PROCESS(GB_Filter);
  GB_Filter(sc_module_name n);
  ~GB_Filter() = default;

private:
  void do_filter();
  int val_r, val_g, val_b;
};
#endif
