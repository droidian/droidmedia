#include <binder/BinderService.h>

#if ANDROID_MAJOR == 10
#include "services/services_10_0_0_sensors.h"
#endif

void FakeSensorServices_instantiate() {
    static sp<android::frameworks::sensorservice::V1_0::ISensorManager> sensorManager = new FakeSensorManager;
    status_t status = sensorManager->registerAsService();
    (void)status;

    FakeSensorServer::instantiate();
}
