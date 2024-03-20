/*
 *
 *  oFono voice call filter plugin
 *
 *  Copyright (C) 2024  Damien Caliste <dcaliste@free.fr>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef FILTERSERVICE_H
#define FILTERSERVICE_H

#include <ofono/voicecall-filter.h>

#include <glib.h>

enum {
      SERVICE_CONTINUE,
      SERVICE_IGNORE,
      SERVICE_BLOCK
};

struct _FilterService {
    guint id;
    GHashTable *requests;
};

struct _FilterService* service_new(void);
void service_free(struct _FilterService *service);

void service_cancel(struct _FilterService *service, guint id);
gboolean service_filter(struct _FilterService *service, guint id, guint action);
guint service_request(struct _FilterService *service, ofono_voicecall_filter_incoming_cb_t cb, void *data);

#endif
