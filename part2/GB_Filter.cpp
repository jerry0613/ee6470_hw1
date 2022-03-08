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
  int x, y;
  for (int i = 0; i < 256; ++i) {
    buffer_r[0][i] = 0;
    buffer_r[1][i] = 0;
    buffer_r[2][i] = 0;
    buffer_g[0][i] = 0;
    buffer_g[1][i] = 0;
    buffer_g[2][i] = 0;
    buffer_b[0][i] = 0;
    buffer_b[1][i] = 0;
    buffer_b[2][i] = 0;
  }
  while (true) {
    /*if(i_pixel_x.num_available()==0) wait(i_pixel_x.data_written_event());
    if(i_pixel_y.num_available()==0) wait(i_pixel_y.data_written_event());*/
    x = i_pixel_x.read();
    y = i_pixel_y.read();
    val_r = 0;
    val_g = 0;
    val_b = 0;

    // row buffer
    buffer_r[0][x] = buffer_r[1][x];
    buffer_r[1][x] = buffer_r[2][x];
    buffer_r[2][x] = i_r.read();
      
    buffer_g[0][x] = buffer_g[1][x];
    buffer_g[1][x] = buffer_g[2][x];
    buffer_g[2][x] = i_g.read();

    buffer_b[0][x] = buffer_b[1][x];
    buffer_b[1][x] = buffer_b[2][x];
    buffer_b[2][x] = i_b.read();
    // do GB filter
    if (y >= 1 && x >= 1) {
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          if ((u + x - 1) >= 0 && (u + x - 1) < 256) {
            val_r += buffer_r[v][u + x - 1] * mask[v][u];
            val_g += buffer_g[v][u + x - 1] * mask[v][u];
            val_b += buffer_b[v][u + x - 1] * mask[v][u];
          }
        }
      }

      int result_r = (int)(val_r / 16);
      int result_g = (int)(val_g / 16);
      int result_b = (int)(val_b / 16);

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
    }
        
    wait(1); //emulate module delay
  }
}
