#include <QtDebug>
/* BEGIN_COMMON_COPYRIGHT_HEADER
*
* TOra - An Oracle Toolkit for DBA's and developers
*
* Shared/mixed copyright is held throughout files in this product
*
* Portions Copyright (C) 2000-2001 Underscore AB
* Portions Copyright (C) 2003-2005 Quest Software, Inc.
* Portions Copyright (C) 2004-2008 Numerous Other Contributors
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation;  only version 2 of
* the License is valid for this program.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*      As a special exception, you have permission to link this program
*      with the Oracle Client libraries and distribute executables, as long
*      as you follow the requirements of the GNU GPL in regard to all of the
*      software in the executable aside from Oracle client libraries.
*
*      Specifically you are not permitted to link this program with the
*      Qt/UNIX, Qt/Windows or Qt Non Commercial products of TrollTech.
*      And you are not permitted to distribute binaries compiled against
*      these libraries.
*
*      You may link this product with any GPL'd Qt library.
*
* All trademarks belong to their respective owners.
*
* END_COMMON_COPYRIGHT_HEADER */

#include "toresult.h"
#include "tobrowserbasewidget.h"


toBrowserBaseWidget::toBrowserBaseWidget(QWidget * parent)
    : QTabWidget(parent),
    m_schema(0),
    m_object(0)
{
    setObjectName("toBrowserBaseWidget");

    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(tabWidget_currentChanged(int)));
}

void toBrowserBaseWidget::addTab(QWidget * page, const QString & label)
{
    QTabWidget::addTab(page, label);
    toResult * r = dynamic_cast<toResult*>(page);
    Q_ASSERT_X(r, "new tab is not toResult child!", "toBrowserBaseWidget::addTab params must contain toResult");
    m_tabs[count()-1] = r;
}

void toBrowserBaseWidget::changeParams(const QString & schema, const QString & object)
{
    if (m_schema != schema || m_object != object)
    {
        m_schema = schema;
        m_object = object;
        updateData(currentIndex());
    }
}

void toBrowserBaseWidget::tabWidget_currentChanged(int ix)
{
    // Re-read data from sql only when there is no cache for given
    // schema/object
    if (m_schema.isEmpty() || m_object.isEmpty())
        return;

    if ((!m_cache.contains(ix))
        || (m_cache[ix].first != m_schema)
        || (m_cache[ix].second != m_object))
    {
        m_cache[ix] = qMakePair(m_schema, m_object);
        updateData(ix);
    }
}

void toBrowserBaseWidget::updateData(int ix)
{
    if (schema().isEmpty() || object().isEmpty())
        return;
    m_tabs[ix]->changeParams(schema(), object());
}