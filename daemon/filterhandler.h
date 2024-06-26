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

#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H

#include <QObject>
#include <QDBusObjectPath>

namespace VoiceCallFilter {
    class Filter;
}
class OrgSailfishosVoicecallfilterInterface;
class FilterHandler : public QObject
{
    Q_OBJECT

public:
    FilterHandler(QObject *parent = nullptr);
    ~FilterHandler();

private slots:
    void onFilterRequest(uint id, const QDBusObjectPath &modemPath,
                         const QString &incomingNumber);

private:
    OrgSailfishosVoicecallfilterInterface *m_interface;
    VoiceCallFilter::Filter *m_filter;
};

#endif
