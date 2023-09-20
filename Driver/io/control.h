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
//    // ���IRP��Ĺؼ�����
//    pIrpStack = IoGetCurrentIrpStackLocation(irp);
//    // ��ȡ������
//    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
//    // ���������Ļ�������DeviceIoControl��InBuffer��OutBuffer��������
//    pIoBuffer = (uint8_t*)irp->AssociatedIrp.SystemBuffer;
//    // EXE���ʹ������ݵ�BUFFER���ȣ�DeviceIoControl��nInBufferSize��
//    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
//    // EXE���մ������ݵ�BUFFER���ȣ�DeviceIoControl��nOutBufferSize��
//    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
//    // �Բ�ͬ�����źŵĴ�������
//    switch (uIoControlCode) {
//      // ���ջ���
//    case kControlBaseCode: {
//      auto& control = control_map_->find(device_object);
//      // ����ͨ��״̬
//      status = STATUS_SUCCESS;
//      if (control != control_map_->end()) {
//        uint32_t callback_code = 0;
//        // �õ��������
//        memcpy(&callback_code, pIoBuffer, sizeof(uint32_t));
//        pIoBuffer += 4;
//
//        auto& callback = control->second->callback_code_map_.find(callback_code);
//        if (callback != control->second->callback_code_map_.end()) {
//          // ���������������Ӧ�ò��У�����Ҫע����Ǻ��� DeviceIoControl ���� Windows API �е�һ��ϵͳ�����乤����Ӧ�ò㣬�������豸��������ͨ�š��ú����������豸���������ݡ������Ǹú������﷨��
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
//    // �趨DeviceIoControl��*lpBytesReturned��ֵ�����ͨ��ʧ���򷵻�0���ȣ�
//    if (status == STATUS_SUCCESS)
//      irp->IoStatus.Information = uOutSize;
//    else
//      irp->IoStatus.Information = 0;
//    // �趨DeviceIoControl�ķ���ֵ�ǳɹ�����ʧ��
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