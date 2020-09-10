#include "rtmoduletest.h"
#include "incl_cpp.h"
#include "rtmoduleregistrar.h"
#include "projectruntime.h"

//заполнение имени класса и регистрация класса
REGISTRAR(Test)

//---------------------------------------------------------------------
/*static*/ XModuleTest *XModuleTest::new_module() {
    return new XModuleTest();
}

//---------------------------------------------------------------------
XModuleTest::XModuleTest()
    :XModule(*static_class_name_ptr)
{

}

//---------------------------------------------------------------------
XModuleTest::~XModuleTest()
{

}

//---------------------------------------------------------------------
void XModuleTest::start_impl() {
    xclu_console_append(QString("%1 - start").arg(name()));

}

//---------------------------------------------------------------------
void XModuleTest::update_impl() {
    xclu_console_append(QString("%1 - update").arg(name()));
    if (geti("callback")) {
        xclu_console_append(QString("%1 - pressed Callback").arg(name()));
        xclu_message_box("Callbacks are not implemented here");
        //RUNTIME.execute_callbacks(gets("callback_modules"));
    }
}

//---------------------------------------------------------------------
void XModuleTest::stop_impl() {
    xclu_console_append(QString("%1 - stop").arg(name()));



}

//---------------------------------------------------------------------
