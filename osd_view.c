/*

pidvbip - tvheadend client for the Raspberry Pi

(C) Dave Chapman 2012-2013
(C) Daniel Nordqvist 2013

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "osd_view.h"

static uint32_t channellist_win_x; 
static uint32_t channellist_win_y;
static uint32_t channellist_win_w;
static uint32_t channellist_win_h;

static uint32_t nownext_win_x;
static uint32_t nownext_win_y;
static uint32_t nownext_win_w;
static uint32_t nownext_win_h;

static uint32_t eventinfo_win_x;
static uint32_t eventinfo_win_y;
static uint32_t eventinfo_win_w;
static uint32_t eventinfo_win_h;

char* day_str[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

static void osd_channellist_channels(struct osd_t* osd)
{
  int i;
  uint32_t row_space_y = 50;
  uint32_t x = channellist_win_x + PADDING_X;
  uint32_t y = channellist_win_y + PADDING_Y;
  uint32_t color;
  uint32_t bg_color;
  char str[128];
  
  for (i = 0; i < osd->model_channellist.numUsed; i++) {
    if ( osd_model_channellist_compare(&osd->model_channellist, &osd->model_channellist_current, i) == 1 ) {
      //printf("osd_channellist_channels: Update index %d - lcn %d\n", i, osd->model_channellist.channel[i].lcn);
      if (osd->model_channellist.selectedIndex == i) {
        color = COLOR_SELECTED_TEXT;
        if (osd->model_channellist.active) {
          bg_color = COLOR_SELECTED_BACKGROUND;
        }
        else {
          bg_color = COLOR_BACKGROUND;
        }
      }
      else {
        color = COLOR_TEXT;
        bg_color = COLOR_BACKGROUND;
      }
      snprintf(str, sizeof(str), "%d %s", osd->model_channellist.channel[i].lcn, osd->model_channellist.channel[i].name); 
      osd_text(osd, x, y , 500, row_space_y, color, bg_color, str);                      
    }    
    y += row_space_y;     
  }
}

static void osd_channellist_event_info(struct osd_t* osd)
{
  uint32_t x = eventinfo_win_x + PADDING_X;
  uint32_t y = eventinfo_win_y + PADDING_Y;
  uint32_t w = eventinfo_win_w - 2 * PADDING_X; 
  uint32_t h = eventinfo_win_h - 2 * PADDING_Y;
  struct event_t* event;
  uint32_t event_id;
  
  // clear window
  graphics_resource_fill(osd->img, x - 2, y - 5, w + 5, h + 20, COLOR_BACKGROUND);

  event_id = osd->model_now_next.event[osd->model_now_next.selectedIndex];
  if (event_id > 0) {
    event = event_copy(event_id, osd->model_now_next.server);
    if (event != NULL) {
//      printf("eventinfo: %s\n", event->title);
//      printf("eventinfo: %s\n", event->description);
      osd_text(osd, x, y, w, 50, COLOR_SELECTED_TEXT, COLOR_BACKGROUND, event->title);
      osd_paragraph(osd, event->description, 40, x, y + 50, w, h - 50);
    }
  }   
}

static void osd_channellist_nownext_title(struct osd_t* osd)
{
  uint32_t row_space_y = 50;
  uint32_t x = nownext_win_x + PADDING_X;
  uint32_t y = nownext_win_y + PADDING_Y + row_space_y;
  uint32_t w = nownext_win_w - 2 * PADDING_X; 
  uint32_t h = nownext_win_h - 2 * PADDING_Y;
  uint32_t color;
  uint32_t bg_color = COLOR_BACKGROUND;
  uint32_t date_y = nownext_win_y + PADDING_Y;
  struct tm start_time;
  struct tm stop_time;
  struct event_t* event;
  char str[128];
  int i;
  int update_all;

  update_all = osd->model_now_next.event[0] != osd->model_now_next_current.event[0];
  if (update_all) {
    // clear window
    graphics_resource_fill(osd->img, x - 2, date_y - 5, w + 5, h + 20, COLOR_BACKGROUND);
  }
  
  /* Fixa scroll i nn window. Och endast uppdatera ändringar!! */
  
  for (i = 0; i < 11; i++) {  
    if (osd->model_now_next.event[i] > 0) {
      event = event_copy(osd->model_now_next.event[i], osd->model_now_next.server);
      if (event != NULL) {
        if (osd->model_now_next.selectedIndex == i) {
          color = COLOR_SELECTED_TEXT;
          if (osd->model_channellist.active == 0) {
            bg_color = COLOR_SELECTED_BACKGROUND; 
          }
        }
        else {
          color = COLOR_TEXT;
          bg_color = COLOR_BACKGROUND;
        }  
        
        if(update_all || i == osd->model_now_next.selectedIndex || i == osd->model_now_next_current.selectedIndex) { 
          localtime_r((time_t*)&event->start, &start_time);
          localtime_r((time_t*)&event->stop, &stop_time);
          snprintf(str, sizeof(str),"%02d:%02d - %02d:%02d %s", start_time.tm_hour,start_time.tm_min,stop_time.tm_hour,stop_time.tm_min, event->title);      
          osd_text(osd, x, y, w, row_space_y, color, bg_color, str);
          printf("Now: %s\n", str);
          
          if (i == osd->model_now_next.selectedIndex) {
            graphics_resource_fill(osd->img, x - 2, date_y - 5, w + 5, row_space_y, COLOR_BACKGROUND);
            snprintf(str, sizeof(str),"%s %d", day_str[start_time.tm_wday], start_time.tm_mday);      
            osd_text(osd, x, date_y, w, row_space_y, COLOR_SELECTED_TEXT, COLOR_BACKGROUND, str);            
          }  
        }  
        y += row_space_y;
      }
    }  
  }
}

