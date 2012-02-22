/*
Dwarf Therapist
Copyright (c) 2009 Trey Stout (chmod)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "attributecolumn.h"
#include "columntypes.h"
#include "viewcolumnset.h"
#include "gamedatareader.h"
#include "dwarfmodel.h"
#include "truncatingfilelogger.h"

AttributeColumn::AttributeColumn(const QString &title, DWARF_ATTRIBUTE_TYPE type, ViewColumnSet *set, QObject *parent)
    : ViewColumn(title, CT_ATTRIBUTE, set, parent)
    , m_attribute_type(type)
{
    if (title.isEmpty()) { // Determine title based on type if no title was passed in
        switch(type) {
            case DTA_STRENGTH:  m_title = tr("Strength");   break;
            case DTA_AGILITY:   m_title = tr("Agility");    break;
            case DTA_TOUGHNESS: m_title = tr("Toughness");  break;
            case DTA_ENDURANCE: m_title = tr("Endurance");  break;
            case DTA_RECUPERATION: m_title = tr("Recuperation");  break;
            case DTA_DISEASE_RESISTANCE: m_title = tr("Disease resistance");  break;
        }
    }
}

AttributeColumn::AttributeColumn(QSettings &s, ViewColumnSet *set, QObject *parent)
    : ViewColumn(s, set, parent)
    , m_attribute_type(static_cast<DWARF_ATTRIBUTE_TYPE>(s.value("attribute", -1).toInt()))
{}

AttributeColumn::AttributeColumn(const AttributeColumn &to_copy)
    : ViewColumn(to_copy)
    , m_attribute_type(to_copy.m_attribute_type)
{}

QStandardItem *AttributeColumn::build_cell(Dwarf *d) {
    QStandardItem *item = init_cell(d);
    QString key("attributes/%1/level_%2");
    short val = -1;
    short rawVal = 0;
    val = d->get_attribute((int)m_attribute_type);
    QString msg;
    switch (m_attribute_type) {
        case DTA_STRENGTH:
            key = key.arg("Strength").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Strength", val);
            rawVal = (int)d->strength();
            break;
        case DTA_AGILITY:
            key = key.arg("Agility").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Agility", val);
            rawVal = (int)d->agility();
            break;
        case DTA_TOUGHNESS:
            key = key.arg("Toughness").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Toughness", val);
            rawVal = (int)d->toughness();
            break;
        case DTA_ENDURANCE:
            key = key.arg("Endurance").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Endurance", val);
            rawVal = (int)d->endurance();
            break;
        case DTA_RECUPERATION:
            key = key.arg("Recuperation").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Recuperation", val);
            rawVal = (int)d->recuperation();
            break;
        case DTA_DISEASE_RESISTANCE:
            key = key.arg("Disease Resistance").arg(val > 8 ? 8 : val);
            msg = GameDataReader::ptr()->get_attribute_level_name("Disease Resistance", val);
            rawVal = (int)d->disease_resistance();
            break;
        default:
            LOGW << "Attribute column can't build cell since type is set to" << m_attribute_type;
    }

    if (val) {
        //msg = GameDataReader::ptr()->get_string_for_key(key);
        item->setData(val, Qt::DisplayRole);
    }

    item->setData(rawVal, DwarfModel::DR_SORT_VALUE);
    item->setData(val, DwarfModel::DR_RATING);
    item->setData(CT_ATTRIBUTE, DwarfModel::DR_COL_TYPE);

    QString tooltip = QString("<h3>%1</h3>%2 (%3)<h4>%5</h4>")
        .arg(m_title)
        .arg(msg)
        .arg(rawVal)
        .arg(d->nice_name());
    item->setToolTip(tooltip);
    return item;
}

QStandardItem *AttributeColumn::build_aggregate(const QString &group_name, const QVector<Dwarf*> &dwarves) {
    Q_UNUSED(group_name);
    Q_UNUSED(dwarves);
    QStandardItem *item = new QStandardItem;
    item->setData(m_bg_color, DwarfModel::DR_DEFAULT_BG_COLOR);
    return item;
}
