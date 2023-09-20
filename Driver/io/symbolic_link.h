//#ifndef STARRY_EYE_IO_SYMBOLIC_LINK_H_
//#define STARRY_EYE_IO_SYMBOLIC_LINK_H_
//
//#include <krnlib/stl_container.hpp>
//
//#include <basic.h>
//
//namespace stareye {
//namespace io {
//
//class SymbolicLink {
//public:
//  SymbolicLink() {
//    
//  }
//
//  ~SymbolicLink() {
//    Close();
//  }
//
//  bool Create(_SCN wstring sym_name, _SCN wstring device_name) {
//    UNICODE_STRING unicode_sym_name, unicode_device_name;
//    RtlInitUnicodeString(&unicode_sym_name, sym_name.c_str());
//    RtlInitUnicodeString(&unicode_device_name, device_name.c_str());
//    NTSTATUS status = IoCreateSymbolicLink(&unicode_sym_name, &unicode_device_name);
//    if (NT_SUCCESS(status)) {
//      sym_name_ = sym_name;
//    }
//    return NT_SUCCESS(status);
//  }
//
//  void Close() {
//    if (!sym_name_.empty()) {
//      UNICODE_STRING unicode_sym_name;
//      RtlInitUnicodeString(&unicode_sym_name, sym_name_.c_str());
//      IoDeleteSymbolicLink(&unicode_sym_name);
//      sym_name_.clear();
//    }
//  }
//
//private:
//  _SCN wstring sym_name_;
//};
//
//} // namespace io
//} // namespace starry_eye
//
//#endif // STARRY_EYE_IO_SYMBOLIC_LINK_H_