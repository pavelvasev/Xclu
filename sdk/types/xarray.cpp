#include "xarray.h"
#include "incl_cpp.h"

//---------------------------------------------------------------------
//размер одного элемента данных
unsigned int XArrayDataTypeSize(XArrayDataType type) {
    switch (type) {
    case XArrayDataType_none:
        return 0;
    case XArrayDataType_u8bit:
        return 1;
    case XArrayDataType_s8bit:
        return 1;
    case XArrayDataType_int16:
        return 2;
    case XArrayDataType_uint16:
        return 2;
    case XArrayDataType_int32:
        return 4;
    case XArrayDataType_uint32:
        return 4;
    case XArrayDataType_float:
        return 4;
    case XArrayDataType_double:
        return 8;
    default:
        xclu_exception(QString("Unknown XArrayDataType %1").arg(type));
        break;
    }
    return 0;
}

//---------------------------------------------------------------------
QString XArrayDataType_to_string(XArrayDataType type) {
    switch (type) {
    case XArrayDataType_none:
        return "";
    case XArrayDataType_u8bit:
        return "u8bit";
    case XArrayDataType_s8bit:
        return "s8bit";
    case XArrayDataType_int16:
        return "int16";
    case XArrayDataType_uint16:
        return "uint16";
    case XArrayDataType_int32:
        return "int32";
    case XArrayDataType_uint32:
        return "uint32";
    case XArrayDataType_float:
        return "float";
    case XArrayDataType_double:
        return "double";
    default:
        xclu_exception(QString("Unknown XArrayDataType %1").arg(type));
        break;
    }
    return 0;
}

//---------------------------------------------------------------------
XArrayDataType string_to_XArrayDataType(QString type) {
    if (type == "none") return XArrayDataType_none;
    if (type == "u8bit") return XArrayDataType_u8bit;
    if (type == "s8bit") return XArrayDataType_s8bit;
    if (type == "int16") return XArrayDataType_int16;
    if (type == "uint16") return XArrayDataType_uint16;
    if (type == "int32") return XArrayDataType_int32;
    if (type == "uint32") return XArrayDataType_uint32;
    if (type == "float") return XArrayDataType_float;
    if (type == "double") return XArrayDataType_double;
    xclu_exception(QString("Unknown XArrayDataType '%1'").arg(type));
    return XArrayDataType_none;
}

//---------------------------------------------------------------------
bool is_XArrayDataType_integer(XArrayDataType type) {
    switch (type) {
    case XArrayDataType_none:
        return false;
    case XArrayDataType_u8bit:
        return true;
    case XArrayDataType_s8bit:
        return true;
    case XArrayDataType_int16:
        return true;
    case XArrayDataType_uint16:
        return true;
    case XArrayDataType_int32:
        return true;
    case XArrayDataType_uint32:
        return true;
    case XArrayDataType_float:
        return false;
    case XArrayDataType_double:
        return false;
    default:
        xclu_exception(QString("Unknown XArrayDataType %1").arg(type));
        break;
    }
    return 0;
}

//---------------------------------------------------------------------
bool is_XArrayDataType_float(XArrayDataType type) {
    if (type == XArrayDataType_none) return false;
    return !is_XArrayDataType_integer(type);
}

//---------------------------------------------------------------------
XArray::XArray()
{

}

//---------------------------------------------------------------------
void XArray::clear() {
    size_ = 0;
    size_bytes_ = 0;
    elem_size_ = 0;
    data_type_ = XArrayDataType_none;
    data_.clear();
    data_ptr_ = nullptr;

}

//---------------------------------------------------------------------
void XArray::fill(int v) {
    if (is_empty()) return;
    if (is_int()) {
        for (quint32 i=0; i<size_; i++) {
            seti(i, v);
        }
        return;
    }
    fill(double(v));
}

//---------------------------------------------------------------------
void XArray::fill(double v) {
    if (is_empty()) return;
    xclu_assert(is_float() || is_double(), "It's allowed to fill only float and double arrays with floats");
    if (is_float()) {
        for (quint32 i=0; i<size_; i++) {
            setf(i, v);
        }
    }
    if (is_double()) {
        for (quint32 i=0; i<size_; i++) {
            set_double(i, v);
        }
    }
}


//---------------------------------------------------------------------
void XArray::allocate(unsigned int size, XArrayDataType data_type) {
    xclu_assert(size>=0, QString("Bad total array size %1").arg(size));
    quint32 elem_size = XArrayDataTypeSize(data_type);
    quint32 size_bytes = elem_size * size;

    //data
    if (size_bytes != size_bytes_) {
        data_.resize(size_bytes);
        data_ptr_ = &data_[0];
    }

    //parameters
    data_type_ = data_type;
    size_ = size;
    size_bytes_ = size_bytes;
    elem_size_ = elem_size;
}


//---------------------------------------------------------------------
XArrayDataType XArray::data_type() const {
    return data_type_;
}

//---------------------------------------------------------------------
unsigned int XArray::size() const {    //число элементов
    return size_;
}

//---------------------------------------------------------------------
unsigned int XArray::size_bytes() const {  //размер массива в байтах
    return size_bytes_;
}

//---------------------------------------------------------------------
unsigned int XArray::elem_size() const {    //размер одного элемента
    return elem_size_;
}

