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
#include <QtWidgets>

#include "optionsmenu.h"
#include "customcolor.h"
#include "dwarf.h"
#include "dwarftherapist.h"
#include "utils.h"
#include "defines.h"
#include "truncatingfilelogger.h"
#include "uberdelegate.h"
#include "mainwindow.h"
#include "fortressentity.h"
#include "dfinstance.h"
#include "gamedatareader.h"
#include "defaultfonts.h"

OptionsMenu::OptionsMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionsMenu)
    , m_reading_settings(false)
{
    ui->setupUi(this);

    m_general_colors
            << new CustomColor(tr("Skill"), tr("The color of the growing skill indicator box "
                                               "inside a cell. Is not used when auto-contrast is enabled."), "skill", from_hex("0xAAAAAAFF"), this)
            << new CustomColor(tr("Active Labor Cell"),
                               tr("Color shown for a cell when the labor is active for a dwarf."),
                               "active_labor", from_hex("0x7878B3FF"), this)
            << new CustomColor(tr("Active Group Cell"),
                               tr("Color shown on an aggregate cell if <b>all</b> dwarves have this labor enabled."),
                               "active_group", from_hex("0x33FF33FF"), this)
            << new CustomColor(tr("Inactive Group Cell"),
                               tr("Color shown on an aggregate cell if <b>none</b> of the dwarves have this labor enabled."),
                               "inactive_group", from_hex("0x00000020"), this)
            << new CustomColor(tr("Partial Group Cell"),
                               tr("Color shown on an aggregate cell if <b>some</b> of the dwarves have this labor enabled."),
                               "partial_group", from_hex("0x00000060"), this)
            << new CustomColor(tr("Selection Guides"),
                               tr("Color of the lines around cells when a row and/or column are selected."),
                               "guides", QColor(0x0099FF), this)
            << new CustomColor(tr("Main Border"),
                               tr("Color of cell borders"),
                               "border", QColor(0xd9d9d9), this)
            << new CustomColor(tr("Dirty Cell Indicator"),
                               tr("Border color of a cell that has pending changes. Set to main border color to disable this."),
                               "dirty_border", QColor(0xFF6600), this)
            << new CustomColor(tr("Highest Moodable Skill"),
                               tr("Border color of a labor or skill cell for the highest moodable skill which hasn't been used. This is only applied if the option to highlight mood cells is enabled."),
                               "highest_mood_border", QColor(0x32cd32), this)
            << new CustomColor(tr("Mood Finished"),
                               tr("Border color of a labor or skill cell for the highest moodable skill which has already been finished. This is only applied if the option to highlight mood cells is enabled."),
                               "had_mood_border", QColor(0x696969), this);

    m_happiness_colors
            << new CustomColor(tr("Ecstatic"), tr("Color shown in happiness columns when a dwarf is <b>ecstatic</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_ECSTATIC)), QColor(0x00FF00), this)
            << new CustomColor(tr("Happy"), tr("Color shown in happiness columns when a dwarf is <b>happy</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_HAPPY)), QColor(0x71cc09), this)
            << new CustomColor(tr("Content"), tr("Color shown in happiness columns when a dwarf is <b>quite content</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_CONTENT)), QColor(0xDDDD00), this)
            << new CustomColor(tr("Fine"), tr("Color shown in happiness columns when a dwarf is <b>fine</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_FINE)), QColor(0xe7e2ab), this)
            << new CustomColor(tr("Unhappy"), tr("Color shown in happiness columns when a dwarf is <b>unhappy</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_UNHAPPY)), QColor(0xffaa00), this)
            << new CustomColor(tr("Very Unhappy"), tr("Color shown in happiness columns when a dwarf is <b>very unhappy</b>."),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_VERY_UNHAPPY)), QColor(0xCC0000), this)
            << new CustomColor(tr("Miserable"), tr("Color shown in happiness columns when a dwarf is <b>miserable.</b>"),
                               QString("happiness/%1").arg(static_cast<int>(Dwarf::DH_MISERABLE)), QColor(0xFF0000), this);

    QColor m_noble_default = FortressEntity::default_noble_color;
    m_noble_colors
            << new CustomColor(tr("Bookkeeper"),tr("Highlight color for the bookkeeper."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::BOOKKEEPER)), m_noble_default, this)
            << new CustomColor(tr("Broker"),tr("Highlight color for the broker."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::BROKER)), m_noble_default, this)
            << new CustomColor(tr("Champions"),tr("Highlight color for champions."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::CHAMPION)), m_noble_default, this)
            << new CustomColor(tr("Chief Medical Dwarf"),tr("Highlight color for the chief medical dwarf."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::CHIEF_MEDICAL_DWARF)), m_noble_default, this)
            << new CustomColor(tr("Hammerer"),tr("Highlight color for the hammerer."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::HAMMERER)), m_noble_default, this)
            << new CustomColor(tr("Law"),tr("Highlight color for the captain of the guard and sherrif."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::LAW)), m_noble_default, this)
            << new CustomColor(tr("Leader/Mayor"),tr("Highlight color for the expedition leaders and mayors."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::LEADER)), m_noble_default, this)
            << new CustomColor(tr("Manager"),tr("Highlight color for the managers."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::MANAGER)), m_noble_default, this)
            << new CustomColor(tr("Militia"),tr("Highlight color for the militia commander, militia captains, lieutenants and generals."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::MILITIA)), m_noble_default, this)
            << new CustomColor(tr("Monarch"),tr("Highlight color for kings, queens, emperors and empresses."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::MONARCH)), m_noble_default, this)
            << new CustomColor(tr("Royalty"),tr("Highlight color for barons, baronesses, dukes, duchesses, counts, countesses, lords and ladies."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::ROYALTY)), m_noble_default, this)
            << new CustomColor(tr("Religious"),tr("Highlight color for high priests, priests and druids."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::RELIGIOUS)), m_noble_default, this)
            << new CustomColor(tr("Multiple"),tr("Highlight color when holding multiple positions, or unknown positions."),
                               QString("nobles/%1").arg(static_cast<int>(FortressEntity::MULTIPLE)), m_noble_default, this);

    m_curse_color = new CustomColor(tr("Cursed"),tr("Cursed creatures will be highlighted with this color."),
                                    "cursed",QColor(125,97,186),this);

    QVBoxLayout *grid_layout = new QVBoxLayout;
    grid_layout->addWidget(ui->cb_grid_health_colors);
    foreach(CustomColor *cc, m_general_colors) {
        grid_layout->addWidget(cc);
    }
    grid_layout->setSpacing(0);
    ui->tab_grid_colors->setLayout(grid_layout);

    QVBoxLayout *happiness_layout = new QVBoxLayout;
    happiness_layout->addWidget(ui->cb_happiness_icons);
    foreach(CustomColor *cc, m_happiness_colors) {
        happiness_layout->addWidget(cc);
    }
    happiness_layout->setSpacing(0);
    ui->tab_happiness_colors->setLayout(happiness_layout);

    QVBoxLayout *nobles_layout = new QVBoxLayout;
    nobles_layout->addWidget(ui->cb_noble_highlight);
    foreach(CustomColor *cc, m_noble_colors) {
        nobles_layout->addWidget(cc);
    }
    nobles_layout->setSpacing(2);
    ui->tab_noble_colors->setLayout(nobles_layout);

    ui->horizontal_curse_layout->addWidget(m_curse_color);

    ui->cb_skill_drawing_method->addItem("Growing Center Box", UberDelegate::SDM_GROWING_CENTRAL_BOX);
    ui->cb_skill_drawing_method->addItem("Line Glyphs", UberDelegate::SDM_GLYPH_LINES);
    ui->cb_skill_drawing_method->addItem("Growing Fill", UberDelegate::SDM_GROWING_FILL);
    ui->cb_skill_drawing_method->addItem("Text", UberDelegate::SDM_NUMERIC);

    connect(ui->btn_restore_defaults, SIGNAL(pressed()), this, SLOT(restore_defaults()));

    connect(ui->btn_change_font, SIGNAL(pressed()), this, SLOT(show_row_font_chooser()));
    connect(ui->btn_change_header_font, SIGNAL(pressed()), this, SLOT(show_header_font_chooser()));
    connect(ui->btn_change_tooltip_font, SIGNAL(pressed()), this, SLOT(show_tooltip_font_chooser()));
    connect(ui->btn_change_main_font, SIGNAL(pressed()), this, SLOT(show_main_font_chooser()));

    connect(ui->cb_auto_contrast, SIGNAL(toggled(bool)), m_general_colors.at(0), SLOT(setDisabled(bool)));
    connect(ui->cb_moodable, SIGNAL(toggled(bool)), m_general_colors.at(8), SLOT(setEnabled(bool)));
    connect(ui->cb_moodable, SIGNAL(toggled(bool)), m_general_colors.at(9), SLOT(setEnabled(bool)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_index_changed(int)));

    read_settings();
}

