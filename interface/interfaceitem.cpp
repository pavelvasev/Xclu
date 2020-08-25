#include <QJsonObject>
#include <QJsonArray>

#include "interfaceitem.h"
#include "incl_cpp.h"
#include "interfaceitempage.h"
#include "interfaceitemgroup.h"
#include "interfaceitemseparator.h"
#include "interfaceitemfloat.h"
#include "interfaceitemint.h"
#include "interfaceitemcheckbox.h"
#include "interfaceitemstring.h"
#include "interfaceitemenum.h"
#include "interfaceitembutton.h"
#include "interfaceitemtext.h"
#include "interfaceitemobject.h"

#include "interfacegui.h"


//---------------------------------------------------------------------
/*static*/ InterfaceItem *InterfaceItem::create_item(ModuleInterface *parent,
                                                     QString title_underscored,
                                                     InterfaceItemType type,
                                                     const QStringList &description,
                                                     VarQualifier qual,
                                                     QString line_to_parse,
                                                     QString options,
                                                     QString qual_options
                                                     ) {
    InterfaceItemPreDescription descr;
    descr.title = xclu_remove_underscore(title_underscored);
    descr.type = type;
    descr.qualifier = qual;
    descr.qualifier_options = qual_options;
    descr.line_to_parse = line_to_parse;
    descr.options = options;
    descr.description = description;
    return create_item(parent, descr);
}

//---------------------------------------------------------------------
//page, group
/*static*/ InterfaceItem *InterfaceItem::create_decorate_item(ModuleInterface *parent,
                                                              QString name,
                                                              InterfaceItemType type, const QStringList &description) {
    //QString title = xclu_remove_underscore(name);
    return create_item(parent, name, type, description, VarQualifierIn, name);
}

//---------------------------------------------------------------------
//separator
/*static*/ InterfaceItem *InterfaceItem::create_separator(ModuleInterface *parent, QString name, InterfaceItemType type, bool is_line) {
    QString descr;
    if (is_line) descr = "line";    //если это линия - то указываем это как дополнительное обозначение в descriptor
    return create_item(parent, name, type, QStringList(descr), VarQualifierIn, name);
}

//---------------------------------------------------------------------
/*static*/ InterfaceItem *InterfaceItem::create_item(ModuleInterface *parent, const InterfaceItemPreDescription &pre_description) {
    switch(pre_description.type) {
    case InterfaceItemTypePage:
        return new InterfaceItemPage(parent, pre_description);
        break;
    case InterfaceItemTypeGroup:
        return new InterfaceItemGroup(parent, pre_description);
        break;
    case InterfaceItemTypeSeparator:
        return new InterfaceItemSeparator(parent, pre_description);
        break;
    case InterfaceItemTypeFloat:
        return new InterfaceItemFloat(parent, pre_description);
        break;
    case InterfaceItemTypeInt:
        return new InterfaceItemInt(parent, pre_description);
        break;
    case InterfaceItemTypeString:
        return new InterfaceItemString(parent, pre_description);
        break;
    case InterfaceItemTypeText:
        return new InterfaceItemText(parent, pre_description);
        break;
    case InterfaceItemTypeCheckbox:
        return new InterfaceItemCheckbox(parent, pre_description);
        break;
    case InterfaceItemTypeButton:
        return new InterfaceItemButton(parent, pre_description);
        break;
    case InterfaceItemTypeStringlist:
        return new InterfaceItemEnum(parent, pre_description);
        break;
    case InterfaceItemTypeObject:
        return new InterfaceItemObject(parent, pre_description);
        break;
    default:
        break;
    }
    xclu_exception("Internal error: item was not created, '" + pre_description.title
                   + "', type '" + interfacetype_to_string(pre_description.type) + "'");
    return nullptr;
}


//---------------------------------------------------------------------
//создание пункта интерфейса, и парсинг остатка строки line_to_parse
InterfaceItem::InterfaceItem(ModuleInterface *parent, const InterfaceItemPreDescription &pre_description) {
    //Проверка, что parent не нулевой - возможно, в конструкторе это не очень хорошо, но все же лучше проверить:)
    xclu_assert(parent,
                "Internal error in InterfaceItem constructor, empty 'ModuleInterface *parent' at '" + pre_description.title + "'");
    parent_ = parent;
    title_ = pre_description.title;
    type_ = pre_description.type;
    qualifier_ = pre_description.qualifier;
    description_ = pre_description.description;

    //опции квалификатора out(not_save)
    if (pre_description.qualifier_options.contains("not_save")) {
        save_to_project_ = false;
    }

}

//---------------------------------------------------------------------
InterfaceItem::~InterfaceItem() {

}

//---------------------------------------------------------------------
QString InterfaceItem::name() {
    return name_;
}

//---------------------------------------------------------------------
QString InterfaceItem::title() {
    return title_;
}

//---------------------------------------------------------------------
InterfaceItemType InterfaceItem::type() {
    return type_;
}

