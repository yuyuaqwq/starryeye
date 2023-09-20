//#ifndef STARRY_EYE_IO_DEVICE_H_
//#define STARRY_EYE_IO_DEVICE_H_
//
//#include <krnlib/stl_container.hpp>
//#include <krnlib/string.hpp>
//
//#include <basic.h>
//
//namespace stareye {
//namespace io {
//
//class Device {
//public:
//  Device() : device_object_ { nullptr } {
//
//  }
//  ~Device() {
//    Close();
//  }
//
//  /*
//  * \\Device\\..
//  */
//  bool Create(DRIVER_OBJECT* driver_object, _SCN wstring name) {
//    name_ = name;
//    UNICODE_STRING unicode_name;
//    RtlInitUnicodeString(&unicode_name, name.c_str());
//    NTSTATUS status = IoCreateDevice(driver_object, 0, &unicode_name, FILE_DEVICE_UNKNOWN, 0, TRUE, &device_object_);
//    return NT_SUCCESS(status);
//  }
//
//  void Close() {
//    if (device_object_) {
//      IoDeleteDevice(device_object_);
//      device_object_ = nullptr;
//    }
//  }
//
//  DEVICE_OBJECT* Get() {
//    return device_object_;
//  }
//
//  _SCN wstring GetName() {
//    return name_;
//  }
//
//private:
//  DEVICE_OBJECT* device_object_;
//  _SCN wstring name_;
//};
//
//} // namespace io
//} // namespace starry_eye
//
//#endif // STARRY_EYE_IO_DEVICE_H_