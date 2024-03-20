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

#include "filterservice.h"

struct _cb {
    ofono_voicecall_filter_incoming_cb_t cb;
    void *data;
};

struct _FilterService* service_new(void)
{
    struct _FilterService *service = g_malloc(sizeof(struct _FilterService));
    service->id = 1;
    service->requests = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                                              NULL, g_free);
    return service;
}

void service_free(struct _FilterService *service)
{
    if (service) {
        g_hash_table_destroy(service->requests);
    }
    g_free(service);
}

void service_cancel(struct _FilterService *service, guint id)
{
    if (service) {
        g_hash_table_remove(service->requests, GUINT_TO_POINTER(id));
    }
}

gboolean service_filter(struct _FilterService *service, guint id, guint action)
{
    gpointer data;
    if (service &&
        g_hash_table_steal_extended(service->requests, GUINT_TO_POINTER(id),
                                    NULL, &data)) {
        struct _cb *cb = (struct _cb*)data;
        switch (action) {
        case (SERVICE_IGNORE):
            cb->cb(OFONO_VOICECALL_FILTER_INCOMING_IGNORE, cb->data);
            break;
        case (SERVICE_BLOCK):
            cb->cb(OFONO_VOICECALL_FILTER_INCOMING_HANGUP, cb->data);
            break;
        default:
            cb->cb(OFONO_VOICECALL_FILTER_INCOMING_CONTINUE, cb->data);
            break;
        }
        g_free(data);
        return TRUE;
    } else {
        return FALSE;
    }
}

guint service_request(struct _FilterService *service,
                      ofono_voicecall_filter_incoming_cb_t cb, void *data)
{
    if (service && cb) {
        struct _cb *req = g_malloc(sizeof(struct _cb));
        req->cb = cb;
        req->data = data;
        g_hash_table_insert(service->requests, GUINT_TO_POINTER(service->id), req);
        service->id += 1;
        return service->id - 1;
    } else {
        return 0;
    }
}
