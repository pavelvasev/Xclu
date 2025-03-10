#ifndef INTERFACEITEM_H
#define INTERFACEITEM_H

//Абстрактный элемент интерфейса (переменные и оформление GUI), невизуальная часть
//Подклассы реализуют конкретные элементы.
//Также, они порождают визуальное представление в виде XGui

#include "incl_h.h"
#include "xpointer.h"
#include "xobject.h"
#include "xlink.h"
#include "componentcontextmenu.h"
#include "xwaschanged.h"

struct XGuiPageBuilder;
class XGui;
class QJsonObject;
class XItem;
class Module;
class ModuleInterface;

//---------------------------------------------------------------------
//Preliminary information for constructing interface element
class XItemPreDescription {
public:
    QString title;
    QString type = "";
    XQualifier qualifier = XQualifierIn;
    QString qualifier_options; //опции в квалификаторе, типа out(save)
    QString options;        //дополнительные опции, типа choose:file, choose:folder для строк string(choose:file)
    QString line_to_parse;  //полная строка для парсинга: in float ...
    QStringList description;    //строки с описанием
};


//---------------------------------------------------------------------
//Hepler - Element creator class
class XItemCreator {
public:
    static XItem *new_item(ModuleInterface *interf, const XItemPreDescription &pre_description);
    static XItem *new_item(ModuleInterface *interf, QString title_underscored, QString type,
                                  const QStringList &description,
                                  XQualifier qual = XQualifierIn, QString line_to_parse = "",
                                  QString options = "",
                                  QString qual_options = "");
    //page, group
    static XItem *new_decorate_item(ModuleInterface *interf, QString name, QString type, const QStringList &description);
    //separator
    static XItem *new_separator(ModuleInterface *interf, QString name, QString type_raw = xitem_separator());
};

//---------------------------------------------------------------------
//Module interface component
class XItem {
public:
    //создание невизуальной переменной (или описание элемента интерфейса),
    //и парсинг остатка строки line_to_parse
    XItem(ModuleInterface *interf, const XItemPreDescription &pre_description);
    virtual ~XItem();

    //access to interface
    ModuleInterface *interf();
    //access to module
    Module *module();

    //Compiling links and other things
    virtual void compile();

    //update
    //checks changes at current frame
    //and maintains "link" copying (for scalars and objects) or sets pointer (for objects optionally)
    //Note: update should be called after updating value from user GUI to correctly maintain "was changed"
    virtual void update();

    //Имя и тип, а также информация для создания GUI  -------------------------
    QString name();
    QString title();
    QString type();
    //Qualifier of the item
    //Please note, also `is_linked` can be used - for linked variables, which are const or in
    XQualifier qualifier();
    bool is_const();
    bool is_in();
    bool is_out();
    bool is_save_to_project();    //в случае out(save) ставится true. Это знак, что значение переменной нужно сохранять в файле проекта

    //Описание добавляется уже после создания, следующей строкой в скрипте GUI
    //может быть несколько строк - например, в string с выбором файла или папки,
    //или значение по умолчанию для текста
    //поэтому, к ним доступ через индекс. Если элемента нет - возвращается пустая строка
    QString description(int index = 0);
    int description_count();
    //void add_description(QString description); //не позволяем добавлять описание - так как его сразу передаем в конструктор

    //Checking that value was changed -------------------------
    //works relative to save "change chacker", which stores frame fo last check
    //It's really implemented at XItem_<T>
    virtual bool was_changed(XWasChangedChecker &checker);

    //Set "was changed" at start. Really it's implemented in XItem_<T>, but here just reset was_changed_
    void reset_was_changed_simple();

    //was changed for one-point access, between "update" calling
    bool was_changed_simple() { return was_changed_; }


    //Доступ к значениям -------------------------
    //Соответствующие типы должны переопределить эти функции
    //Здесь мы не проверяем правильность использования, и перелагаем проверку на модули
    //string
    virtual bool supports_string() { return false; }
    virtual QString value_string() { return ""; }
    virtual void set_value_string(const QString &) {}

