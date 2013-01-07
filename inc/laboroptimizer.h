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
#ifndef LABOROPTIMIZER_H
#define LABOROPTIMIZER_H

#include <QtGui>

class Dwarf;
class Labor;
class GameDataReader;
class laborOptimizerPlan;
class PlanDetail;

class LaborOptimizer : public QObject {
    Q_OBJECT
public:
    LaborOptimizer(laborOptimizerPlan *plan, QObject *parent=0);
    virtual ~LaborOptimizer();

    void optimize_labors();
    void optimize_labors(QList<Dwarf*> dwarfs);

    void update_population(QList<Dwarf*>);    
    void update_ratios();

    //getters
    const int total_jobs() {return m_total_jobs;}
    const int total_raw_jobs() {return m_raw_total_jobs;}
    const int assigned_jobs() {return m_estimated_assigned_jobs;}
    const int total_population() {return m_total_population;}
    const int targeted_population() {return roundf(m_target_population);}
public slots:
    void calc_population(bool load_labor_map = false);

signals:
    QString optimize_message(QVector<QPair<int, QString> >);

protected:
    GameDataReader *gdr;
    laborOptimizerPlan *plan;
    QList<Dwarf*> m_dwarfs;
    float m_ratio_sum;

    float m_total_jobs;
    int m_raw_total_jobs;
    int m_estimated_assigned_jobs;    
    int m_total_population; //total selected dwarves - excluded dwarves
    float m_target_population; //m_total_population * % population to use

    bool labors_exceed_pop;
    bool check_conflicts;

    struct dwarf_labor_map{
        float rating;
        Dwarf * d;        
        PlanDetail *det;
    };

    struct less_than_key
    {
        bool operator() (dwarf_labor_map dlm1, dwarf_labor_map dlm2)
        {
            return (dlm2.rating < dlm1.rating);
        }
    };

    QVector<dwarf_labor_map> m_labor_map;
    QVector<QPair<int, QString> > m_current_message;

};

#endif // LABOROPTIMIZER_H