static void osd_channellist_view(struct osd_t* osd)
{
  if (osd->model_channellist_current.channel[0].id == -1) {
    // not currently displayed, draw everything
    
    // Channellist window
    channellist_win_x = OSD_XMARGIN;
    channellist_win_y = OSD_YMARGIN;    
    channellist_win_w = 500;
    channellist_win_h = 616;
    osd_draw_window(osd, channellist_win_x, channellist_win_y, channellist_win_w, channellist_win_h);

    nownext_win_x = channellist_win_x + channellist_win_w + OSD_XMARGIN;
    nownext_win_y = OSD_YMARGIN;
    nownext_win_w = osd->display_width - nownext_win_x - OSD_XMARGIN;
    nownext_win_h = 616;    
    osd_draw_window(osd, nownext_win_x, nownext_win_y, nownext_win_w, nownext_win_h);
    
    eventinfo_win_x = OSD_XMARGIN;
    eventinfo_win_y = nownext_win_y + nownext_win_h + OSD_YMARGIN;
    eventinfo_win_w = osd->display_width - eventinfo_win_x - OSD_XMARGIN;
    eventinfo_win_h = osd->display_height - OSD_YMARGIN - eventinfo_win_y + 6;
    osd_draw_window(osd, eventinfo_win_x, eventinfo_win_y, eventinfo_win_w, eventinfo_win_h);
  }
  
  osd_channellist_channels(osd);
  osd_channellist_nownext_title(osd);
  osd_channellist_event_info(osd);
}

/*  
 * Display the menu view 
 */
static void osd_menu_view(struct osd_t* osd)
{
  osd_draw_window(osd, OSD_XMARGIN, OSD_YMARGIN, osd->display_width - 2 * OSD_XMARGIN, 300);
  osd_text(osd, OSD_XMARGIN + PADDING_X, OSD_YMARGIN + PADDING_Y, osd->display_width - 2 * OSD_XMARGIN, 300, COLOR_TEXT, COLOR_BACKGROUND, osd->model_menu.info);
  osd_text(osd, OSD_XMARGIN + PADDING_X, OSD_YMARGIN + PADDING_Y + 50, osd->display_width - 2 * OSD_XMARGIN, 300, COLOR_TEXT, COLOR_BACKGROUND, osd->model_menu.bitrate);  
}

/*
 * view dispatcher
 */
void osd_view(struct osd_t* osd, int view)
{
  pthread_mutex_lock(&osd->osd_mutex);
  
  switch (view) {
    case OSD_CHANNELLIST:
      osd_channellist_view(osd);
      break;
    case OSD_MENU:
      osd_menu_view(osd);
      break;
  }
  osd->osd_state = view;
  graphics_update_displayed_resource(osd->img, 0, 0, 0, 0);  
  pthread_mutex_unlock(&osd->osd_mutex);
}
