//----------------------------------------------------
//Interface for XClassRealsenseCamera
//Created automatically at 2020.09.14 11:01:46
//----------------------------------------------------
//Page Main
//General settings.

//Const Enum Capture Source
//Source of frames - capture frames from real camera of load BAG recording from a disk.
enum enum_capture_source {
    capture_source_Disable = 0,
    capture_source_Camera = 1,
    capture_source_Bag_File = 2,
    capture_source_N__ = 3
};
bool was_changed_capture_source() { return was_changed("capture_source"); }
enum_capture_source gete_capture_source() { return enum_capture_source(geti("capture_source")); }

//Const Enum Device
//Choose the way for selecting the camera.
enum enum_select_device {
    select_device_Default = 0,
    select_device_By_Index = 1,
    select_device_By_Serial = 2,
    select_device_N__ = 3
};
bool was_changed_select_device() { return was_changed("select_device"); }
enum_select_device gete_select_device() { return enum_select_device(geti("select_device")); }

//Const Int Device Index
//Camera index in Operating System.
bool was_changed_device_index() { return was_changed("device_index"); }
int geti_device_index() { return geti("device_index"); }

//Const String Device Serial
//Serial of the camera to connect (or part of the serial), go to 'Device Info' to see the list.
bool was_changed_device_serial() { return was_changed("device_serial"); }
QString gets_device_serial() { return gets("device_serial"); }
QStringList get_strings_device_serial() { return get_strings("device_serial"); }

//Const String Bag File
//Choose BAG file. You can record it using Intel Realsense Viewer application.
bool was_changed_bag_file() { return was_changed("bag_file"); }
QString gets_bag_file() { return gets("bag_file"); }
QStringList get_strings_bag_file() { return get_strings("bag_file"); }


//Checkbox Runtime Details
//Runtime variables.
bool was_changed_runtime() { return was_changed("runtime"); }
int geti_runtime() { return geti("runtime"); }

//Out String Device Info
//Information about connected camera.
bool was_changed_connected_device_info() { return was_changed("connected_device_info"); }
QString gets_connected_device_info() { return gets("connected_device_info"); }
QStringList get_strings_connected_device_info() { return get_strings("connected_device_info"); }
void sets_connected_device_info(QString value) { sets("connected_device_info", value); }
void clear_string_connected_device_info() { clear_string("connected_device_info"); }
void append_string_connected_device_info(QString v, int extra_new_lines_count = 0) { append_string("connected_device_info", v, extra_new_lines_count); }
void append_string_connected_device_info(QStringList v, int extra_new_lines_count = 0) { append_string("connected_device_info", v, extra_new_lines_count); }

//Out Checkbox Is Started
//Does capture started.
bool was_changed_is_started() { return was_changed("is_started"); }
int geti_is_started() { return geti("is_started"); }
void seti_is_started(int value) { seti("is_started", value); }

//Out String Frames Captured
//How much frames were captured and processed after last start.
bool was_changed_frames_captured() { return was_changed("frames_captured"); }
QString gets_frames_captured() { return gets("frames_captured"); }
QStringList get_strings_frames_captured() { return get_strings("frames_captured"); }
void sets_frames_captured(QString value) { sets("frames_captured", value); }
void clear_string_frames_captured() { clear_string("frames_captured"); }
void append_string_frames_captured(QString v, int extra_new_lines_count = 0) { append_string("frames_captured", v, extra_new_lines_count); }
void append_string_frames_captured(QStringList v, int extra_new_lines_count = 0) { append_string("frames_captured", v, extra_new_lines_count); }


//Checkbox Show Color
//Generate color image preview. Please disable is not required for speed optimization.
bool was_changed_show_color() { return was_changed("show_color"); }
int geti_show_color() { return geti("show_color"); }

//Out Object Color
//Resulted Color image.
bool was_changed_color_image() { return was_changed("color_image"); }
XDict *getobj_color_image() { return get_object("color_image"); }

//Checkbox Show Depth
//Generate Depth image preview. Please disable is not required for speed optimization.
bool was_changed_show_depth() { return was_changed("show_depth"); }
int geti_show_depth() { return geti("show_depth"); }

//Out Object Depth
//Resulted Depth image.
bool was_changed_depth_image() { return was_changed("depth_image"); }
XDict *getobj_depth_image() { return get_object("depth_image"); }

//Checkbox Show IR
//Generate IR image preview. Please disable is not required for speed optimization.
bool was_changed_show_ir() { return was_changed("show_ir"); }
int geti_show_ir() { return geti("show_ir"); }

//Out Object IR
//Resulted IR image.
bool was_changed_ir_image() { return was_changed("ir_image"); }
XDict *getobj_ir_image() { return get_object("ir_image"); }

//Out Checkbox Is New Frame
//Flag is true when new frame is arrived.
bool was_changed_is_new_frame() { return was_changed("is_new_frame"); }
int geti_is_new_frame() { return geti("is_new_frame"); }
void seti_is_new_frame(int value) { seti("is_new_frame", value); }

//----------------------------------------------------
//Page Color
//Settings for color stream.

//Const Checkbox Color Stream
//Capture color stream.
bool was_changed_color_stream_enabled() { return was_changed("color_stream_enabled"); }
int geti_color_stream_enabled() { return geti("color_stream_enabled"); }

//Const Enum Resolution
//Resolution for color stream.
enum enum_color_resolution {
    color_resolution_320_x_180 = 0,
    color_resolution_320_x_240 = 1,
    color_resolution_424_x_240 = 2,
    color_resolution_640_x_360 = 3,
    color_resolution_640_x_480 = 4,
    color_resolution_848_x_480 = 5,
    color_resolution_960_x_540 = 6,
    color_resolution_1280_x_720 = 7,
    color_resolution_1920_x_1080 = 8,
    color_resolution_N__ = 9
};
bool was_changed_color_resolution() { return was_changed("color_resolution"); }
enum_color_resolution gete_color_resolution() { return enum_color_resolution(geti("color_resolution")); }

