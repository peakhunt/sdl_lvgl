#include "lvgl/lvgl.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

typedef struct
{
  lv_obj_t * bar;
  lv_obj_t * arc1;
  lv_obj_t * arc2;
  lv_obj_t * chart;
  lv_chart_series_t * ser;
  int t;
} sim_widgets_t;

// Simulation timer callback
static void
sim_timer_cb(lv_timer_t * timer)
{
  sim_widgets_t * w = (sim_widgets_t *) lv_timer_get_user_data(timer);
  w->t++;

  // Smooth sine baseline + small noise
  int base = 50 + 30 * sin(w->t * 0.1);
  int bar_val  = base + (rand() % 5 - 2);
  int arc1_val = 60 + 20 * sin(w->t * 0.08) + (rand() % 3 - 1);
  int arc2_val = 40 + 25 * sin(w->t * 0.12) + (rand() % 4 - 2);

  // Clamp values
  if (bar_val < 0)
    bar_val = 0;
  if (bar_val > 100)
    bar_val = 100;

  if (arc1_val < 0)
    arc1_val = 0;
  if (arc1_val > 100)
    arc1_val = 100;

  if (arc2_val < 0)
    arc2_val = 0;
  if(arc2_val > 100)
    arc2_val = 100;

  // Animate updates
  lv_bar_set_value(w->bar, bar_val, LV_ANIM_ON);
  lv_arc_set_value(w->arc1, arc1_val);
  lv_arc_set_value(w->arc2, arc2_val);

  lv_chart_set_next_value(w->chart, w->ser, bar_val);
}

