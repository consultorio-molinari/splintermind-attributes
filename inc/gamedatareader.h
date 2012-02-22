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
#ifndef GAME_DATA_READER_H
#define GAME_DATA_READER_H

#include <string>
#include <stdexcept>
#include <QtCore>
#include "raws/rawobjectlist.h"

// forward declaration
class QSettings;
#include "labor.h"
#include "attribute.h"
class Trait;
class MilitaryPreference;
class Profession;
class DwarfJob;

// exceptions
class MissingValueException : public std::runtime_error {
public:
    MissingValueException(const std::string &msg) : runtime_error(msg) {}
};

class CorruptedValueException : public std::runtime_error {
public:
    CorruptedValueException(const std::string &msg) : runtime_error(msg) {}
};

//singleton reader of game data
class GameDataReader : public QObject {
    Q_OBJECT
public:
    static GameDataReader *ptr() {
        if (!m_instance) {
            m_instance = new GameDataReader(0);
        }
        return m_instance;
    }
    int get_int_for_key(QString key, short base = 16);
    int get_address(QString key) {return get_int_for_key("addresses/" + key);}
    int get_offset(QString key) {return get_int_for_key("offsets/" + key);}
    int get_dwarf_offset(QString key) {return get_int_for_key("dwarf_offsets/" + key);}
    int get_xp_for_next_attribute_level(int current_number_of_attributes);

    QList<Labor*> get_ordered_labors() {return m_ordered_labors;}
    QHash<int, QString> get_skills() {return m_skills;}
    QList<QPair<int, QString> > get_ordered_skills() {return m_ordered_skills;}
    QHash<int, Trait*> get_traits() {return m_traits;}
    QList<QPair<int, Trait*> > get_ordered_traits() {return m_ordered_traits;}
    QHash<int, MilitaryPreference*> get_military_preferences() {return m_military_preferences;}
    QHash<short, Profession*> get_professions() {return m_professions;}

    Labor *get_labor(const int &labor_id);
    Trait *get_trait(const int &trait_id);
    DwarfJob *get_job(const short &job_id);
    MilitaryPreference *get_military_preference(const int &mil_pref_id);

    QString get_string_for_key(QString key);
    Profession* get_profession(const short &profession_id);
    QString get_skill_level_name(short level);
    QString get_skill_name(short skill_id);
    QString get_attribute_level_name(QString attribute, short level);

    QColor get_color(QString key);

    QStringList get_child_groups(QString section);
    QStringList get_keys(QString section);
    int get_level_from_xp(int xp);

    RawObjectPtr get_reaction(QString reactionClass, QString id) {
        if(m_reaction_classes.contains(reactionClass)) {
            return m_reaction_classes.value(reactionClass)
                    .getRawObject("REACTION", id);
        }
        return RawObjectPtr();
    }

    RawObjectPtr get_creature(QString creatureClass, QString id) {
        if(m_creatures_classes.contains(creatureClass)) {
            return m_creatures_classes.value(creatureClass)
                    .getRawObject("CREATURE", id);
        }
        return RawObjectPtr();
    }

    QString get_race_name(int race_id);
    QString get_caste_name(int caste_id);
    int get_attribute_mean_value(int attribute, int caste) {return m_attributes_mean_value.value(caste).value(attribute);}

    void read_raws(QDir df_dir);

protected:
    GameDataReader(QObject *parent = 0);
private:
    void load_race_names();
    void load_caste_names();
    void load_attributes_mean_value();
    static GameDataReader *m_instance;
    QSettings *m_data_settings;

    QHash<int, Labor*> m_labors;
    QList<Labor*> m_ordered_labors;

    QHash<int, MilitaryPreference*> m_military_preferences;

    QHash<int, Trait*> m_traits;
    QList<QPair<int, Trait*> > m_ordered_traits;

    QHash<int, QString> m_skills;
    QList<QPair<int, QString> > m_ordered_skills;

    QHash<int, QString> m_skill_levels;
    QHash<int, int> m_attribute_levels;
    QHash<QString, Attribute*> m_attributes;
    QHash<short, DwarfJob*> m_dwarf_jobs;
    QHash<short, Profession*> m_professions;

    QHash<QString, QRawObjectList> m_reaction_classes;
    QHash<QString, QRawObjectList> m_creatures_classes;

    QHash<QString, QString> m_race_names;
    QHash<QString, QString> m_caste_names;
    QHash<int, QHash<int, int> > m_attributes_mean_value;
};
#endif
