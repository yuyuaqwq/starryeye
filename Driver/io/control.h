//#ifndef STARRY_EYE_IO_CONTROL_H_
//#define STARRY_EYE_IO_CONTROL_H_
//
//#include <krnlib/stl_container.hpp>
//#include <krnlib/functional.hpp>
//
//#include <basic.h>
//
//#include <io/device.h>
//#include <io/symbolic_link.h>
//
//namespace stareye {
//namespace io {
//
//class Control {
//public:
//  //typedef size_t(*Callback)(void* buf, size_t len);
//  using Callback = krnlib::function<size_t(void* buf, size_t len)>;
//
//private:
//  static NTSTATUS DispatchIoControl(DEVICE_OBJECT* device_object, PIRP irp) {
//    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
//    PIO_STACK_LOCATION pIrpStack;
//    ULONG uIoControlCode;
//    uint8_t* pIoBuffer;
//    ULONG uInSize;
//    ULONG uOutSize;
//    // 获得IRP里的关键数据
//    pIrpStack = IoGetCurrentIrpStackLocation(irp);
//    // 获取控制码
//    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
//    // 输入和输出的缓冲区（DeviceIoControl的InBuffer和OutBuffer都是它）
//    pIoBuffer = (uint8_t*)irp->AssociatedIrp.SystemBuffer;
//    // EXE发送传入数据的BUFFER长度（DeviceIoControl的nInBufferSize）
//    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
//    // EXE接收传出数据的BUFFER长度（DeviceIoControl的nOutBufferSize）
//    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
//    // 对不同控制信号的处理流程
//    switch (uIoControlCode) {
//      // 接收或发送
//    case kControlBaseCode: {
//      auto& control = control_map_->find(device_object);
//      // 返回通信状态
//      status = STATUS_SUCCESS;
//      if (control != control_map_->end()) {
//        uint32_t callback_code = 0;
//        // 得到输入参数
//        memcpy(&callback_code, pIoBuffer, sizeof(uint32_t));
//        pIoBuffer += 4;
//
//        auto& callback = control->second->callback_code_map_.find(callback_code);
//        if (callback != control->second->callback_code_map_.end()) {
//          // 设置输出参数而在应用层中，最需要注意的是函数 DeviceIoControl 它是 Windows API 中的一个系统函数其工作于应用层，用于与设备驱动程序通信。该函数可以向设备驱区的数据。以下是该函数的语法：
//          uint32_t buf_len;
//          memcpy(&buf_len, pIoBuffer, sizeof(uint32_t));
//          pIoBuffer += 4;
//          uOutSize = callback->second(pIoBuffer, buf_len);
//          if (uOutSize > pIrpStack->Parameters.DeviceIoControl.OutputBufferLength) {
//            uOutSize = 0;
//          }
//        }
//      }
//      break;
//    }
//    default: {
//      irp->IoStatus.Status = status;
//      irp->IoStatus.Information = uOutSize;
//      IoCompleteRequest(irp, IO_NO_INCREMENT);
//      return status;
//    }
//    }
//    // 设定DeviceIoControl的*lpBytesReturned的值（如果通信失败则返回0长度）
//    if (status == STATUS_SUCCESS)
//      irp->IoStatus.Information = uOutSize;
//    else
//      irp->IoStatus.Information = 0;
//    // 设定DeviceIoControl的返回值是成功还是失败
//    irp->IoStatus.Status = status;
//    IoCompleteRequest(irp, IO_NO_INCREMENT);
//    return status;
//  }
//
//public:
//  Control() {
//    if (!control_map_) {
//      control_map_ = new _SCN map<DEVICE_OBJECT*, Control*>;
//      control_map_reference_count_ = 1;
//    }
//    else {
//      control_map_reference_count_++;
//    }
//  }
//  ~Control() {
//    if (--control_map_reference_count_ == 0) {
//      delete control_map_;
//    }
//  }
//
//  bool Create(Device* device, _SCN wstring symlink_name) {
//    symlink_.Create(symlink_name, device_->GetName());
//    device_ = device;
//    control_map_->insert(std::make_pair(device_->Get(), this));
//    device_->Get()->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoControl;
//    return true;
//  }
//
//  void Close() {
//    control_map_->erase(device_->Get());
//  }
//
//  void Register(uint32_t callback_code, const Callback& callback) {
//    callback_code_map_.insert(std::make_pair(callback_code, callback));
//  }
//
//private:
//  Device* device_;
//  SymbolicLink symlink_;
//  _SCN map<uint32_t, Callback> callback_code_map_;
//
//private:
//  static inline _SCN map<DEVICE_OBJECT*, Control*>* control_map_;
//  static inline uint32_t control_map_reference_count_;
//
//private:
//  constexpr static DWORD kControlBaseCode = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS);
//};
//
//} // namespace io
//} // namespace starry_eye
//
//#endif // STARRY_EYE_IO_CONTROL_H_