//
// Created by bele on 25.07.17.
//

#include <thread>
#include "BLEScanner.h"

void BLEScanner::scan() {
    if (running && stopped) {
        // restart
    }
    if (running && !stopped) {
        // already running
        return;
    }
    if (!running && stopped) {
        // we can start it normally
    }
    if (!running && !stopped) {
        // we can start it normally
    }
    // TODO start scanning
}

std::shared_ptr<BLEAdvertisment> BLEScanner::awaitAdvertisment() {
    using namespace std::chrono_literals;
    // http://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
    std::this_thread::sleep_for(2000ms);
    //return std::shared_ptr<BLEAdvertisment>();
    uint8_t mac[6] = {1, 2, 3, 4, 5, 6};
    return std::make_shared<BLEAdvertisment>(mac);
}

void BLEScanner::stop() {
    BLEScanner::stopped = true;
}


const std::atomic<bool> &BLEScanner::isStopped() const {
    return stopped;
}

const std::atomic<bool> &BLEScanner::isRunning() const {
    return running;
}

BLEScanner::BLEScanner(std::shared_ptr<BLEController> controller) : controller(controller){

}

std::shared_ptr<BLEAdvertisment> BLEScanner::getAdvertisment() {
    return std::shared_ptr<BLEAdvertisment>();
}
