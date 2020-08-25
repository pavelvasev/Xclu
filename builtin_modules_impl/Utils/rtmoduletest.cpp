#include "rtmoduletest.h"
#include "incl_cpp.h"
#include "rtmoduleregistrar.h"
#include "projectruntime.h"

//заполнение имени класса и регистрация класса
REGISTRAR(Test)

//---------------------------------------------------------------------
/*static*/ RtModuleTest *RtModuleTest::new_module() {
    return new RtModuleTest();
}

//---------------------------------------------------------------------
RtModuleTest::RtModuleTest()
    :RtModule(*static_class_name_ptr)
{

}

//---------------------------------------------------------------------
RtModuleTest::~RtModuleTest()
{

}

//---------------------------------------------------------------------
void RtModuleTest::execute_start_internal() {
    xclu_console_append(QString("%1 - start").arg(name()));

}

//---------------------------------------------------------------------
void RtModuleTest::execute_update_internal() {
    xclu_console_append(QString("%1 - update").arg(name()));
    if (get_int("callback")) {
        xclu_console_append(QString("%1 - pressed Callback").arg(name()));
        xclu_message_box("Callbacks are not implemented here");
        //RUNTIME.execute_callbacks(get_string("callback_modules"));
    }
}

//---------------------------------------------------------------------
void RtModuleTest::execute_stop_internal() {
    xclu_console_append(QString("%1 - stop").arg(name()));



}

//---------------------------------------------------------------------
