/*
 * Copyright (C) 2014-2019 Jolla Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by: Mohammed Hassan <mohammed.hassan@jolla.com>
 */

using namespace android;

#include <sensor/ISensorServer.h>
#include <sensor/ISensorEventConnection.h>
#include <sensor/Sensor.h>
#include <sensor/BitTube.h>

class FakeSensorEventConnection : public BnSensorEventConnection
{
    sp<BitTube> mChannel;
public:
    FakeSensorEventConnection()
    {
        mChannel = new BitTube(0);
    }
    sp<BitTube> getSensorChannel() const {
        return mChannel;
    }
    status_t enableDisable(int, bool, nsecs_t,
                            nsecs_t, int) {
        return 0;
    }
    status_t setEventRate(int, nsecs_t) {
        return 0;
    }
    status_t flush() {
        return 0;
    }

    virtual int32_t configureChannel(int32_t, int32_t) {
        return 0;
    }
protected:
    void destroy() {
    }

};
class FakeSensorServer : public BinderService<FakeSensorServer>,
                         public BnSensorServer
{
public:
    static char const *getServiceName() {
        return "sensorservice";
    }

    Vector<Sensor> getSensorList(const String16&) {
        return Vector<Sensor>();
    }

    Vector<Sensor> getDynamicSensorList(const String16&) {
        return Vector<Sensor>();
    }
    sp<ISensorEventConnection> createSensorEventConnection(const String8&,
                        int, const String16&) {
        return sp<ISensorEventConnection>(new FakeSensorEventConnection);
    }

    sp<ISensorEventConnection> createSensorDirectConnection(const String16&,
            uint32_t, int32_t, int32_t, const native_handle_t *) {
        return sp<ISensorEventConnection>(new FakeSensorEventConnection);
    }

    int setOperationParameter(
            int32_t, int32_t, const Vector<float> &, const Vector<int32_t> &) {
        return 0;
    }

    int32_t isDataInjectionEnabled() {
        return 0;
    }

    virtual status_t shellCommand(int, int, int,
                                  Vector<String16>&) {
        return 0;
    }
};

#include <android/frameworks/sensorservice/1.0/IEventQueue.h>
#include <android/frameworks/sensorservice/1.0/ISensorManager.h>
#include <android/frameworks/sensorservice/1.0/types.h>
#include <android/hardware/sensors/1.0/types.h>

class FakeEventQueue :
    public android::frameworks::sensorservice::V1_0::IEventQueue
{
public:
    FakeEventQueue() {}

    android::hardware::Return<android::frameworks::sensorservice::V1_0::Result> enableSensor(
            int32_t sensorHandle, int32_t samplingPeriodUs, int64_t maxBatchReportLatencyUs) {
        return android::frameworks::sensorservice::V1_0::Result::BAD_VALUE;
    }

    android::hardware::Return<android::frameworks::sensorservice::V1_0::Result> disableSensor(
            int32_t sensorHandle) {
        return android::frameworks::sensorservice::V1_0::Result::BAD_VALUE;
    }
};

class FakeSensorManager :
    public android::frameworks::sensorservice::V1_0::ISensorManager
{

    // Methods from ::android::frameworks::sensorservice::V1_0::ISensorManager follow.
    android::hardware::Return<void> getSensorList(getSensorList_cb _hidl_cb) {
        android::hardware::hidl_vec<::android::hardware::sensors::V1_0::SensorInfo> ret;
        _hidl_cb(ret, android::frameworks::sensorservice::V1_0::Result::OK);
        return android::hardware::Void();
    }

    android::hardware::Return<void> getDefaultSensor(
            android::hardware::sensors::V1_0::SensorType type,
            getDefaultSensor_cb _hidl_cb) {
        _hidl_cb({}, android::frameworks::sensorservice::V1_0::Result::NOT_EXIST);
        return android::hardware::Void();
    }

    android::hardware::Return<void> createAshmemDirectChannel(
            const android::hardware::hidl_memory& mem, uint64_t size,
            createAshmemDirectChannel_cb _hidl_cb) {
        _hidl_cb(nullptr, android::frameworks::sensorservice::V1_0::Result::BAD_VALUE);
        return android::hardware::Void();
    }

    android::hardware::Return<void> createGrallocDirectChannel(
            const android::hardware::hidl_handle& buffer, uint64_t size,
            createGrallocDirectChannel_cb _hidl_cb) {
        _hidl_cb(nullptr, android::frameworks::sensorservice::V1_0::Result::UNKNOWN_ERROR);
        return android::hardware::Void();
    }

    android::hardware::Return<void> createEventQueue(
            const sp<android::frameworks::sensorservice::V1_0::IEventQueueCallback> &callback,
            createEventQueue_cb _hidl_cb) {
        if (callback == nullptr) {
            _hidl_cb(nullptr, android::frameworks::sensorservice::V1_0::Result::BAD_VALUE);
            return android::hardware::Void();
        }

        sp<android::frameworks::sensorservice::V1_0::IEventQueue> queue = new FakeEventQueue();

        _hidl_cb(queue, android::frameworks::sensorservice::V1_0::Result::OK);
        return android::hardware::Void();
    }
};
