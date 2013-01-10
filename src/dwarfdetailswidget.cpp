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

#include "dwarfdetailswidget.h"
#include "ui_dwarfdetailswidget.h"
#include "dwarftherapist.h"
#include "gamedatareader.h"
#include "dwarf.h"
#include "trait.h"
#include "dwarfstats.h"
#include "utils.h"
#include "dfinstance.h"
#include "fortressentity.h"
#include "caste.h"
#include "skill.h"
#include "attribute.h"
#include "attributelevel.h"
#include "sortabletableitems.h"

DwarfDetailsWidget::DwarfDetailsWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , ui(new Ui::DwarfDetailsWidget)
{
    ui->setupUi(this);
}

void DwarfDetailsWidget::show_dwarf(Dwarf *d) {
    // Draw the name/profession text labels...
    ui->lbl_dwarf_name->setText(QString("<img src='%1'> %2").arg(d->gender_icon_path()).arg(d->nice_name()));
    ui->lbl_dwarf_name->setToolTip(tr("Name: %1").arg(ui->lbl_dwarf_name->text()));

    ui->lbl_age->setText(QString("Age: %1 years").arg(d->get_age()));
    ui->lbl_age->setToolTip(d->get_migration_desc());

    ui->lbl_translated_name->setText(QString("(%1)").arg(d->translated_name()));
    ui->lbl_translated_name->setToolTip(tr("Translated Name: %1").arg(ui->lbl_translated_name->text()));

    ui->lbl_profession->setText(QString("<img src='%1'> %2").arg(d->profession_icon_path()).arg(d->profession()));
    ui->lbl_profession->setToolTip(tr("Profession: %1").arg(ui->lbl_profession->text()));

    if(d->noble_position().isEmpty()){
        ui->lbl_noble_position->setText("");
        ui->lbl_noble_position->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
        ui->lbl_noble->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Ignored);
    }
    else{
        ui->lbl_noble->setText(tr("<b>Noble Position%1</b>").arg(d->noble_position().contains(",") ? "s" : ""));
        ui->lbl_noble_position->setText(d->noble_position());
        ui->lbl_noble_position->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    }
    ui->lbl_noble_position->setToolTip(ui->lbl_noble_position->text());

    if(d->artifact_name().isEmpty()){
        ui->lbl_artifact->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
        ui->lbl_artifact->setText("");
    }else{
        ui->lbl_artifact->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        ui->lbl_artifact->setText(tr("Creator of '%1'").arg(d->artifact_name()));
    }
    ui->lbl_artifact->setToolTip(ui->lbl_artifact->text());

    ui->lbl_current_job->setText(QString("%1").arg(d->current_job()));
    ui->lbl_current_job->setToolTip(tr("Job ID: %1").arg(QString::number(d->current_job_id())));

    GameDataReader *gdr = GameDataReader::ptr();

    Dwarf::DWARF_HAPPINESS happiness = d->get_happiness();
    ui->lbl_happiness->setText(QString("<b>%1</b> (%2)").arg(d->happiness_name(happiness)).arg(d->get_raw_happiness()));
    QColor color = DT->user_settings()->value(
                QString("options/colors/happiness/%1").arg(static_cast<int>(happiness))).value<QColor>();
    QPalette p;
    QColor color2 = p.window().color();
    ui->lbl_happiness->setStyleSheet(QString("background: QLinearGradient(x1:0,y1:0,x2:0.9,y1:0,stop:0 %1, stop:1 %2); color: %3")
                                     .arg(color.name())
                                     .arg(color2.name())
                                     .arg(compliment(color).name())
                                     );

    if(DT->user_settings()->value("options/highlight_nobles",false).toBool() && d->noble_position() != ""){
        color = DT->get_DFInstance()->fortress()->get_noble_color(d->historical_id());
        ui->lbl_noble_position->setStyleSheet(QString("background: QLinearGradient(x1:0,y1:0,x2:0.9,y1:0,stop:0 %1, stop:1 %2); color: %3")
                                     .arg(color.name())
                                         .arg(color2.name())
                                         .arg(compliment(color).name())
                                         );
    }

    //save user changes before cleaning
    if(m_cleanup_list.count() > 0){
        //save splitter settings
        QSplitter* old_splitter = qobject_cast<QSplitter *>(m_cleanup_list[0]);
        m_splitter_sizes = old_splitter->saveState();        
        QTableWidget *temp;
        //save skill sorts
        temp = qobject_cast<QTableWidget *>(m_cleanup_list[1]);
        m_skill_sort_col = temp->horizontalHeader()->sortIndicatorSection();
        m_skill_sort_desc = temp->horizontalHeader()->sortIndicatorOrder();        
        //save attribute sorts
        temp = qobject_cast<QTableWidget *>(m_cleanup_list[2]);
        m_attribute_sort_col = temp->horizontalHeader()->sortIndicatorSection();
        m_attribute_sort_desc = temp->horizontalHeader()->sortIndicatorOrder();
        //save trait sorts
        temp = qobject_cast<QTableWidget *>(m_cleanup_list[3]);
        m_trait_sort_col = temp->horizontalHeader()->sortIndicatorSection();
        m_trait_sort_desc = temp->horizontalHeader()->sortIndicatorOrder();
        //save role sorts
        temp = qobject_cast<QTableWidget *>(m_cleanup_list[4]);
        m_role_sort_col = temp->horizontalHeader()->sortIndicatorSection();
        m_role_sort_desc = temp->horizontalHeader()->sortIndicatorOrder();
        //save preference sorts
        temp = qobject_cast<QTableWidget *>(m_cleanup_list[5]);
        m_pref_sort_col = temp->horizontalHeader()->sortIndicatorSection();
        m_pref_sort_desc = temp->horizontalHeader()->sortIndicatorOrder();
    }else{//defaults
        //splitter
        m_splitter_sizes = DT->user_settings()->value("gui_options/detailPanesSizes").toByteArray();
        //skill sorts
        m_skill_sort_col = 1;
        m_skill_sort_desc = Qt::DescendingOrder;
        //attribute sorts
        m_attribute_sort_col = 0;
        m_attribute_sort_desc = Qt::AscendingOrder;
        //trait sorts
        m_trait_sort_col = 1;
        m_trait_sort_desc = Qt::DescendingOrder;
        //role sorts
        m_role_sort_col = 1;
        m_role_sort_desc = Qt::DescendingOrder;
        //pref sorts
        m_pref_sort_col = 0;
        m_pref_sort_desc = Qt::DescendingOrder;
    }

    for(int i = m_cleanup_list.count()-1; i >=0; i--){
        delete m_cleanup_list[i];
    }
    m_cleanup_list.clear();


    QSplitter *details_splitter = new QSplitter(this);
    details_splitter->setOrientation(Qt::Vertical);
    details_splitter->setOpaqueResize(true);
    details_splitter->setObjectName("details_splitter");
    ui->vbox_main->addWidget(details_splitter);
    m_cleanup_list << details_splitter;


    // SKILLS TABLE
    QVector<Skill*> *skills = d->get_skills();
    QTableWidget *tw = new QTableWidget(0, 4, this);
    details_splitter->addWidget(tw);
    m_cleanup_list << tw;
    tw->setEditTriggers(QTableWidget::NoEditTriggers);
    tw->setGridStyle(Qt::NoPen);
    tw->setAlternatingRowColors(true);
    tw->setHorizontalHeaderLabels(QStringList() << "Skill" << "Level" << "Bonus" << "Progress");
    tw->verticalHeader()->hide();
    tw->horizontalHeader()->setStretchLastSection(true);
    tw->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tw->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    tw->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    tw->setSortingEnabled(false); // no sorting while we're inserting    
    int real_count = 0;
    int raw_bonus_xp = 100;
    int bonus_xp = 0;
    for (int row = 0; row < skills->size(); ++row) {
        Skill *s = skills->at(row);
        if(s->capped_rating() > -1)
        {
            real_count = tw->rowCount();
            tw->insertRow(real_count);
            tw->setRowHeight(real_count, 18);

            QTableWidgetItem *item_skill = new QTableWidgetItem(s->name());
            if(s->id()==d->highest_moodable()->id()){
                if(d->had_mood()){
                    item_skill->setBackgroundColor(QColor(153,102,34,255));
                    item_skill->setToolTip(tr("Has already had a mood!<br/>"));
                }
                else{
                    item_skill->setBackgroundColor(QColor(220, 220, 255, 255));
                    item_skill->setToolTip(tr("This is the highest moodable skill."));
                }
            }

            sortableFloatTableWidgetItem *level = new sortableFloatTableWidgetItem();
            level->setText(QString::number(d->skill_rating(s->id())));
            level->setData(Qt::UserRole, (float)d->skill_rating(s->id(),true));
            level->setTextAlignment(Qt::AlignHCenter);
            QString l_tooltip = tr("RAW LEVEL: %1").arg(QString::number(s->raw_rating()));
            //level->setToolTip(tr("RAW LEVEL: %1").arg(QString::number(s->raw_rating())));
//            level->setToolTip(tr("RAW LEVEL: %1%2")
//                              .arg(QString::number(s->raw_rating()))
//                              .arg(s->rust() > 0 ? " <b>" + s->rust_rating() + "</b>" : ""));
            if(s->is_losing_xp() > 0){
                level->setBackgroundColor(s->rust_color());
                level->setForeground(Qt::black);
                l_tooltip += tr("<br/><br/>This skill has begun to lose experience!");
            }
            level->setToolTip(l_tooltip);


            raw_bonus_xp = s->skill_rate();
            bonus_xp = raw_bonus_xp - 100;
            sortableFloatTableWidgetItem *item_bonus = new sortableFloatTableWidgetItem();
            item_bonus->setText(QString::number(bonus_xp,'f',0)+"%");
            item_bonus->setData(Qt::UserRole, bonus_xp);
            item_bonus->setTextAlignment(Qt::AlignHCenter);
            if(bonus_xp != 0)
                item_bonus->setToolTip(tr("Receives %1% <b>%2</b> experience than normal. (RAW: %3%)")
                                       .arg(abs(bonus_xp))
                                       .arg(bonus_xp > 0 ? "more" : "less")
                                       .arg(raw_bonus_xp));
            if(bonus_xp < 0){
                item_bonus->setBackground(QColor(204, 0, 0, 128));
                item_bonus->setForeground(QColor(0, 0, 128, 255));
            }

            QProgressBar *pb = new QProgressBar(tw);
            pb->setRange(s->exp_for_current_level(), s->exp_for_next_level());            
            if(s->is_losing_xp())
                pb->setValue(s->exp_for_next_level());
            else
                pb->setValue(s->actual_exp());
            pb->setDisabled(true);// this is to keep them from animating and looking all goofy
            pb->setToolTip(s->exp_summary());

            tw->setItem(real_count, 0, item_skill);
            tw->setItem(real_count, 1, level);
            tw->setItem(real_count, 2, item_bonus);
            tw->setCellWidget(real_count, 3, pb);
        }
    }    
    tw->setSortingEnabled(true); // no sorting while we're inserting    
    tw->sortItems(m_skill_sort_col, static_cast<Qt::SortOrder>(m_skill_sort_desc));

    if(!DT->get_DFInstance()->show_skill_rates())
        tw->removeColumn(2);


    // ATTRIBUTES TABLE
    QHash<int, short> attributes = d->get_attributes();
    QTableWidget *tw_attributes = new QTableWidget(this);
    details_splitter->addWidget(tw_attributes);
    m_cleanup_list << tw_attributes;
    tw_attributes->setColumnCount(3);
    tw_attributes->setEditTriggers(QTableWidget::NoEditTriggers);
    tw_attributes->setWordWrap(true);
    tw_attributes->setShowGrid(false);
    tw_attributes->setGridStyle(Qt::NoPen);
    tw_attributes->setAlternatingRowColors(true);
    tw_attributes->setHorizontalHeaderLabels(QStringList() << "Attribute" << "Value" << "Message");
    tw_attributes->verticalHeader()->hide();
    tw_attributes->horizontalHeader()->setStretchLastSection(true);
    tw_attributes->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tw_attributes->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    tw_attributes->setSortingEnabled(false);
    for (int row = 0; row < attributes.size(); ++row) {
        Attribute *r = gdr->get_attribute(row);

        tw_attributes->insertRow(0);
        tw_attributes->setRowHeight(0, 18);
        AttributeLevel l = d->get_attribute_rating(row);

        QTableWidgetItem *attribute_name = new QTableWidgetItem(r->name);
        QTableWidgetItem *attribute_rating = new QTableWidgetItem;
        attribute_rating->setTextAlignment(Qt::AlignHCenter);
        attribute_rating->setData(0,d->attribute(row));

        if (l.rating < 0) {
            attribute_rating->setBackground(QColor(204, 0, 0, 128));
            attribute_rating->setForeground(QColor(0, 0, 128, 255));
        }

        QString lvl_msg = l.description;
        QTableWidgetItem *attribute_msg = new QTableWidgetItem(lvl_msg);
        attribute_msg->setToolTip(lvl_msg);
        tw_attributes->setItem(0, 0, attribute_name);
        tw_attributes->setItem(0, 1, attribute_rating);
        tw_attributes->setItem(0, 2, attribute_msg);
    }
    tw_attributes->setSortingEnabled(true);
    tw_attributes->sortItems(m_attribute_sort_col, static_cast<Qt::SortOrder>(m_attribute_sort_desc));


    // TRAITS TABLE
    QHash<int, short> traits = d->traits();
    QTableWidget *tw_traits = new QTableWidget(this);
    details_splitter->addWidget(tw_traits);
    m_cleanup_list << tw_traits;
    tw_traits->setColumnCount(3);
    tw_traits->setEditTriggers(QTableWidget::NoEditTriggers);
    tw_traits->setWordWrap(true);
    tw_traits->setShowGrid(false);
    tw_traits->setGridStyle(Qt::NoPen);
    tw_traits->setAlternatingRowColors(true);
    tw_traits->setHorizontalHeaderLabels(QStringList() << "Trait" << "Raw" << "Message");
    tw_traits->verticalHeader()->hide();
    tw_traits->horizontalHeader()->setStretchLastSection(true);
    tw_traits->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tw_traits->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    tw_traits->setSortingEnabled(false);
    for (int row = 0; row < traits.size(); ++row) {
        short val = traits[row];

        if (d->trait_is_active(row))
        {
            tw_traits->insertRow(0);
            tw_traits->setRowHeight(0, 18);
            Trait *t = gdr->get_trait(row);
            QTableWidgetItem *trait_name = new QTableWidgetItem(t->name);
            QTableWidgetItem *trait_score = new QTableWidgetItem;
            trait_score->setTextAlignment(Qt::AlignHCenter);
            trait_score->setData(0, val);

            int deviation = abs(50 - val);
            if (deviation >= 41) {
                trait_score->setBackground(QColor(0, 0, 128, 255));
                trait_score->setForeground(QColor(255, 255, 255, 255));
            } else if (deviation >= 25) {
                trait_score->setBackground(QColor(220, 220, 255, 255));
                trait_score->setForeground(QColor(0, 0, 128, 255));
            }

            QTableWidgetItem *trait_msg = new QTableWidgetItem();
            QString msg = t->level_message(val);
            QString temp = t->conflicts_messages(val);
            if(!temp.isEmpty())
                msg.append(". " + temp);
            temp = t->special_messages(val);
            if(!temp.isEmpty())
                msg.append(". " + temp);
            trait_msg->setText(msg);

//            if(val < 50)
//                trait_msg->setBackground(Trait::col_bad);
//            else
//                trait_msg->setBackground(Trait::col_good);
            trait_msg->setToolTip(QString("%1<br/>%2<br/>%3")
                                  .arg(t->level_message(val))
                                  .arg(t->conflicts_messages(val))
                                  .arg(t->special_messages(val)));

            tw_traits->setItem(0, 0, trait_name);
            tw_traits->setItem(0, 1, trait_score);
            tw_traits->setItem(0, 2, trait_msg);
        }
    }
    tw_traits->setSortingEnabled(true);
    tw_traits->sortItems(m_trait_sort_col, static_cast<Qt::SortOrder>(m_trait_sort_desc));



    // ROLES TABLE
    QList<QPair<QString, float> > roles = d->sorted_role_ratings();
    QTableWidget *tw_roles = new QTableWidget(this);
    details_splitter->addWidget(tw_roles);
    m_cleanup_list << tw_roles;
    tw_roles->setColumnCount(2);
    tw_roles->setEditTriggers(QTableWidget::NoEditTriggers);
    tw_roles->setWordWrap(true);
    tw_roles->setShowGrid(false);
    tw_roles->setGridStyle(Qt::NoPen);
    tw_roles->setAlternatingRowColors(true);
    tw_roles->setHorizontalHeaderLabels(QStringList() << "Role" << "Rating");
    tw_roles->verticalHeader()->hide();    
    tw_roles->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tw_roles->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    tw_roles->setSortingEnabled(false);
    QString name = "";
    float val = 0.0;
    int max = DT->user_settings()->value("options/role_count_pane",10).toInt();
    if(max > d->sorted_role_ratings().count())
        max = d->sorted_role_ratings().count();
    for(int i = 0; i < max; i++){
        name = roles.at(i).first;
        val = roles.at(i).second;

        tw_roles->insertRow(0);
        tw_roles->setRowHeight(0, 18);

        QTableWidgetItem *role_name = new QTableWidgetItem(name);
        sortableFloatTableWidgetItem *role_rating = new sortableFloatTableWidgetItem();
        role_rating->setText(QString::number(val,'f',2)+"%");
        role_rating->setData(Qt::UserRole,val);
        role_rating->setTextAlignment(Qt::AlignHCenter);

        if (val < 50) {
            role_rating->setBackground(QColor(204, 0, 0, 128));
            role_rating->setForeground(QColor(0, 0, 128, 255));
        }

        tw_roles->setItem(0, 0, role_name);
        tw_roles->setItem(0, 1, role_rating);

        Role *r = gdr->get_role(name);
        if(r)
            role_rating->setToolTip(r->get_role_details());
    }
    tw_roles->setSortingEnabled(true);
    tw_roles->sortItems(m_role_sort_col, static_cast<Qt::SortOrder>(m_role_sort_desc));


    // PREFERENCES TABLE
    QTableWidget *tw_prefs = new QTableWidget(this);
    details_splitter->addWidget(tw_prefs);
    m_cleanup_list << tw_prefs;
    tw_prefs->setColumnCount(2);
    tw_prefs->setEditTriggers(QTableWidget::NoEditTriggers);
    tw_prefs->setWordWrap(true);
    tw_prefs->setShowGrid(false);
    tw_prefs->setGridStyle(Qt::NoPen);
    tw_prefs->setAlternatingRowColors(true);
    tw_prefs->setHorizontalHeaderLabels(QStringList() << "Type" << "Preferences");
    tw_prefs->verticalHeader()->hide();
    tw_prefs->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tw_prefs->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    tw_prefs->setSortingEnabled(false);

    QString prefs;
    foreach(QString name, d->get_grouped_preferences().uniqueKeys()){

        prefs = capitalize(d->get_grouped_preferences().value(name)->join(", "));

        tw_prefs->insertRow(0);
        tw_prefs->setRowHeight(0, 18);

        QTableWidgetItem *pref_type = new QTableWidgetItem(name);
        QTableWidgetItem *pref_values = new QTableWidgetItem(prefs);

        tw_prefs->setItem(0, 0, pref_type);
        tw_prefs->setItem(0, 1, pref_values);

        pref_values->setToolTip(prefs);
    }
    tw_prefs->setSortingEnabled(true);
    tw_prefs->sortItems(m_pref_sort_col, static_cast<Qt::SortOrder>(m_pref_sort_desc));

    //resize based on user settings
    details_splitter->restoreState(m_splitter_sizes);
}


