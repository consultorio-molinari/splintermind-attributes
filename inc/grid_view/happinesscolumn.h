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
#ifndef HAPPINESS_COLUMN_H
#define HAPPINESS_COLUMN_H

#include "viewcolumn.h"
#include "dwarf.h"

class HappinessColumn : public ViewColumn {
public:
	HappinessColumn(QString title, ViewColumnSet *set = 0, QObject *parent = 0);
    HappinessColumn(const HappinessColumn &to_copy); // copy ctor
    HappinessColumn* clone() {return new HappinessColumn(*this);}
	QStandardItem *build_cell(Dwarf *d);
	QStandardItem *build_aggregate(const QString &group_name, const QVector<Dwarf*> &dwarves);

	public slots:
        void read_settings();
		void redraw_cells();
private:
	QMap<Dwarf::DWARF_HAPPINESS, QColor> m_colors;
};

#endif