OptionsMenu::~OptionsMenu() {
    qDeleteAll(m_general_colors);
    m_general_colors.clear();
    qDeleteAll(m_noble_colors);
    m_noble_colors.clear();
    qDeleteAll(m_happiness_colors);
    m_happiness_colors.clear();
    m_curse_color = 0;
    delete ui;
}

bool OptionsMenu::event(QEvent *evt) {
    if (evt->type() == QEvent::StatusTip) {
        ui->text_status_tip->setHtml(static_cast<QStatusTipEvent*>(evt)->tip());
        return true; // we've handled it, don't pass it
    }
    return QWidget::event(evt); // pass the event along the chain
}

void OptionsMenu::showEvent(QShowEvent *evt){
    //if we haven't detected multiple castes (mods) skill rate isn't used, so hide the weight setting
    if(!DT->multiple_castes){
        ui->lbl_def_skill_rate_weight->setVisible(false);        
    }
    if(!DT->show_skill_learn_rates){
        ui->dsb_skill_rate_weight->setVisible(false);
    }
    QDialog::showEvent(evt);
}

void OptionsMenu::read_settings() {
    m_reading_settings = true;
    QSettings *s = DT->user_settings();
    s->beginGroup("options");
    s->beginGroup("colors");
    QColor c;
    foreach(CustomColor *cc, m_general_colors) {
        c = s->value(cc->get_config_key(), cc->get_default()).value<QColor>();
        cc->set_color(c);
    }
    foreach(CustomColor *cc, m_happiness_colors) {
        c = s->value(cc->get_config_key(), cc->get_default()).value<QColor>();
        cc->set_color(c);
    }
    foreach(CustomColor *cc, m_noble_colors) {
        c = s->value(cc->get_config_key(), cc->get_default()).value<QColor>();
        cc->set_color(c);
    }
    c = s->value(m_curse_color->get_config_key(), m_curse_color->get_default()).value<QColor>();
    m_curse_color->set_color(c);

    s->endGroup();
    s->beginGroup("grid");
    UberDelegate::SKILL_DRAWING_METHOD m = static_cast<UberDelegate::SKILL_DRAWING_METHOD>(s->value("skill_drawing_method", UberDelegate::SDM_GROWING_CENTRAL_BOX).toInt());
    for(int i=0; i < ui->cb_skill_drawing_method->count(); ++i) {
        if (ui->cb_skill_drawing_method->itemData(i) == m) {
            ui->cb_skill_drawing_method->setCurrentIndex(i);
            break;
        }
    }
    ui->sb_cell_size->setValue(s->value("cell_size", DEFAULT_CELL_SIZE).toInt());
    ui->sb_cell_padding->setValue(s->value("cell_padding", 0).toInt());
    ui->cb_shade_column_headers->setChecked(s->value("shade_column_headers", true).toBool());
    ui->cb_shade_cells->setChecked(s->value("shade_cells", true).toBool());
    ui->cb_header_text_direction->setChecked(s->value("header_text_bottom", false).toBool());

    QFont temp = s->value("font", QFont(DefaultFonts::getRowFontName(), DefaultFonts::getRowFontSize())).value<QFont>();
    m_row_font = qMakePair(temp,temp);
    show_current_font(temp, ui->lbl_current_font);

    temp = s->value("header_font", QFont(DefaultFonts::getHeaderFontName(), DefaultFonts::getHeaderFontSize())).value<QFont>();
    m_col_header_font = qMakePair(temp,temp);
    show_current_font(temp, ui->lbl_header_font);


    ui->cb_happiness_icons->setChecked(s->value("happiness_icons",false).toBool());
    ui->cb_labor_counts->setChecked(s->value("show_labor_counts",false).toBool());

    ui->cb_sync_grouping->setChecked(s->value("group_all_views",true).toBool());
    ui->cb_sync_scrolling->setChecked(s->value("scroll_all_views",false).toBool());

    ui->cb_moodable->setChecked(s->value("color_mood_cells",false).toBool());    
    ui->cb_gender_icons->setChecked(s->value("show_gender_icons",true).toBool());
    ui->cb_grid_health_colors->setChecked(s->value("color_health_cells",true).toBool());
    //the signal to disable the color pickers doesn't fire on the initial read. this is a work around for the inital setting
    if(!ui->cb_moodable->isChecked()){
        m_general_colors.at(8)->setDisabled(true);
        m_general_colors.at(9)->setDisabled(true);
    }
    s->endGroup();        

    temp = s->value("tooltip_font", QFont(DefaultFonts::getTooltipFontName(), DefaultFonts::getTooltipFontSize())).value<QFont>();
    m_tooltip_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_current_tooltip);

    temp = s->value("main_font", QFont(DefaultFonts::getMainFontName(), DefaultFonts::getMainFontSize())).value<QFont>();
    m_main_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_current_main_font);


    ui->cb_read_dwarves_on_startup->setChecked(s->value("read_on_startup", true).toBool());
    ui->cb_auto_contrast->setChecked(s->value("auto_contrast", true).toBool());
    ui->cb_show_aggregates->setChecked(s->value("show_aggregates", true).toBool());
    ui->cb_single_click_labor_changes->setChecked(s->value("single_click_labor_changes", true).toBool());
    ui->cb_show_toolbar_text->setChecked(s->value("show_toolbutton_text", true).toBool());
    ui->cb_auto_expand->setChecked(s->value("auto_expand_groups", true).toBool());
    ui->cb_show_full_dwarf_names->setChecked(s->value("show_full_dwarf_names", false).toBool());        
    ui->cb_check_for_updates_on_startup->setChecked(s->value("check_for_updates_on_startup", true).toBool());
    ui->cb_alert_on_lost_connection->setChecked(s->value("alert_on_lost_connection", true).toBool());
    ui->cb_labor_cheats->setChecked(s->value("allow_labor_cheats", false).toBool());
    ui->cb_hide_children->setChecked(s->value("hide_children_and_babies", false).toBool());
    ui->cb_generic_names->setChecked(s->value("use_generic_names", false).toBool());
    ui->cb_curse_highlight->setChecked(s->value("highlight_cursed", false).toBool());
    ui->cb_noble_highlight->setChecked(s->value("highlight_nobles", false).toBool());
    ui->cb_labor_exclusions->setChecked(s->value("labor_exclusions", true).toBool());    
    ui->cb_no_diagnosis->setChecked(s->value("diagnosis_not_required",false).toBool());
    ui->cb_animal_health->setChecked(s->value("animal_health",false).toBool());

    ui->chk_show_caste->setChecked(s->value("tooltip_show_caste", true).toBool());
    ui->chk_show_caste_desc->setChecked(s->value("tooltip_show_caste_desc", true).toBool());
    ui->chk_show_happiness->setChecked(s->value("tooltip_show_happiness", true).toBool());
    ui->chk_show_icons->setChecked(s->value("tooltip_show_icons", true).toBool());
    ui->chk_show_noble->setChecked(s->value("tooltip_show_noble", true).toBool());
    ui->chk_show_prefs->setChecked(s->value("tooltip_show_preferences", true).toBool());
    ui->chk_show_traits->setChecked(s->value("tooltip_show_traits", true).toBool());
    ui->chk_show_prof->setChecked(s->value("tooltip_show_profession", true).toBool());
    ui->chk_show_artifact->setChecked(s->value("tooltip_show_artifact",true).toBool());
    ui->chk_show_highest_mood->setChecked(s->value("tooltip_show_mood", true).toBool());
    ui->chk_show_thoughts->setChecked(s->value("tooltip_show_thoughts", true).toBool());
    ui->chk_show_squad->setChecked(s->value("tooltip_show_squad", true).toBool());
    ui->chk_show_age->setChecked(s->value("tooltip_show_age", true).toBool());
    ui->chk_show_unit_size->setChecked(s->value("tooltip_show_size",true).toBool());
    ui->chk_show_buffs->setChecked(s->value("tooltip_show_buffs",false).toBool());
    short syn_option = s->value("syndrome_display_type",0).toInt();
    if(syn_option == 0)
        ui->rad_syn_names->setChecked(true);
    else if(syn_option == 1)
        ui->rad_syn_classes->setChecked(true);
    else
        ui->rad_syn_both->setChecked(true);
    ui->chk_show_health->setChecked(s->value("tooltip_show_health",false).toBool());
    ui->chk_health_colors->setChecked(s->value("tooltip_health_colors",true).toBool());
    ui->chk_health_symbols->setChecked(s->value("tooltip_health_symbols",false).toBool());

    ui->chk_show_roles->setChecked(s->value("tooltip_show_roles", true).toBool());
    ui->sb_roles_tooltip->setValue(s->value("role_count_tooltip",3).toInt());

    ui->chk_show_skills->setChecked(s->value("tooltip_show_skills", true).toBool());
    ui->sb_min_skill_level->setValue(s->value("min_tooltip_skill_level",1).toInt());


    ui->dsb_attribute_weight->setValue(s->value("default_attributes_weight",0.25).toDouble());
    ui->dsb_skill_weight->setValue(s->value("default_skills_weight",1.0).toDouble());
    ui->dsb_trait_weight->setValue(s->value("default_traits_weight",0.20).toDouble());
    ui->dsb_pref_weight->setValue(s->value("default_prefs_weight",0.15).toDouble());
    ui->dsb_skill_rate_weight->setValue(s->value("default_skill_rate_weight",0.25).toDouble());
    ui->dsb_att_potential_weight->setValue(s->value("default_attribute_potential_weight",0.50).toDouble());
    ui->sb_roles_pane->setValue(s->value("role_count_pane",10).toInt());

    ui->chk_custom_roles->setChecked(s->value("show_custom_roles",false).toBool());
    ui->chk_roles_in_labor->setChecked(s->value("show_roles_in_labor",true).toBool());
    ui->chk_roles_in_skills->setChecked(s->value("show_roles_in_skills",true).toBool());

    s->endGroup();

    m_reading_settings = false;
}

