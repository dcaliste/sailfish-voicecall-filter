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

#include <CommHistory/event.h>
#include <CommHistory/eventmodel.h>
#include <CommHistory/commonutils.h>

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

void FilterHandler::onFilterRequest(uint id, const QDBusObjectPath &modemPath,
                                    const QString &incomingNumber)
{
    VoiceCallFilter::Action action = m_filter->evaluate(incomingNumber);
    qDebug() << "filter request" << id << modemPath.path() << incomingNumber << action;

    // We need to answer as soon as possible.
    m_interface->filter(id, action);

    if (action != VoiceCallFilter::Continue) {
        CommHistory::Event event;
        event.setType(CommHistory::Event::CallEvent);
        event.setStartTime(QDateTime::currentDateTime());
        event.setEndTime(event.startTime());
        event.setDirection(CommHistory::Event::Inbound);
        event.setLocalUid(CommHistory::RING_ACCOUNT + modemPath.path());
        event.setRecipients(CommHistory::Recipient(event.localUid(), incomingNumber));
        switch (action) {
        case VoiceCallFilter::Ignore:
            event.setFilterType(CommHistory::Event::Ignored);
            break;
        case VoiceCallFilter::Block:
            event.setFilterType(CommHistory::Event::Blocked);
            break;
        default:
            break;
        }

        CommHistory::EventModel model;
        if (!model.addEvent(event)) {
            qWarning() << "Failed to save filtered call event." << event.toString();
        }
    }
}
