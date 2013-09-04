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

#include "osd_model.h"
#include "events.h"

void osd_model_channellist_set(model_channellist_t *model, int index, int id, int lcn, char *name, int selected)
{
  if (selected) {
    model->selectedIndex = index;
  }
  
  model->channel[index].id = id;
  model->channel[index].lcn = lcn; 
  snprintf(model->channel[index].name, sizeof(model->channel[index].name), "%s", name); 
}

void osd_model_channellist_clear(model_channellist_t *model) 
{
  int i;
  memset(model, 0, sizeof(model_channellist_t));
  
  for (i = 0; i < CHANNELLIST_NUM_CHANNELS; i++) {
    model->channel[i].id = -1;
  }  
}

void osd_model_channellist_copy(model_channellist_t *toModel, const model_channellist_t *fromModel)
{
  memcpy(toModel, fromModel, sizeof(model_channellist_t));
}

void setSelectedIndex(model_channellist_t *model, int index) {
  model->selectedIndex = index;
}

int osd_model_channellist_compare(model_channellist_t *newModel, model_channellist_t *oldModel, int index)
{
  if ((newModel->selectedIndex == index || oldModel->selectedIndex == index) ||
      newModel->channel[index].id != oldModel->channel[index].id ||
      (oldModel->channel[index].id == -1 && newModel->channel[index].id != -1) ||
      (oldModel->selectedIndex == index && oldModel->active != newModel->active )) {
          return 1;
  }
  return 0;
}

void osd_model_nownext_clear(model_now_next_t *model) 
{
  memset(model, 0, sizeof(model_now_next_t));
  model->selectedIndex = -1;
}

void osd_model_nownext_set(model_now_next_t *model, uint32_t *eventids, int server)
{ 
  model->selectedIndex = 0;
  model->server = server;
  memcpy(model->event, eventids, sizeof(model->event));
}

void osd_model_nownext_copy(model_now_next_t *toModel, const model_now_next_t *fromModel)
{
  memcpy(toModel, fromModel, sizeof(model_now_next_t));
}


