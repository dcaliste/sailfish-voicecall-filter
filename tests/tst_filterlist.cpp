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

#include <QTest>
#include <QtGlobal>
#include <QSignalSpy>
#include <QDir>

#include <filter.h>
#include <ignoredlist.h>
#include <blockedlist.h>

using namespace VoiceCallFilter;

class TestFilterList: public QObject
{
    Q_OBJECT

public:
    explicit TestFilterList(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void empty(void);
    void ignored(void);
    void blocked(void);

private:
    void emptyList(FilterList *list);
};

TestFilterList::TestFilterList(QObject *parent)
    : QObject(parent)
{
}

void TestFilterList::initTestCase()
{
}

void TestFilterList::cleanupTestCase()
{
}

void TestFilterList::init()
{
    qputenv("DCONF_PROFILE", QDir::currentPath().toUtf8() + "/dconf.prof");

    IgnoredList ignored;
    emptyList(&ignored);
    BlockedList blocked;
    emptyList(&blocked);
}

void TestFilterList::cleanup()
{
}

void TestFilterList::emptyList(FilterList *list)
{
    if (!list->list().isEmpty()) {
        FilterList check(list->key());
        QSignalSpy changed(&check, &FilterList::changed);
        list->set(QStringList());
        if (!changed.count())
            QVERIFY(changed.wait());
    }
}

void TestFilterList::empty(void)
{
    Filter filter;

    QVERIFY(filter.ignoredList().isEmpty());
    QVERIFY(filter.blockedList().isEmpty());

    QCOMPARE(filter.evaluate(QString::fromLatin1("0123456789")), Continue);
    QCOMPARE(filter.evaluate(QString::fromLatin1("+33123456789")), Continue);
}

void TestFilterList::ignored(void)
{
    Filter filter;

    IgnoredList ignored;
    const QStringList numbers(QStringList()
                              << QString::fromLatin1("0123456789")
                              << QString::fromLatin1("+33123456789"));

    QVERIFY(filter.ignoredList().isEmpty());
    QVERIFY(filter.blockedList().isEmpty());

    QSignalSpy changed(&filter, &Filter::ignoredListChanged);
    ignored.set(numbers);
    if (!changed.count())
        QVERIFY(changed.wait());
    QCOMPARE(changed.count(), 1);
    QVERIFY(!filter.ignoredList().isEmpty());
    QVERIFY(filter.blockedList().isEmpty());

    QCOMPARE(filter.evaluate(QString::fromLatin1("0123456789")), Ignore);
    QCOMPARE(filter.evaluate(QString::fromLatin1("+33123456789")), Ignore);
    QCOMPARE(filter.evaluate(QString::fromLatin1("0321654987")), Continue);
}

void TestFilterList::blocked(void)
{
    Filter filter;

    BlockedList blocked;
    const QStringList numbers(QStringList()
                              << QString::fromLatin1("0123456789")
                              << QString::fromLatin1("+33123456789"));

    QVERIFY(filter.ignoredList().isEmpty());
    QVERIFY(filter.blockedList().isEmpty());

    QSignalSpy changed(&filter, &Filter::blockedListChanged);
    blocked.set(numbers);
    if (!changed.count())
        QVERIFY(changed.wait());
    QCOMPARE(changed.count(), 1);
    QVERIFY(filter.ignoredList().isEmpty());
    QVERIFY(!filter.blockedList().isEmpty());

    QCOMPARE(filter.evaluate(QString::fromLatin1("0123456789")), Block);
    QCOMPARE(filter.evaluate(QString::fromLatin1("+33123456789")), Block);
    QCOMPARE(filter.evaluate(QString::fromLatin1("0321654987")), Continue);
}

#include "tst_filterlist.moc"

QTEST_GUILESS_MAIN(TestFilterList)