void OptionsMenu::write_settings() {
    if (!m_reading_settings) {
        QSettings *s = DT->user_settings();
        s->beginGroup("options");

        s->beginGroup("colors");
        foreach(CustomColor *cc, m_general_colors) {
            s->setValue(cc->get_config_key(), cc->get_color());
        }
        foreach(CustomColor *cc, m_happiness_colors) {
            s->setValue(cc->get_config_key(), cc->get_color());
        }
        foreach(CustomColor *cc, m_noble_colors) {
            s->setValue(cc->get_config_key(), cc->get_color());
        }
        s->setValue(m_curse_color->get_config_key(), m_curse_color->get_color());
        s->endGroup();

        s->beginGroup("grid");
        s->setValue("skill_drawing_method", ui->cb_skill_drawing_method->itemData(ui->cb_skill_drawing_method->currentIndex()).toInt());
        s->setValue("cell_size", ui->sb_cell_size->value());
        s->setValue("cell_padding", ui->sb_cell_padding->value());
        s->setValue("shade_column_headers", ui->cb_shade_column_headers->isChecked());
        s->setValue("shade_cells", ui->cb_shade_cells->isChecked());
        s->setValue("header_text_bottom", ui->cb_header_text_direction->isChecked());
        s->setValue("font", m_row_font.first);
        s->setValue("header_font", m_col_header_font.first);
        s->setValue("happiness_icons",ui->cb_happiness_icons->isChecked());
        s->setValue("color_mood_cells", ui->cb_moodable->isChecked());
        s->setValue("show_gender_icons", ui->cb_gender_icons->isChecked());
        s->setValue("color_health_cells", ui->cb_grid_health_colors->isChecked());
        s->setValue("show_labor_counts",ui->cb_labor_counts->isChecked());
        s->setValue("group_all_views",ui->cb_sync_grouping->isChecked());
        s->setValue("scroll_all_views",ui->cb_sync_scrolling->isChecked());
        s->endGroup();

        s->setValue("read_on_startup", ui->cb_read_dwarves_on_startup->isChecked());
        s->setValue("auto_contrast", ui->cb_auto_contrast->isChecked());
        s->setValue("show_aggregates", ui->cb_show_aggregates->isChecked());
        s->setValue("single_click_labor_changes", ui->cb_single_click_labor_changes->isChecked());
        s->setValue("show_toolbutton_text", ui->cb_show_toolbar_text->isChecked());
        s->setValue("auto_expand_groups", ui->cb_auto_expand->isChecked());
        s->setValue("show_full_dwarf_names", ui->cb_show_full_dwarf_names->isChecked());
        s->setValue("min_tooltip_skill_level", ui->sb_min_skill_level->value());
        s->setValue("check_for_updates_on_startup", ui->cb_check_for_updates_on_startup->isChecked());
        s->setValue("alert_on_lost_connection", ui->cb_alert_on_lost_connection->isChecked());
        s->setValue("allow_labor_cheats", ui->cb_labor_cheats->isChecked());
        s->setValue("hide_children_and_babies", ui->cb_hide_children->isChecked());
        s->setValue("use_generic_names", ui->cb_generic_names->isChecked());
        s->setValue("highlight_cursed", ui->cb_curse_highlight->isChecked());
        s->setValue("highlight_nobles", ui->cb_noble_highlight->isChecked());
        s->setValue("labor_exclusions", ui->cb_labor_exclusions->isChecked());
        s->setValue("diagnosis_not_required", ui->cb_no_diagnosis->isChecked());
        s->setValue("animal_health", ui->cb_animal_health->isChecked());
        s->setValue("tooltip_font", m_tooltip_font.first);
        s->setValue("main_font", m_main_font.first);

        s->setValue("default_attributes_weight",ui->dsb_attribute_weight->value());
        s->setValue("default_skills_weight",ui->dsb_skill_weight->value());
        s->setValue("default_traits_weight",ui->dsb_trait_weight->value());
        s->setValue("default_prefs_weight",ui->dsb_pref_weight->value());
        s->setValue("default_skill_rate_weight",ui->dsb_skill_rate_weight->value());
        s->setValue("default_attribute_potential_weight",ui->dsb_att_potential_weight->value());

        s->setValue("role_count_tooltip",ui->sb_roles_tooltip->value());
        s->setValue("role_count_pane",ui->sb_roles_pane->value());

        s->setValue("show_custom_roles",ui->chk_custom_roles->isChecked());
        s->setValue("show_roles_in_labor",ui->chk_roles_in_labor->isChecked());
        s->setValue("show_roles_in_skills",ui->chk_roles_in_skills->isChecked());

        s->setValue("tooltip_show_caste", ui->chk_show_caste->isChecked());
        s->setValue("tooltip_show_caste_desc", ui->chk_show_caste_desc->isChecked());
        s->setValue("tooltip_show_happiness", ui->chk_show_happiness->isChecked());
        s->setValue("tooltip_show_icons", ui->chk_show_icons->isChecked());
        s->setValue("tooltip_show_noble", ui->chk_show_noble->isChecked());
        s->setValue("tooltip_show_preferences", ui->chk_show_prefs->isChecked());
        s->setValue("tooltip_show_traits", ui->chk_show_traits->isChecked());
        s->setValue("tooltip_show_profession", ui->chk_show_prof->isChecked());
        s->setValue("tooltip_show_roles", ui->chk_show_roles->isChecked());
        s->setValue("tooltip_show_skills", ui->chk_show_skills->isChecked());
        s->setValue("tooltip_show_artifact", ui->chk_show_artifact->isChecked());
        s->setValue("tooltip_show_mood", ui->chk_show_highest_mood->isChecked());
        s->setValue("tooltip_show_thoughts", ui->chk_show_thoughts->isChecked());
        s->setValue("tooltip_show_squad", ui->chk_show_squad->isChecked());
        s->setValue("tooltip_show_age", ui->chk_show_age->isChecked());
        s->setValue("tooltip_show_size", ui->chk_show_unit_size->isChecked());
        s->setValue("tooltip_show_health", ui->chk_show_health->isChecked());
        s->setValue("tooltip_health_colors", ui->chk_health_colors->isChecked());
        s->setValue("tooltip_health_symbols", ui->chk_health_symbols->isChecked());
        s->setValue("tooltip_show_buffs", ui->chk_show_buffs->isChecked());
        short val = 0;
        if(ui->rad_syn_classes->isChecked())
            val = 1;
        else if(ui->rad_syn_both->isChecked())
            val = 2;
        s->setValue("syndrome_display_type", val);


        s->endGroup();
    }
}

