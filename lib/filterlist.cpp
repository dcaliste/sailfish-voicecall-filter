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

#include "filterlist.h"

#include <MGConfItem>

using namespace VoiceCallFilter;

class FilterList::Private
{
public:
    Private(const QString &key)
        : m_conf(key)
    {
    }
    ~Private()
    {
    }

    MGConfItem m_conf;
};

FilterList::FilterList(const QString &key, QObject *parent)
    : QObject(parent)
    , d(new Private(key))
{
    connect(&d->m_conf, &MGConfItem::valueChanged,
            this, &FilterList::changed);
}

FilterList::~FilterList()
{
}

QString FilterList::key() const
{
    return d->m_conf.key();
}

QStringList FilterList::list() const
{
    return d->m_conf.value().toStringList();
}

void FilterList::set(const QStringList &list)
{
    d->m_conf.set(list);
}

bool FilterList::match(const QString &number)
{
    return list().contains(number);
}
