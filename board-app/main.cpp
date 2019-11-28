#include<iostream>
#include <android-base/logging.h>
#include <android/hardware/usb/1.0/IUsb.h>
#include <android/hardware/usb/1.0/IUsbCallback.h>
#include <android/hardware/usb/1.0/types.h>
#include <unistd.h>

using ::android::hardware::usb::V1_0::IUsbCallback;
using ::android::hardware::usb::V1_0::IUsb;
using ::android::hardware::usb::V1_0::PortDataRole;
using ::android::hardware::usb::V1_0::PortMode;
using ::android::hardware::usb::V1_0::PortPowerRole;
using ::android::hardware::usb::V1_0::PortRole;
using ::android::hardware::usb::V1_0::PortRoleType;
using ::android::hardware::usb::V1_0::PortStatus;
using ::android::hardware::usb::V1_0::Status;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;


class usbCallback : public IUsbCallback {

public:
	Return<void> notifyPortStatusChange(
 		const hidl_vec<PortStatus>& currentPortStatus, Status retval) override {
      	if (retval == Status::SUCCESS) {

	auto i = 0;
        for(;i < currentPortStatus.size(); i++) {
		std::cout << "Port Name:" << currentPortStatus[i].portName << std::endl;
	}
      }
      return Void();
    };

	    // Callback method for the status of role switch operation.
    Return<void> notifyRoleSwitchStatus(const hidl_string& /*portName*/,
                                        const PortRole& newRole,
                                        Status retval) override {
    auto usb_last_status = retval;
     auto usb_last_port_role = newRole;
      return Void();
    };
 
};

usbCallback *ptrUsbCallback;
int main()
{
	auto usb = IUsb::getService();
        ptrUsbCallback = new usbCallback();
	usb->setCallback(ptrUsbCallback);

	 usb->queryPortStatus();
	sleep(5);
	std::cout << "From Board" << std::endl;

}
