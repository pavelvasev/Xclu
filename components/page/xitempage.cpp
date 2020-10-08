#include "xitempage.h"
#include "xguipage.h"
#include "registrarxitem.h"

REGISTER_XITEM(XItemPage, page)
//---------------------------------------------------------------------
XItemPage::XItemPage(ModuleInterface *interf, const XItemPreDescription &pre_description)
    : XItem(interf, pre_description)
{
    //page Main_page
    name_ = pre_description.line_to_parse;
}

//---------------------------------------------------------------------
//графический интерфейс
XGui *XItemPage::create_gui(XGuiPageCreator &input) {
    gui__ = new XGuiPage(input, this);
    return gui__;
}

//---------------------------------------------------------------------
//получение значения из gui
void XItemPage::gui_to_var_internal() {
    scroll_ = ((XGuiPage *)gui__)->get_vscroll();
}

//---------------------------------------------------------------------
//установка значения в gui
void XItemPage::var_to_gui_internal() {
    ((XGuiPage *)gui__)->set_vscroll(scroll_);
}

//---------------------------------------------------------------------
//Context menu
//Each component must provide information about its menu
ComponentPopupMenuInfo XItemPage::component_popup_info() {
    return ComponentPopupMenuInfo(false, false, false, false);
}

//---------------------------------------------------------------------
//C++
void XItemPage::export_interface(QStringList &file) {
    export_interface_template(file, true, true, "Page ");
}

//---------------------------------------------------------------------