void OptionsMenu::accept() {
    m_row_font.first = m_row_font.second;
    m_col_header_font.first = m_col_header_font.second;
    m_tooltip_font.first = m_tooltip_font.second;
    m_main_font.first = m_main_font.second;
    write_settings();
    emit settings_changed();
    QDialog::accept();
    int answer = QMessageBox::question(
            0, tr("Apply Options"),
            tr("Would you like to apply the new options now (Read Data)?"),
            QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes)
        DT->get_main_window()->read_dwarves();    
}

void OptionsMenu::reject() {
    m_row_font.second = m_row_font.first;
    m_col_header_font.second = m_col_header_font.first;
    m_tooltip_font.second = m_tooltip_font.first;
    m_main_font.second = m_main_font.first;
    read_settings();
    QDialog::reject();
}

void OptionsMenu::restore_defaults() {
    foreach(CustomColor *cc, m_general_colors) {
        cc->reset_to_default();
    }
    foreach(CustomColor *cc, m_happiness_colors) {
        cc->reset_to_default();
    }
    foreach(CustomColor *cc, m_noble_colors) {
        cc->reset_to_default();
    }
    ui->cb_read_dwarves_on_startup->setChecked(true);
    ui->cb_auto_contrast->setChecked(true);
    ui->cb_show_aggregates->setChecked(true);
    ui->cb_single_click_labor_changes->setChecked(false);
    ui->cb_show_toolbar_text->setChecked(true);
    ui->cb_auto_expand->setChecked(false);
    ui->cb_show_full_dwarf_names->setChecked(false);    
    ui->sb_min_skill_level->setValue(1);
    ui->cb_check_for_updates_on_startup->setChecked(true);
    ui->cb_alert_on_lost_connection->setChecked(true);
    ui->cb_labor_cheats->setChecked(false);
    ui->cb_header_text_direction->setChecked(false);
    ui->cb_curse_highlight->setChecked(false);
    ui->cb_happiness_icons->setChecked(false);
    ui->cb_noble_highlight->setChecked(false);
    ui->sb_roles_tooltip->setValue(3);
    ui->sb_roles_pane->setValue(10);
    ui->chk_custom_roles->setChecked(false);
    ui->chk_roles_in_labor->setChecked(false);
    ui->cb_labor_counts->setChecked(false);
    ui->cb_sync_grouping->setChecked(true);
    ui->cb_sync_scrolling->setChecked(false);
    ui->cb_moodable->setChecked(false);
    ui->cb_gender_icons->setChecked(true);
    ui->cb_grid_health_colors->setChecked(true);
    ui->cb_no_diagnosis->setChecked(false);
    ui->cb_animal_health->setChecked(false);

    ui->chk_show_caste->setChecked(true);
    ui->chk_show_caste_desc->setChecked(true);
    ui->chk_show_happiness->setChecked(true);
    ui->chk_show_icons->setChecked(true);
    ui->chk_show_noble->setChecked(true);
    ui->chk_show_prefs->setChecked(true);
    ui->chk_show_traits->setChecked(true);
    ui->chk_show_prof->setChecked(true);
    ui->chk_show_artifact->setChecked(true);
    ui->chk_show_highest_mood->setChecked(false);
    ui->chk_show_thoughts->setChecked(true);
    ui->chk_show_squad->setChecked(true);
    ui->chk_show_unit_size->setChecked(true);
    ui->chk_show_age->setChecked(true);
    ui->chk_show_buffs->setChecked(false);
    ui->rad_syn_names->setChecked(true);

    ui->dsb_attribute_weight->setValue(0.25);
    ui->dsb_pref_weight->setValue(0.15);
    ui->dsb_skill_weight->setValue(1.0);
    ui->dsb_trait_weight->setValue(0.20);
    ui->dsb_skill_rate_weight->setValue(0.25);
    ui->dsb_att_potential_weight->setValue(0.50);

    ui->chk_show_roles->setChecked(true);
    ui->chk_show_skills->setChecked(true);    
    ui->chk_show_health->setChecked(false);
    ui->chk_health_colors->setChecked(true);
    ui->chk_health_symbols->setChecked(false);

    QFont temp = QFont(DefaultFonts::getRowFontName(), DefaultFonts::getRowFontSize());
    m_row_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_current_font);

    temp = QFont(DefaultFonts::getHeaderFontName(), DefaultFonts::getHeaderFontSize());
    m_col_header_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_header_font);

    temp = QFont(DefaultFonts::getTooltipFontName(), DefaultFonts::getTooltipFontSize());
    m_tooltip_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_current_tooltip);

    temp = QFont(DefaultFonts::getMainFontName(), DefaultFonts::getMainFontSize());
    m_main_font = qMakePair(temp,temp);
    show_current_font(temp,ui->lbl_current_main_font);

    ui->sb_cell_size->setValue(DEFAULT_CELL_SIZE);
    ui->sb_cell_padding->setValue(0);
}