//---------------------------------------------------------------------
VarQualifier InterfaceItem::qualifier() {
    return qualifier_;
}

//---------------------------------------------------------------------
bool InterfaceItem::is_const() {
    return (qualifier_ == VarQualifierConst);
}

//---------------------------------------------------------------------
bool InterfaceItem::is_in() {
    return (qualifier_ == VarQualifierIn);
}

//---------------------------------------------------------------------
bool InterfaceItem::is_out() {
    return (qualifier_ == VarQualifierOut);
}

//---------------------------------------------------------------------
bool InterfaceItem::is_save_to_project() {
    return save_to_project_;
}

//---------------------------------------------------------------------
//Описание добавляется уже после создания, следующей строкой в скрипте GUI
QString InterfaceItem::description(int index) {
    if (index >= 0 && index < description_.size()) {
        return description_.at(index);
    }
    return "";
}

//---------------------------------------------------------------------
bool InterfaceItem::was_changed() {
    //Если было задано, что переменная изменилась - то просто запомнить
    if (force_changed_) {
        force_changed_ = false;
        if (supports_int()) last_int_ = value_int();
        else {
            if (supports_float()) last_float_ = value_float();
            else
                if (supports_string()) last_string_ = value_string();
        }
        //сбрасываем флажок с object
        if (supports_object()) {
            ObjectRead(get_object()).reset_changed(); // Доступ к объекту!
        }
        return true;
    }

    if (supports_int()) {
        if (last_int_ != value_int()) {
            last_int_ = value_int();
            return true;
        }
        return false;
    }
    if (supports_float()) {
        if (last_float_ != value_float()) {
            last_float_ = value_float();
            return true;
        }
        return false;
    }
    if (supports_string()) {
        if (last_string_ != value_string()) {
            last_string_ = value_string();
            return true;
        }
        return false;
    }
    //объекты - у них есть своя метка
    if (supports_object()) {
        return ObjectRead(get_object()).was_changed();  // Доступ к объекту!
    }

    return false;
}

//---------------------------------------------------------------------
void InterfaceItem::set_changed() { //пометить, что переменная была изменена
    force_changed_ = true;
}

//---------------------------------------------------------------------
int InterfaceItem::description_count() {
    return description_.size();
}

//---------------------------------------------------------------------
/*void InterfaceItem::add_description(QString description) {
    //xclu_assert(description_.isEmpty(), "non-empty description for variable '" + name_ + "' is already set, but trying to set another '" + description + "'");
    //description_ = description;
    description_.append(description);
}
*/

//---------------------------------------------------------------------
bool InterfaceItem::expression_enabled() {
    return (qualifier_ != VarQualifierOut);
}

//---------------------------------------------------------------------
bool InterfaceItem::use_expression() {  //используется ли expression для установки значения
    return use_expression_;
}

//---------------------------------------------------------------------
void InterfaceItem::set_use_expression(bool v) {
    use_expression_ = v;
}

//---------------------------------------------------------------------
QString InterfaceItem::expression() {
    return expression_;
}

//---------------------------------------------------------------------
void InterfaceItem::set_expression(const QString &expr) {
    expression_ = expr;
}

//---------------------------------------------------------------------
//парсинг q=0 0:1 100,10 -> name='q', query = '0','0:1','100,10'
/*static*/ void InterfaceItem::split_equal(const QString &line, QString &name, QStringList &query) {
    int equal = line.indexOf('=');
    xclu_assert(equal != -1, "no '=', expect 'some_name=...'");
    xclu_assert(equal != 0, "empty variable name, expect 'some_name=...'");
    name = line.left(equal);
    QRegExp rx("(\\ |\\t)"); //RegEx for ' ' or '\t'
    query = QString(line.mid(equal+1)).split(rx);
    xclu_assert(!query.isEmpty(), "expected something after '='");
}

//---------------------------------------------------------------------
//парсинг q A B -> name='q', query = 'A','B'
/*static*/ void InterfaceItem::split_spaced(const QString &line, QString &name, QStringList &query) {
    QRegExp rx("(\\ |\\t)"); //RegEx for ' ' or '\t'
    query = line.split(rx);
    xclu_assert(!query.isEmpty(), "empty name, expected ... q ...");
    name = query.at(0);
    query.removeFirst();
}

//---------------------------------------------------------------------
//парсинг числовых значений
/*static*/ float InterfaceItem::parse_float(QString line, QString error_message) {
    bool ok;
    float value = line.toFloat(&ok);
    xclu_assert(ok, error_message);
    return value;
}

//---------------------------------------------------------------------
/*static*/ int InterfaceItem::parse_int(QString line, QString error_message) {
    bool ok;
    float value = line.toInt(&ok);
    xclu_assert(ok, error_message);
    return value;
}

//---------------------------------------------------------------------
//графический интерфейс, он тут создается, но хранится отдельно
InterfaceGui *InterfaceItem::create_gui(InterfaceGuiPageCreator &input) {
    gui__ = new InterfaceGui(input, this);
    return gui__;   //не нужно его удалять
}

