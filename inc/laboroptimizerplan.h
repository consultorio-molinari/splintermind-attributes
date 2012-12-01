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
#ifndef LABOROPTIMIZERPLAN_H
#define LABOROPTIMIZERPLAN_H

#include <QtGui>
#include "role.h"

class laborOptimizerPlan : public QObject {
    Q_OBJECT
public:
    laborOptimizerPlan();
    laborOptimizerPlan(QSettings &s, QObject *parent = 0);
    laborOptimizerPlan(const laborOptimizerPlan &lop);

    QString name;
    bool exclude_nobles;
    bool exclude_military;
    bool exclude_injured;
    int max_jobs_per_dwarf;
    int pop_percent; //the percent of the total target population to be optimized
    bool auto_haulers; //auto-assign remaining dwarfs as haulers
    float hauler_percent;

    struct detail{
        int labor_id;
        QString role_name;
        float priority;
        float ratio; //ratio compared to other jobs
        bool use_skill; //set if a role isn't specified
        int max_count; //derived from the max_laborers * target population
        int assigned_laborers; //used when applying optimization
        float group_ratio;
    };

    QVector<detail*> plan_details;

    detail* job_exists(int labor_id);
    void remove_job(int labor_id);

    void write_to_ini(QSettings &s);

protected:
    void read_details(QSettings &s);
};

#endif // LABOROPTIMIZERPLAN_H