void OptionsMenu::show_font_chooser(QPair<QFont,QFont> &font_pair, QString msg, QLabel *l) {
    bool ok;
    QFont tmp = QFontDialog::getFont(&ok, font_pair.first, this, msg);
    if (ok) {
        show_current_font(tmp,l);
        font_pair.second = tmp;
    }
}

void OptionsMenu::show_current_font(QFont tmp, QLabel *l){
    l->setText(QString("%1 [%2]").arg(tmp.family()).arg(QString::number(tmp.pointSize())));
    l->setFont(tmp);
}

void OptionsMenu::show_header_font_chooser(){
    show_font_chooser(m_col_header_font,"Font used for grid/view column headers.",ui->lbl_header_font);
}
void OptionsMenu::show_row_font_chooser(){
    show_font_chooser(m_row_font,"Font used for grid/view row headers.",ui->lbl_current_font);
}
void OptionsMenu::show_main_font_chooser(){
    show_font_chooser(m_main_font,"General Dwarf Therapist font.",ui->lbl_current_main_font);
}
void OptionsMenu::show_tooltip_font_chooser(){
    show_font_chooser(m_tooltip_font,"Font used for all tooltips.",ui->lbl_current_tooltip);
}

void OptionsMenu::set_skill_drawing_method(const UberDelegate::SKILL_DRAWING_METHOD &sdm) {
    LOGD << "Setting SDM to" << UberDelegate::name_for_method(sdm);
    QSettings *s = DT->user_settings();
    s->setValue("options/grid/skill_drawing_method", static_cast<int>(sdm));
    read_settings(); // to set the combo-box correctly
    emit settings_changed();
}

void OptionsMenu::tab_index_changed(int index){
    if(index == ui->tabWidget->indexOf(ui->tab_roles)){
        int max_roles = GameDataReader::ptr()->get_roles().count();
        QString max_text = tr(" (max %1)").arg(max_roles);

        ui->lbl_pane_roles_max->setText(max_text);
        ui->sb_roles_pane->setMaximum(max_roles);

        ui->lbl_tooltip_roles_max->setText(max_text);
        ui->sb_roles_tooltip->setMaximum(max_roles);
    }
}