    //string to write/read json (for enum is custom)
    virtual QString value_string_json() { return value_string(); }
    virtual void set_value_string_json(const QString &v) { set_value_string(v); }

    //int
    virtual bool supports_int() { return false; }
    virtual int value_int() { return 0; }
    virtual void set_value_int(int) {}

    //float
    virtual bool supports_float() { return false; }
    virtual float value_float() { return 0; }
    virtual void set_value_float(float) {}

    //value_raw - for enum
    virtual bool supports_value_raw() { return false; }
    virtual QString value_raw() { return ""; }
    virtual void set_value_raw(QString) {}

    //value_title - for enum
    virtual bool supports_value_title() { return false; }
    virtual QString value_title() { return ""; }
    virtual void set_value_title(QString) {}

    //object
    virtual bool supports_object() { return false; }
    virtual XProtectedObject *get_object() { return nullptr; }
    virtual void set_object(XProtectedObject *) {}

    //Запись, считывание и копирование -------------------------
    //хранит ли данные (или просто интерфейсный элемент, например, сепаратор)
    virtual bool store_data() { return true; }

    //копирование данных - для duplicate; предполагается, что имя и тип - одинаковые
    //специальные типы, которые не поддерживают перенос через строку (object) - должны переписать copy_data_to_internal
    void copy_data_to(XItem *item);

    //Запись и считывание json
    void write_json(QJsonObject &json);
    void read_json(const QJsonObject &json);

    //Link -------------------------
    //obtain link to itself - for using in "Copy link"
    XLinkParsed get_link_to_itself();

    bool is_link_can_be_used(); //can be link used (for out - no), used for project saving
    bool is_linked() const;     //is using link enabled - works together with 'is_const`, `is_in`, `is_out`
    const XLink &link() const;
    void set_link(const XLink &link);
    void set_link(bool enabled, QString link);
    void clear_link();    

    //User change link settings - should show it in GUI and switch value_ in XItem_
    virtual void link_was_changed();

    //Expression -------------------------
    bool is_expression_can_be_used(); //требуется ли expression (для out - нет), используется при записи на диск
    bool is_use_expression();  //используется ли expression для установки значения
    void set_use_expression(bool v);
    QString expression();
    void set_expression(const QString &expr);

    //GUI -------------------------
    //графический интерфейс, он тут создается, но хранится отдельно
    virtual XGui *create_gui(XGuiPageBuilder &page_builder);

    //сигнал, что GUI подключен/отключен
    void gui_attached();
    void gui_detached();
    bool is_gui_attached();

    //Working with GUI
    bool is_matches_qual_mask(const XQualifierMask &qual);
    void gui_to_var(const XQualifierMask &qual, bool evaluate_expr); //вычисление expression и получение значения из gui
    void var_to_gui(const XQualifierMask &qual); //установка значения в gui, также отправляет сигнал о видимости
    void block_gui_editing(const XQualifierMask &qual);       //запретить редактирование - всегда для out и после запуска для const
    void unblock_gui_editing(const XQualifierMask &qual);     //разрешить редактирование
    void propagate_visibility();    //обновить дерево видимости - используется, в частности, при тестировании интерфейса

    //Belonging to general page, common for all modules
    void set_belongs_general_page(bool v);
    bool belongs_general_page();

    //Context menu ----------------
    //Common setting up of context menu
    //It's filled using custom implementations of context_menu_has_set_default_value() and context_menu_has_set_size()
    ComponentContextMenuInfo context_menu_info();

    //Each component can provide information menu details
    virtual bool context_menu_has_set_default_value() { return false; }
    virtual bool context_menu_has_set_size() { return false; }

    //Processing of context menu actions
    //Subclasses must implement non-common actions and call parent method
    virtual void context_menu_on_action(ComponentContextMenuEnum id, QString action_text);

