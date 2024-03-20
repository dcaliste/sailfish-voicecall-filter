/*
 *
 *  Sailfish voice call filter daemon
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

#include "filterhandler.h"

#include <QDBusConnection>

#include <filterservice_proxy.h>
#include <filter.h>

FilterHandler::FilterHandler(QObject *parent)
    : QObject(parent)
    , m_interface(new OrgSailfishosVoicecallfilterInterface
                  (QString::fromLatin1("org.ofono"),
                   QString::fromLatin1("/voicecallfilter"),
                   QDBusConnection::systemBus(),
                   this))
    , m_filter(new VoiceCallFilter::Filter(this))
{
    connect(m_interface, &OrgSailfishosVoicecallfilterInterface::requestFiltering,
            this, &FilterHandler::onFilterRequest);
}

FilterHandler::~FilterHandler()
{
}

void FilterHandler::onFilterRequest(uint id, const QString &incomingNumber)
{
    qDebug() << "filter request" << id << incomingNumber;
    m_interface->filter(id, m_filter->evaluate(incomingNumber));
}
