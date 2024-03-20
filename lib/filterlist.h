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

#ifndef MATCHER_H
#define MATCHER_H

#include <QSharedPointer>
#include <QObject>

namespace VoiceCallFilter {
    class FilterList : public QObject
    {
        Q_OBJECT
    public:
        FilterList(const QString &key, QObject *parent = nullptr);
        ~FilterList();

        bool match(const QString &number);
        QString key() const;
        QStringList list() const;
        void set(const QStringList &list);

    signals:
        void changed();

    private:
        class Private;
        QSharedPointer<Private> d;
    };
}

#endif