    //C++ -------------------------
    //function `export_interface` generates function or functions definitions
    //for using inside C++ class module definition
    //----------------------------------------------------
    virtual void export_interface(QStringList &file);
    //Subclasses must reimplement it, in opposite case the exception will arise.
    //export_interface_template() - useful helper for this
    //Note: only items which are not belong to general page, that is belongs_general_page() == false
    //are exported by default


protected:
    //доступ ко всему интерфейсу
    ModuleInterface *interf_ = nullptr;

    //основные характеристики
    QString title_;
    QString name_;
    QString type_;
    XQualifier qualifier_;
    QStringList description_;   //может быть несколько строк

    bool save_to_project_ = true;  //если false, то не записывать значение в проект

    //Was changed checker
    XWasChangedChecker was_changed_checker_;
    bool was_changed_ = false;  //value changed at each update"

    //Link ----------------------------------------
    //Link packed in stricg for editing
    XLink link_;

    //Link, ready to use runtime
    QScopedPointer<XLinkResolved> link_resolved_;

    //find item corresponding to link
    //called from `compile` and `link_was_changed`
    void resolve_link();

    //Function for setting value using link
    //Subclasses must implement it
    virtual void set_value_from_link(XLinkResolved *linkres);

    //Expression ----------------------------------------
    //Выражения (в данный момент не поддерживаются)
    bool use_expression_ = false;
    QString expression_;

    //парсинг q=0 0:1 100,10 -> name='q', query = '0','0:1','100,10'
    //        v="aaa bbb" -> name='v', query = "aaa bbb"
    static void split_equal(const QString &line, QString &name, QStringList &query);

    //парсинг q A B -> name='q', query = 'A','B'
    static void split_spaced(const QString &line, QString &name, QStringList &query);

    //подключен ли GUI
    bool gui_attached_ = false;

    //работа с GUI - внутренняя реализация, без expression,
    //реализуется в конкретных классах
    //вызывается, когда is_gui_attached - поэтому, это не нужно проверять
    virtual void gui_to_var_internal() {} //получение значения из gui
    virtual void var_to_gui_internal() {} //установка значения в gui

    //копирование данных - для duplicate; предполагается, что имя и тип - одинаковые
    //специальные типы, которые не поддерживают перенос через строку (array и image) - должны переписать copy_data_to_internal
    virtual void copy_data_to_internal(XItem *item);


    //это общее gui__ - хотя в самих представителях будут конкретные представители,
    //нам требуется общий, чтобы передавать сигналы о видимости
    //подклассы должны его устанавливать!
    XGui *gui__ = nullptr; //не нужно его удалять

    //General page marker
    bool belongs_general_page_ = false;

    //Helper for export_interface
    void export_interface_template(QStringList &file,
                                   bool horiz_line,
                                   bool comment_description,
                                   QString custom_comment_begin,

                                   bool getter_setter = false,
                                   QString cpp_type = "",
                                   QString fun_prefix = "",
                                   QString cpp_getter = "",
                                   QString cpp_setter = "",
                                   bool final_blank = true,
                                   bool is_int = false,
                                   bool is_string = false,
                                   bool is_object = false
            );
};

//---------------------------------------------------------------------
//Interface element with value
//allows to use links and protection for multithread access
template<typename T>
class XItem_: public XItem {
public:
    XItem_<T>(ModuleInterface *interf, const XItemPreDescription &pre_description)
        : XItem(interf, pre_description) {
        //by default, set value_ as owner
        //value_.own(new XProtectedData_<T>(new T()));

    }

    //Access for read and write value
    //for one-time access of scalars,
    //can use `value_read().data()`, `value_.data()->write().data() = value`
    //Note: each "value_write" calling calls increasing frame for "was changed keeper"
    XProtectedRead_<T> value_read() { return value_.read(); }
    XProtectedWrite_<T> value_write() { return value_.write(); }

    //Checking that value was changed -------------------------
    //works relative to save "change checker", which stores frame fo last check
    virtual bool was_changed(XWasChangedChecker &checker) {
        return value_.was_changed(checker);
    }

protected:
     XProtectedData_<T> value_;

};



#endif // INTERFACEITEM_H
