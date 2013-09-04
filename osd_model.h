#ifndef _OSD_MODEL_H
#define _OSD_MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHANNELLIST_NUM_CHANNELS 12
#define CHANNELLIST_NUM_EVENTS 11

typedef struct {
  int id;
  char info[30];
  char bitrate[30];
} model_menu_t;

typedef struct {
  int selectedIndex;
  uint32_t event[CHANNELLIST_NUM_EVENTS];
  int server;
} model_now_next_t;

typedef struct {
  int id;
  int lcn;
  char name[30];
  int server;
} model_channels_t;

typedef struct {
  int active;
  int selectedIndex;
  int numUsed;
  model_channels_t channel[CHANNELLIST_NUM_CHANNELS];
} model_channellist_t;

void osd_model_channellist_clear(model_channellist_t*);
void osd_model_channellist_set(model_channellist_t *model, int index, int id, int lcn, char *name, int selected);
void osd_model_channellist_copy(model_channellist_t*, const model_channellist_t*);
int osd_model_channellist_compare(model_channellist_t*, model_channellist_t*, int);
void osd_model_nownext_set(model_now_next_t* model, uint32_t* eventids, int server);
void osd_model_nownext_copy(model_now_next_t *toModel, const model_now_next_t *fromModel);
void osd_model_nownext_clear(model_now_next_t *model);

#endif

