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

#include "currentjobcolumn.h"
#include "columntypes.h"
#include "viewcolumnset.h"
#include "dwarfmodel.h"
#include "dwarf.h"
#include "dwarftherapist.h"
#include "defines.h"
#include "dwarfjob.h"
#include "gamedatareader.h"
#include "reaction.h"

CurrentJobColumn::CurrentJobColumn(const QString &title, ViewColumnSet *set,
                       QObject *parent)
    : ViewColumn(title, CT_IDLE, set, parent)
{}

CurrentJobColumn::CurrentJobColumn(const CurrentJobColumn &to_copy)
    : ViewColumn(to_copy)
{}

QStandardItem *CurrentJobColumn::build_cell(Dwarf *d) {
    QStandardItem *item = init_cell(d);
    short job_id = d->current_job_id();
    QString pixmap_name(":img/help.png");
    if (job_id == -1) {
        if(d->is_on_break()){
            pixmap_name = ":status/img/bullet_blue.png"; // break
        }else{
            pixmap_name = ":status/img/bullet_red.png"; // idle
        }
    } else {
        DwarfJob *job = GameDataReader::ptr()->get_job(job_id);
        if (job) {

            DwarfJob::DWARF_JOB_TYPE job_type = job->type;
            if(!job->reactionClass.isEmpty() && !d->current_sub_job_id().isEmpty()) {
                Reaction* reaction = d->get_reaction();
                if(reaction!=0) {
                    job_type = DwarfJob::get_type(reaction->skill());
                }
            }

            TRACE << "Dwarf: " << d->nice_name() << " job -" << job_id << ": (" << job->description << "," << job_type << ")";

            switch (job_type) {
            case DwarfJob::DJT_IDLE:                
                pixmap_name = ":status/img/bullet_red.png";
                break;
            case DwarfJob::DJT_DIG:
                pixmap_name = ":status/img/pickaxe.png";
                break;
            case DwarfJob::DJT_CUT:
                pixmap_name = ":status/img/axe.png";
                break;
            case DwarfJob::DJT_REST:
                pixmap_name = ":status/img/status_sleep.png";
                break;
            case DwarfJob::DJT_DRINK:
                pixmap_name = ":status/img/status_drink.png";
                break;
            case DwarfJob::DJT_FOOD:
                pixmap_name = ":status/img/cheese.png";
                break;
            case DwarfJob::DJT_BUILD:
                pixmap_name = ":status/img/gear.png";
                break;
            case DwarfJob::DJT_HAUL:
                pixmap_name = ":status/img/status_haul.png";
                break;
            case DwarfJob::DJT_FIGHT:
                pixmap_name = ":status/img/status_fight2.png";
                break;
            case DwarfJob::DJT_MOOD:
                pixmap_name = ":img/exclamation.png";
                break;
            case DwarfJob::DJT_FORGE:
                pixmap_name = ":status/img/status_forge.png";
                break;
            case DwarfJob::DJT_MEDICAL:
                pixmap_name = ":status/img/first_aid_kit.png";
                break;
            case DwarfJob::DJT_WAX_WORKING:
                pixmap_name = ":status/img/lump.png";
                break;
            case DwarfJob::DJT_POTTERY:
                pixmap_name = ":status/img/pot.png";
                break;
            case DwarfJob::DJT_PRESSING:
                pixmap_name = ":status/img/cup2.png";
                break;
            case DwarfJob::DJT_SPINNING:
                pixmap_name = ":status/img/spinning.png";
                break;
            case DwarfJob::DJT_BEE_KEEPING:
                pixmap_name = ":status/img/bee_i_guess.png";
                break;
            default:
            case DwarfJob::DJT_DEFAULT:
                pixmap_name = ":status/img/control_play_blue.png";
                break;
            }
        }
    }
    item->setData(QIcon(pixmap_name), Qt::DecorationRole);
    
    item->setData(CT_IDLE, DwarfModel::DR_COL_TYPE);
    item->setData(d->current_job_id(), DwarfModel::DR_SORT_VALUE);
    QString tooltip = QString("<h3>%1</h3>%2 (%3)<h4>%4</h4>")
        .arg(m_title)
        .arg(d->current_job())
        .arg(d->current_job_id())
        .arg(d->nice_name());
    item->setToolTip(tooltip);
    return item;
}

QStandardItem *CurrentJobColumn::build_aggregate(const QString &group_name,
                                           const QVector<Dwarf*> &dwarves) {
    Q_UNUSED(group_name);
    Q_UNUSED(dwarves);
    QStandardItem *item = new QStandardItem;
    item->setData(m_bg_color, DwarfModel::DR_DEFAULT_BG_COLOR);
    return item;
}
