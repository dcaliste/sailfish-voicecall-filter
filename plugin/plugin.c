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

#include <ofono/plugin.h>
#include <ofono/voicecall-filter.h>
#include <ofono/dbus.h>
#include <ofono/gdbus.h>
#include <ofono/log.h>
#include <ofono/misc.h>
#include <ofono/modem.h>

#include "filterservice.h"

static struct _FilterService *_service = NULL;

static void _cancel(unsigned int id)
{
    DBG("voicecall-filter: cancel a filtering request (%d).", id);
    if (_service) {
        service_cancel(_service, id);
    }
}

static gboolean _timeout(gpointer data)
{
    if (_service && service_filter(_service, GPOINTER_TO_UINT(data), SERVICE_CONTINUE)) {
        ofono_warn("timeout for filtering request (%d).",
                   GPOINTER_TO_UINT(data));
    }
    return G_SOURCE_REMOVE;
}

static unsigned int _incoming(struct ofono_voicecall *vc,
                              const struct ofono_call *call,
                              ofono_voicecall_filter_incoming_cb_t cb,
                              void *data)
{
    unsigned int id;
    static char buffer[OFONO_PHONE_NUMBER_BUFFER_SIZE];
    ofono_phone_number_to_string(&call->phone_number, buffer);

    DBG("voicecall-filter: getting an incoming call.");
    id = service_request(_service, cb, data);
    if (id) {
        dbus_uint32_t dbusId = id;
	const char *path = ofono_modem_get_path(ofono_voicecall_get_modem(vc));
        const char *number = buffer;
        DBG("voicecall-filter: emitting a new request (%d).", id);
        if (g_dbus_emit_signal(ofono_dbus_get_connection(),
                               "/voicecallfilter",
                               "org.sailfishos.voicecallfilter",
                               "requestFiltering",
                               DBUS_TYPE_UINT32, &dbusId,
                               DBUS_TYPE_OBJECT_PATH, &path,
                               DBUS_TYPE_STRING, &number,
                               DBUS_TYPE_INVALID)) {
            g_timeout_add(1500, _timeout, GUINT_TO_POINTER(id));
        } else {
            ofono_error("Could not emit filter request over D-Bus.");
            service_cancel(_service, id);
            id = 0;
        }
    }
    if (!id) {
        cb(OFONO_VOICECALL_FILTER_INCOMING_CONTINUE, data);
    }
    return id;
}

static DBusMessage* _callFfilter(DBusConnection *conn, DBusMessage *msg, void *data)
{
    guint id, action;
    DBG("voicecall-filter: getting a D-Bus filter call.");
    if (dbus_message_get_args(msg, NULL,
                              DBUS_TYPE_UINT32, &id,
                              DBUS_TYPE_UINT32, &action,
                              DBUS_TYPE_INVALID) == FALSE) {
        return ofono_dbus_error_invalid_args(msg);
    }
    if (_service && !service_filter(_service, id, action)) {
        ofono_warn("Unknown filtering request id: %d", id);
    }
    return NULL;
}

static const GDBusMethodTable _methods[] = {
	{ GDBUS_NOREPLY_METHOD("filter",
                               GDBUS_ARGS({ "id", "u" }, { "action", "u" }), NULL,
                               _callFfilter) },
	{ }
};

static const GDBusSignalTable _signals[] = {
	{ GDBUS_SIGNAL("requestFiltering",
			GDBUS_ARGS({ "id", "u" },
                                   { "modemPath", "o" },
                                   { "incomingNumber", "s" })) },
	{ }
};

static struct ofono_voicecall_filter _filterPlugin;

static int _init_voicecall_filter(void)
{
    DBG("Initialising voicecall filter plugin.");

    _filterPlugin.name = "number-based-filter";
    _filterPlugin.api_version = 0;
    _filterPlugin.priority = OFONO_VOICECALL_FILTER_PRIORITY_DEFAULT;
    _filterPlugin.filter_cancel = _cancel;
    _filterPlugin.filter_dial = NULL;
    _filterPlugin.filter_incoming = _incoming;

    _service = service_new();
    if (!g_dbus_register_interface(ofono_dbus_get_connection(),
                                   "/voicecallfilter",
                                   "org.sailfishos.voicecallfilter",
                                   _methods, _signals,
                                   NULL, NULL, NULL)) {
        ofono_error("Could not register VoiceCallFilter /voicecallfilter");
        return 0;
    }

    return ofono_voicecall_filter_register(&_filterPlugin);
}

static void _exit_voicecall_filter(void)
{
    DBG("De-initialising voicecall filter plugin.");

    ofono_voicecall_filter_unregister(&_filterPlugin);

    g_dbus_unregister_interface(ofono_dbus_get_connection(), "/voicecallfilter",
                                "org.sailfishos.voicecallfilter");
    service_free(_service);
    _service = NULL;
}

OFONO_PLUGIN_DEFINE("voicecall-filter", "block incoming calls based on numbers",
                    VERSION, OFONO_PLUGIN_PRIORITY_DEFAULT,
                    _init_voicecall_filter, _exit_voicecall_filter)
