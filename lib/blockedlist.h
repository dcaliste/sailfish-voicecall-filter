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

#ifndef BLOCKEDLIST_H
#define BLOCKEDLIST_H

#include "filterlist.h"

class QObject;
namespace VoiceCallFilter {
    class BlockedList : public FilterList
    {
    public:
        BlockedList(QObject *parent = nullptr);
        ~BlockedList();
    };
}

#endif