//---------------------------------------------------------------------
//сигнал, что GUI подключен/отключен
void InterfaceItem::gui_attached() {
    gui_attached_ = true;
}

//---------------------------------------------------------------------
void InterfaceItem::gui_detached() {
    gui_attached_ = false;
}

//---------------------------------------------------------------------
bool InterfaceItem::is_gui_attached() {
    return gui_attached_;
}

//---------------------------------------------------------------------
void InterfaceItem::gui_to_var(bool evaluate_expr) { //вычисление expression и получение значения из gui
    if (use_expression()) {
        if (evaluate_expr) {
            //... expression()
            //if (is_gui_attached()) {
            //    var_to_gui_internal();
            //}
        }
    }
    else {
        if (is_gui_attached()) {
            gui_to_var_internal();
        }
    }
}

//---------------------------------------------------------------------
void InterfaceItem::var_to_gui() { //установка значения в gui, также отправляет сигнал о видимости
    //if (!use_expression()) {
    if (is_gui_attached()) {
        var_to_gui_internal();
        //отправляем сигнал о видимости
        propagate_visibility();
    }
}

//---------------------------------------------------------------------
void InterfaceItem::propagate_visibility() {    //обновить дерево видимости - используется, в частности, при тестировании интерфейса
    if (gui__) {
        gui__->propagate_visibility();
    }
}

//---------------------------------------------------------------------
//запретить редактирование - всегда для out и после запуска для const
void InterfaceItem::block_gui_editing() {
    if (is_gui_attached()) {
        if (gui__) {
            gui__->block_editing();
        }
    }
}

//---------------------------------------------------------------------
//разрешить редактирование
void InterfaceItem::unblock_gui_editing() {
    if (is_gui_attached()) {
        if (gui__) {
            gui__->unblock_editing();
        }
    }
}

//---------------------------------------------------------------------
//копирование данных - для duplicate; предполагается, что имя и тип - одинаковые
//специальные типы, которые не поддерживают перенос через строку (array и image) - должны переписать copy_data_to_internal
void InterfaceItem::copy_data_to(InterfaceItem *item) {
    //если для данного типа не надо копировать - то ничего не делаем
    if (!store_data()) return;

    xclu_assert(name() == item->name(), "Internal error InterfaceItem::copy_data_to: different items names '"
                + name() + "' and '" + item->name() + "'");
    xclu_assert(type() == item->type(), "Internal error InterfaceItem::copy_data_to: different items types '"
                + interfacetype_to_string(type()) + "' and '" + interfacetype_to_string(item->type()) + "'");

    item->set_use_expression(use_expression());
    item->set_expression(expression());

    //если можно пробросить через строку - делаем так
    if (supports_string()) {
        item->set_value_string(value_string());
    }
    else {
        //иначе - item должен иметь реализованную copy_data_to_internal
        copy_data_to_internal(item);
    }

}

//---------------------------------------------------------------------
void InterfaceItem::copy_data_to_internal(InterfaceItem *) {
    xclu_halt("Internal error: copy_data_to_internal is not implemented for type '" + interfacetype_to_string(type()) + "'");
}

//---------------------------------------------------------------------
//Запись и считывание json
void InterfaceItem::write_json(QJsonObject &json) {
    if (!is_save_to_project()) return;
    if (!store_data()) return;
    xclu_assert(supports_string(), "Can't write item '" + name_ + "' to json");

    //"a..." - чтобы были в начале списка
    json["aname"] = name_;
    json["atitle"] = title_;
    json["atype"] = interfacetype_to_string(type_);  //записываем, чтобы отловить ошибки при изменении интерфейса
    json["avalue"] = value_string();
    if (expression_enabled()) {
        json["expr_use"] = use_expression_;
        json["expr"] = expression_;
    }
}

//---------------------------------------------------------------------
void InterfaceItem::read_json(const QJsonObject &json) {
    if (!is_save_to_project()) return;
    if (!store_data()) return;
    xclu_assert(supports_string(), "Can't read item '" + name_ + "' from json, because it's has 'out' (that means read-only) qualifier.\n"
                "May be module's version is different");
    //проверяем, что имя совпадает
    QString name = json_string(json, "aname");
    xclu_assert(name == name_, "Different name for '" + name_ + "'.\n"
                "May be module's version is different");

    //заголовок - закомментировал, пусть он меняется
    //title_ = json_string(json, "atitle");

    //проверяем, что тип совпадает
    QString type_str = json_string(json, "atype");
    xclu_assert(type_str == interfacetype_to_string(type_), "Different type for '" + name_ + "'.\n"
                "May be module's version is different");

    //значение
    set_value_string(json_string(json, "avalue"));

    //expression
    if (expression_enabled()) {
        use_expression_ = json_bool(json, "expr_use");
        expression_ = json_string(json, "expr");
    }

}

//---------------------------------------------------------------------



