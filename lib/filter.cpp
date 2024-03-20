/*
 *
 *  voice call filter library
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

#include "filter.h"
#include "ignoredlist.h"
#include "blockedlist.h"

using namespace VoiceCallFilter;

class Filter::Private
{
public:
    Private()
    {
    }
    ~Private()
    {
    }

    IgnoredList m_ignored;
    BlockedList m_blocked;
};

Filter::Filter(QObject *parent)
    : d(new Private)
{
    connect(&d->m_ignored, &FilterList::changed,
            this, &Filter::ignoredListChanged);
    connect(&d->m_blocked, &FilterList::changed,
            this, &Filter::blockedListChanged);
}

Filter::~Filter()
{
}

QStringList Filter::ignoredList() const
{
    return d->m_ignored.list();
}

QStringList Filter::blockedList() const
{
    return d->m_blocked.list();
}

Action Filter::evaluate(const QString &incomingNumber) const
{
    if (d->m_ignored.match(incomingNumber)) {
        return Ignore;
    } else if (d->m_blocked.match(incomingNumber)) {
        return Block;
    } else {
        return Continue;
    }
}
