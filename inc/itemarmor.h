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
#ifndef ITEMARMOR_H
#define ITEMARMOR_H

#include "item.h"
#include "itemarmorsubtype.h"

class ItemArmor : public Item {
public:

    ItemArmor(const Item &baseItem)
        :Item(baseItem)
    {
        read_def();
    }

    ItemArmor(DFInstance *df, VIRTADDR item_addr)
        :Item(df,item_addr)
    {
        read_def();
    }

    virtual ~ItemArmor(){
        m_df = 0;
        m_armor_def = 0;
    }

    ItemArmorSubtype * get_details(){return m_armor_def;}
    short item_subtype(){return m_armor_def->subType();}

private:
    ItemArmorSubtype *m_armor_def;

    void read_def(){
        if(m_addr > 0){            
            m_armor_def =  ItemArmorSubtype::get_armor(m_df,m_df->read_addr(m_addr+m_df->memory_layout()->item_offset("item_def")),this);
            m_armor_def->set_item_type(m_iType);
            m_item_name = m_armor_def->name();
            QString layer_name = m_armor_def->get_layer_name();
            if(layer_name != "")
                m_layer_name = layer_name;
        }
    }
};

#endif // ITEMARMOR_H
