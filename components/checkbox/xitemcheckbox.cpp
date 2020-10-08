#include "xitemcheckbox.h"
#include "xguicheckbox.h"
#include "incl_cpp.h"

#include "registrarxitem.h"

REGISTER_XITEM(XItemCheckbox, checkbox)

//---------------------------------------------------------------------
//in checkbox Active active=0
//      //Length of the object.    описание

//in checkbox(group) Details show_details=0
//- показ чеббокса слева, для обозначения группы элементов GUI, и еще справа строка

XItemCheckbox::XItemCheckbox(ModuleInterface *interf, const XItemPreDescription &pre_description)
    : XItem_<int>(interf, pre_description)
{
    QString line = pre_description.line_to_parse;
    QStringList query;
    split_equal(line, name_, query);
    xclu_assert(query.size()>=1, "no default value, expected '...q=0...'");

    //опции - "групповой" чекбокс
    QString options = pre_description.options;
    if (!options.isEmpty()) {
        //является ли "групповым"
        is_group_checkbox_ = (options == "group");
        //какая-то из опций должна быть включена, если переданы не пустые
        xclu_assert(is_group_checkbox_, "Unknown option, expected '... checkbox(group)...'");
    }

    //значение по умолчанию
    default_value_ = parse_int(query.at(0), "default value must be an integer, but is '" + query.at(0) + "'");
    set_value_int(default_value_);

}

//---------------------------------------------------------------------
//графический интерфейс
XGui *XItemCheckbox::create_gui(XGuiPageCreator &input) {
    gui__ = new XGuiCheckbox(input, this);
    return gui__;
}

//---------------------------------------------------------------------
//получение значения из gui
void XItemCheckbox::gui_to_var_internal() {
    set_value_int( ((XGuiCheckbox *)gui__)->value());
}

//---------------------------------------------------------------------
//установка значения в gui
void XItemCheckbox::var_to_gui_internal() {
    ((XGuiCheckbox *)gui__)->set_value(value_int());
}

//---------------------------------------------------------------------
//Context menu
//Each component must provide information about its menu
ComponentPopupMenuInfo XItemCheckbox::component_popup_info() {
    return ComponentPopupMenuInfo(!is_out(), false, !is_out(), false);
}

//---------------------------------------------------------------------
//C++
void XItemCheckbox::export_interface(QStringList &file) {
    export_interface_template(file, false, true, "Checkbox ", true,
                              "int ", "i", "geti", "seti");
}

//---------------------------------------------------------------------