//Const Enum Frame Rate
//Frame rate for color stream.
enum enum_color_frame_rate {
    color_frame_rate_6 = 0,
    color_frame_rate_15 = 1,
    color_frame_rate_30 = 2,
    color_frame_rate_60 = 3,
    color_frame_rate_N__ = 4
};
bool was_changed_color_frame_rate() { return was_changed("color_frame_rate"); }
enum_color_frame_rate gete_color_frame_rate() { return enum_color_frame_rate(geti("color_frame_rate")); }

//Const Checkbox Aligh to Depth
//Should color frames to be aligned to depth frames.
bool was_changed_align_to_depth() { return was_changed("align_to_depth"); }
int geti_align_to_depth() { return geti("align_to_depth"); }


//----------------------------------------------------
//Page Depth + IR
//Settings for Depth and IR stream.

//Const Checkbox Depth Stream
//Capture Depth stream.
bool was_changed_depth_stream_enabled() { return was_changed("depth_stream_enabled"); }
int geti_depth_stream_enabled() { return geti("depth_stream_enabled"); }

//Const Checkbox IR Stream
//Capture IR stream.
bool was_changed_ir_stream_enabled() { return was_changed("ir_stream_enabled"); }
int geti_ir_stream_enabled() { return geti("ir_stream_enabled"); }

//Const Enum Preset
//Depth preset.
enum enum_depth_preset {
    depth_preset_Custom = 0,
    depth_preset_Default = 1,
    depth_preset_Hand = 2,
    depth_preset_High_Accuracy = 3,
    depth_preset_High_Density = 4,
    depth_preset_Medium_Density = 5,
    depth_preset_Remove_IR_Pattern = 6,
    depth_preset_N__ = 7
};
bool was_changed_depth_preset() { return was_changed("depth_preset"); }
enum_depth_preset gete_depth_preset() { return enum_depth_preset(geti("depth_preset")); }

//Const Enum Resolution
//Resolution for Depth/IR stream.
enum enum_depth_resolution {
    depth_resolution_424_x_240 = 0,
    depth_resolution_480_x_270 = 1,
    depth_resolution_640_x_360 = 2,
    depth_resolution_640_x_480 = 3,
    depth_resolution_848_x_480 = 4,
    depth_resolution_1280_x_720 = 5,
    depth_resolution_1280_x_800 = 6,
    depth_resolution_N__ = 7
};
bool was_changed_depth_resolution() { return was_changed("depth_resolution"); }
enum_depth_resolution gete_depth_resolution() { return enum_depth_resolution(geti("depth_resolution")); }

//Const Enum Frame Rate
//Frame rate for Depth/IR stream.
enum enum_depth_frame_rate {
    depth_frame_rate_6 = 0,
    depth_frame_rate_15 = 1,
    depth_frame_rate_25 = 2,
    depth_frame_rate_30 = 3,
    depth_frame_rate_60 = 4,
    depth_frame_rate_90 = 5,
    depth_frame_rate_N__ = 6
};
bool was_changed_depth_frame_rate() { return was_changed("depth_frame_rate"); }
enum_depth_frame_rate gete_depth_frame_rate() { return enum_depth_frame_rate(geti("depth_frame_rate")); }


//Const Checkbox Emitter
//Enabling IR laser emitter.
bool was_changed_emitter() { return was_changed("emitter"); }
int geti_emitter() { return geti("emitter"); }

//----------------------------------------------------
//Page Devices
//Here you can get list of connected cameras.

//Button Print Devices
//Print list of all connected devices
bool was_changed_print_devices() { return was_changed("print_devices"); }
int geti_print_devices() { return geti("print_devices"); }

//Out Text Device list
//Console with device list
bool was_changed_device_list() { return was_changed("device_list"); }
QString gets_device_list() { return gets("device_list"); }
QStringList get_strings_device_list() { return get_strings("device_list"); }
void sets_device_list(QString value) { sets("device_list", value); }
void clear_string_device_list() { clear_string("device_list"); }
void append_string_device_list(QString v, int extra_new_lines_count = 0) { append_string("device_list", v, extra_new_lines_count); }
void append_string_device_list(QStringList v, int extra_new_lines_count = 0) { append_string("device_list", v, extra_new_lines_count); }

//----------------------------------------------------
//Page Save Frames
//Saving frames to disk

//Button Save Frames
//Press the button to save images to a given folder
bool was_changed_save_frames_button() { return was_changed("save_frames_button"); }
int geti_save_frames_button() { return geti("save_frames_button"); }

//String Folder
//Folder for saving frames.
bool was_changed_save_folder() { return was_changed("save_folder"); }
QString gets_save_folder() { return gets("save_folder"); }
QStringList get_strings_save_folder() { return get_strings("save_folder"); }

//Out Text Saved To
//Path to last saving.
bool was_changed_saved_to() { return was_changed("saved_to"); }
QString gets_saved_to() { return gets("saved_to"); }
QStringList get_strings_saved_to() { return get_strings("saved_to"); }
void sets_saved_to(QString value) { sets("saved_to", value); }
void clear_string_saved_to() { clear_string("saved_to"); }
void append_string_saved_to(QString v, int extra_new_lines_count = 0) { append_string("saved_to", v, extra_new_lines_count); }
void append_string_saved_to(QStringList v, int extra_new_lines_count = 0) { append_string("saved_to", v, extra_new_lines_count); }

//----------------------------------------------------
