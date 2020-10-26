#include "qt_widgets.h"
#include "xmoduleproject.h"
#include "incl_cpp.h"
#include "registrarxmodule.h"
#include "xcore.h"


//Register implementation class
REGISTER_XMODULE(Project)

//---------------------------------------------------------------------
XModuleProject::XModuleProject(QString class_name)
    :XModule(class_name)
{

}

//---------------------------------------------------------------------
XModuleProject::~XModuleProject()
{

}

//---------------------------------------------------------------------
void XModuleProject::impl_loaded() {
    if (is_enabled()) {
        apply_control_values();
    }
}

//---------------------------------------------------------------------
void XModuleProject::apply_control_values() {
    XCORE.set_frame_rate(geti_frame_rate());
    XCORE.set_autostart(geti_autostart());
}

//---------------------------------------------------------------------
void XModuleProject::impl_start() {
    apply_control_values();
}

//---------------------------------------------------------------------
void XModuleProject::impl_update() {
    setf_elapsed_time_sec(xcore_elapsed_time_sec());

    float dt = xcore_dt();
    setf_dt(dt);

    float fps = 10000;
    if (dt > 0) fps = 1.0 / dt;
    setf_measured_frame_rate(fps);

}

//---------------------------------------------------------------------
void XModuleProject::impl_stop() {

}

//---------------------------------------------------------------------
