//
// Created by bele on 25.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H

#include "helper/Observer.h"
#include "BLEAdvertisement.h"
#include "BLEAdapter.h"
#include "enum.h"

#include <memory>
#include <atomic>
#include <queue>
#include <map>

class BLEAdapter;
class BLEAdvertisement;

class BLEScanner {

public:

    BLEScanner(std::shared_ptr<BLEAdapter> shared_ptr);

    ~BLEScanner();

    std::shared_ptr<BLEAdvertisement> getAdvertisment();

    bool stop();

    void notifyAdvertisment(std::shared_ptr<BLEAdvertisement> advertisment);

private:
    void pushAdvertisment(std::shared_ptr<BLEAdvertisement> advertisment);
    std::shared_ptr<BLEAdvertisement> popAdvertisment();
    std::map<std::string, std::shared_ptr<BLEAdvertisement>> bleAdvertisments;
    std::queue<std::shared_ptr<BLEAdvertisement>> unrequestedBleAdvertisments;
    std::shared_ptr<BLEAdapter> controller = nullptr;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H