int
main(void)
{
  lv_init();
  srand(time(NULL));

  lv_display_t * disp = lv_sdl_window_create(800, 600);
  lv_indev_t * mouse = lv_sdl_mouse_create();

  SDL_SetWindowTitle(lv_sdl_window_get_window(disp), "No Michael Slop");
  LV_UNUSED(mouse);

  // Root container
  lv_obj_t * root = lv_obj_create(lv_screen_active());
  lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_gap(root, 5, 0);
  lv_obj_set_style_bg_color(root, lv_color_hex(0xf5f5f5), 0);
  lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align(root, LV_ALIGN_TOP_MID, 0, 0);

  // Header bar
  lv_obj_t * header = lv_obj_create(root);
  lv_obj_set_size(header, LV_PCT(100), LV_PCT(10));
  lv_obj_set_style_bg_color(header, lv_color_hex(0x2c3e50), 0);
  lv_obj_set_scrollbar_mode(header, LV_SCROLLBAR_MODE_OFF);
  lv_obj_t * header_label = lv_label_create(header);
  lv_label_set_text(header_label, "No Michael Slop Dashboard");
  lv_obj_set_style_text_color(header_label, lv_color_hex(0xffffff), 0);
  lv_obj_center(header_label);

  // Main content
  lv_obj_t * cont = lv_obj_create(root);
  lv_obj_set_size(cont, LV_PCT(100), LV_PCT(85));
  lv_obj_set_layout(cont, LV_LAYOUT_GRID);
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

  // Define grid: 2 columns, 2 rows
  static int col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
  static int row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

  // --- Performance Card ---
  lv_obj_t * perf_card = lv_obj_create(cont);
  lv_obj_set_grid_cell(perf_card, LV_GRID_ALIGN_STRETCH, 0, 1,
      LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_size(perf_card, LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(perf_card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_gap(perf_card, 5, 0);
  lv_obj_set_style_bg_color(perf_card, lv_color_hex(0xffffff), 0);
  lv_obj_set_style_radius(perf_card, 8, 0);
  lv_obj_set_style_shadow_width(perf_card, 8, 0);
  lv_obj_set_scrollbar_mode(perf_card, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t * perf_label = lv_label_create(perf_card);
  lv_obj_set_size(perf_label, LV_PCT(100), LV_PCT(10));
  lv_label_set_text(perf_label, "Performance");
  lv_obj_set_style_text_color(perf_label, lv_color_hex(0x2c3e50), 0);

  lv_obj_t * bar = lv_bar_create(perf_card);
  lv_obj_set_size(bar, LV_PCT(100), LV_PCT(10));

  lv_obj_t * chart = lv_chart_create(perf_card);
  lv_obj_set_size(chart, LV_PCT(100), LV_PCT(80));
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_series_t * ser = lv_chart_add_series(chart, lv_color_hex(0x3498db), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_set_point_count(chart, 20);

  // --- Status Card ---
  lv_obj_t * status_card = lv_obj_create(cont);
  lv_obj_set_grid_cell(status_card, LV_GRID_ALIGN_STRETCH, 1, 1,
      LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_size(status_card, LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(status_card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_gap(status_card, 5, 0);
  lv_obj_set_style_bg_color(status_card, lv_color_hex(0xffffff), 0);
  lv_obj_set_style_radius(status_card, 8, 0);
  lv_obj_set_style_shadow_width(status_card, 8, 0);
  lv_obj_set_scrollbar_mode(status_card, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t * status_label = lv_label_create(status_card);
  lv_label_set_text(status_label, "Status");
  lv_obj_set_size(status_label, LV_PCT(100), LV_PCT(10));
  lv_obj_set_style_text_color(status_label, lv_color_hex(0x2c3e50), 0);

  lv_obj_t * arc_cont = lv_obj_create(status_card);
  lv_obj_set_size(arc_cont, LV_PCT(100), LV_PCT(90));
  lv_obj_set_flex_flow(arc_cont, LV_FLEX_FLOW_ROW);
  //lv_obj_set_style_pad_gap(arc_cont, 30, 0);
  lv_obj_set_style_bg_opa(arc_cont, LV_OPA_TRANSP, 0);
  lv_obj_set_scrollbar_mode(arc_cont, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t * arc1 = lv_arc_create(arc_cont);
  lv_obj_set_size(arc1, LV_PCT(50), LV_PCT(100));
  lv_arc_set_rotation(arc1, 270);
  lv_arc_set_bg_angles(arc1, 0, 360);

  lv_obj_t * arc2 = lv_arc_create(arc_cont);
  lv_obj_set_size(arc2, LV_PCT(50), LV_PCT(100));
  lv_arc_set_rotation(arc2, 270);
  lv_arc_set_bg_angles(arc2, 0, 360);

  // --- Controls Card ---
  lv_obj_t * ctrl_card = lv_obj_create(cont);
  lv_obj_set_grid_cell(ctrl_card, LV_GRID_ALIGN_STRETCH, 0, 1,
      LV_GRID_ALIGN_STRETCH, 1, 1);
  lv_obj_set_size(ctrl_card, LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(ctrl_card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_gap(ctrl_card, 10, 0);
  lv_obj_set_style_bg_color(ctrl_card, lv_color_hex(0xffffff), 0);
  lv_obj_set_style_radius(ctrl_card, 8, 0);
  lv_obj_set_style_shadow_width(ctrl_card, 8, 0);
  lv_obj_set_scrollbar_mode(ctrl_card, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t * ctrl_label = lv_label_create(ctrl_card);
  lv_label_set_text(ctrl_label, "Controls");
  lv_obj_set_style_text_color(ctrl_label, lv_color_hex(0x2c3e50), 0);

  lv_obj_t * btn = lv_button_create(ctrl_card);
  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click Me");

  lv_obj_t * slider = lv_slider_create(ctrl_card);
  lv_obj_set_width(slider, 300);

  lv_obj_t * sw = lv_switch_create(ctrl_card);
  lv_obj_t * dd = lv_dropdown_create(ctrl_card);
  lv_dropdown_set_options(dd, "Option 1\nOption 2\nOption 3");

  LV_UNUSED(sw);

  // Footer bar
  lv_obj_t * footer = lv_obj_create(root);
  lv_obj_set_size(footer, LV_PCT(100), LV_PCT(5));
  lv_obj_set_style_bg_color(footer, lv_color_hex(0x2c3e50), 0);
  lv_obj_set_scrollbar_mode(footer, LV_SCROLLBAR_MODE_OFF);
  lv_obj_t * footer_label = lv_label_create(footer);
  lv_label_set_text(footer_label, "© 2026 No Michael Slop Inc.");
  lv_obj_set_style_text_color(footer_label, lv_color_hex(0xffffff), 0);
  lv_obj_center(footer_label);

  // Simulation setup
  sim_widgets_t * w = malloc(sizeof(sim_widgets_t));
  w->bar = bar;
  w->arc1 = arc1;
  w->arc2 = arc2;
  w->chart = chart;
  w->ser = ser;
  w->t = 0;

  lv_timer_create(sim_timer_cb, 1000, w);

  while(1)
  {
    lv_timer_handler();
    usleep(5000);
  }
  return 0;
}
