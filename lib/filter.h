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

#ifndef FILTER_H
#define FILTER_H

#include <QSharedPointer>
#include <QObject>

#include "filteraction.h"

namespace VoiceCallFilter {
    class Filter : public QObject
    {
        Q_OBJECT
    public:
        Filter(QObject *parent = nullptr);
        ~Filter();

        QStringList ignoredList() const;
        QStringList blockedList() const;

        Action evaluate(const QString &incomingNumber) const;

    signals:
        void ignoredListChanged();
        void blockedListChanged();

    private:
        class Private;
        QSharedPointer<Private> d;
    };
}

#endif
