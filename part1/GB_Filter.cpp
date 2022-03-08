#include <cmath>

#include "GB_Filter.h"

GB_Filter::GB_Filter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// GB mask
const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

void GB_Filter::do_filter() {
  while (true) {
    val_r = 0;
    val_g = 0;
    val_b = 0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        val_r += i_r.read() * mask[u][v];
        val_g += i_g.read() * mask[u][v];
        val_b += i_b.read() * mask[u][v];
      }
    }
    int result_r = (int)(val_r / 16.0);
    int result_g = (int)(val_g / 16.0);
    int result_b = (int)(val_b / 16.0);

    if (result_r > 255)
      o_result_r.write(255);
    else
      o_result_r.write(result_r);

    if (result_g > 255)
      o_result_g.write(255);
    else
      o_result_g.write(result_g);

    if (result_b > 255)
      o_result_b.write(255);
    else
      o_result_b.write(result_b);
    wait(10); //emulate module delay
  }
}