//---------------------------------------------------------------------
bool XArray::is_empty() const {
    return size_bytes_ == 0;
}


//---------------------------------------------------------------------
inline bool XArray::is_int() const {      //это целочисленный массив
    return is_XArrayDataType_integer(data_type_);
}

//---------------------------------------------------------------------
inline bool XArray::is_float() const {    //это массив float
    return data_type_ == XArrayDataType_float;
}

//---------------------------------------------------------------------
inline bool XArray::is_double() const {    //это массив double
    return data_type_ == XArrayDataType_double;
}

//---------------------------------------------------------------------
//получение ссылки на элемент массива
void *XArray::item_pointer(qint32 index) {
    return data_ptr_ + (index * elem_size_);
}

void const *XArray::item_pointer(qint32 index) const {
    return data_ptr_ + (index * elem_size_);
}

//---------------------------------------------------------------------
int XArray::geti(qint32 index) const {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_u8bit:
        return data_u8bit()[index];
    case XArrayDataType_s8bit:
        return data_s8bit()[index];
    case XArrayDataType_int16:
        return data_int16()[index];
    case XArrayDataType_uint16:
        return data_uint16()[index];
    case XArrayDataType_int32:
        return data_int32()[index];
    case XArrayDataType_uint32:
        return data_uint32()[index];
    default:
        xclu_exception("Can't get integer value for array");
        break;
    }
    return 0;
}


//---------------------------------------------------------------------
void XArray::seti(qint32 index, int v) {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_u8bit:
        data_u8bit()[index] = v;
        break;
    case XArrayDataType_s8bit:
        data_s8bit()[index] = v;
        break;
    case XArrayDataType_int16:
        data_int16()[index] = v;
        break;
    case XArrayDataType_uint16:
        data_uint16()[index] = v;
        break;
    case XArrayDataType_int32:
        data_int32()[index] = v;
        break;
    case XArrayDataType_uint32:
        data_uint32()[index] = v;
        break;
    default:
        xclu_exception("Can't set integer value for array");
        break;
    }
}

//---------------------------------------------------------------------
float XArray::getf(qint32 index) const {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_float:
        return data_float()[index];
    case XArrayDataType_double:
        return data_double()[index];
    default:
        xclu_exception("Can't get float value for array");
        break;
    }
    return 0;
}

//---------------------------------------------------------------------
void XArray::setf(qint32 index, float v) {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_float:
        data_float()[index] = v;
        break;
    case XArrayDataType_double:
        data_double()[index] = v;
        break;
    default:
        xclu_exception("Can't set float value for array");
        break;
    }
}

//---------------------------------------------------------------------
double XArray::get_double(qint32 index) const {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_float:
        return data_float()[index];
    case XArrayDataType_double:
        return data_double()[index];
    default:
        xclu_exception("Can't get double value for array");
        break;
    }
    return 0;
}

//---------------------------------------------------------------------
void XArray::set_double(qint32 index, double v) {
    xclu_assert(index >= 0 && index < size_, "Bad index for array access");
    switch (data_type_) {
    case XArrayDataType_float:
        data_float()[index] = v;
        break;
    case XArrayDataType_double:
        data_double()[index] = v;
        break;
    default:
        xclu_exception("Can't set double value for array");
        break;
    }
}

//---------------------------------------------------------------------
/*
получение массивов данных для быстрой работы
quint8* XArray::data_u8bit() {
    if (size_bytes_ == 0) return nullptr;
    xclu_assert(data_type_ == XArrayDataType_u8bit, "Array has no " "u8bit" " pointer");
    return (quint8*)(&data_[0]);
}

*/
#define XArray_get_data(TYPE_NAME,CPP_TYPE) \
    CPP_TYPE* XArray::data_##TYPE_NAME() { \
        if (size_bytes_ == 0) return nullptr; \
        xclu_assert(data_type_ == XArrayDataType_##TYPE_NAME, "Array has another type, can't get " #TYPE_NAME " pointer"); \
        return (CPP_TYPE*)(data_ptr_); \
    }

#define XArray_get_data_const(TYPE_NAME,CPP_TYPE) \
    CPP_TYPE const* XArray::data_##TYPE_NAME() const { \
        if (size_bytes_ == 0) return nullptr; \
        xclu_assert(data_type_ == XArrayDataType_##TYPE_NAME, "Array has another type, can't get " #TYPE_NAME " pointer"); \
        return (CPP_TYPE*)(data_ptr_); \
    }

XArray_get_data(u8bit, quint8)
XArray_get_data(s8bit, qint8)
XArray_get_data(int16, qint16)
XArray_get_data(uint16, quint16)
XArray_get_data(int32, qint32)
XArray_get_data(uint32, quint32)
XArray_get_data(float, float)
XArray_get_data(double, double)


XArray_get_data_const(u8bit, quint8)
XArray_get_data_const(s8bit, qint8)
XArray_get_data_const(int16, qint16)
XArray_get_data_const(uint16, quint16)
XArray_get_data_const(int32, qint32)
XArray_get_data_const(uint32, quint32)
XArray_get_data_const(float, float)
XArray_get_data_const(double, double)

void* XArray::data() {
    return data_ptr_;
}
void const* XArray::data() const {
    return data_ptr_;
}


//---------------------------------------------------------------------
