#include "BranchPredictor.h"

void BranchPredictor::setPrediction(const PC &cur_pc, const PC &pc_disp, bool comp) {
    std::bitset<32> pc{cur_pc.realVal()};
    std::bitset<32> target{PC{cur_pc + pc_disp}.realVal()};
    // In RV32I all opcodes begins with 11, so hashing them is useless
    auto tag = sub_range<31, key_size + 2>(pc);
    std::bitset<key_size> key = sub_range<key_size + 1, 2>(pc);
    auto &bht_bucket1 = bht_[key.to_ulong()].first;
    auto &bht_bucket2 = bht_[key.to_ulong()].second;
    auto tag1 = sub_range<30 - key_size - 1, 0>(bht_bucket1);
    auto tag2 = sub_range<30 - key_size - 1, 0>(bht_bucket2);

    if (/* bit valid */ bht_bucket1[bht_bucket_size - 1] && tag1 == tag) {
        updatePrediction(bht_bucket1, comp);
    } else if (/* bit valid */ bht_bucket2[bht_bucket_size - 1] && tag2 == tag) {
        updatePrediction(bht_bucket2, comp);
    } else if (/* bit valid */ !bht_bucket1[bht_bucket_size - 1]) {
        setupBucket(bht_bucket1, btb_[key.to_ulong()].first, key.to_ulong(), comp, target, tag);
    } else if (/* bit valid */ !bht_bucket2[bht_bucket_size - 1]) {
        setupBucket(bht_bucket2, btb_[key.to_ulong()].second, key.to_ulong(), comp, target, tag);
        swapBuckets(key.to_ulong());
    } else {
        // Shift
        updatePrediction(bht_bucket2, comp);
        swapBuckets(key.to_ulong());
    }
}

void BranchPredictor::updatePrediction(std::bitset<bht_bucket_size> &bht_bucket, bool comp) {
    auto prediction = calcPrediction(bht_bucket, comp);
    bht_bucket = assign_sub<bht_bucket_size - 2, bht_bucket_size - 3>(bht_bucket, prediction);
}

void BranchPredictor::setupBucket(std::bitset<bht_bucket_size> &bht_bucket, std::bitset<btb_bucket_size> &btb_bucket,
                                  uint32_t key, bool comp, std::bitset<32> target, std::bitset<30 - key_size> tag) {
    bht_bucket.set(bht_bucket_size - 1);
    //  For default prediction is weak
    auto prediction = comp ? std::bitset<2>{0b10} : std::bitset<2>{0b01};
    bht_bucket = assign_sub<bht_bucket_size - 2, bht_bucket_size - 3>(bht_bucket, prediction);
    bht_bucket = assign_sub<30 - key_size - 1, 0>(bht_[key].first, tag);

    btb_bucket = assign_sub<btb_bucket_size - 1, 30 - key_size>(btb_[key].first, target);
    btb_bucket = assign_sub<30 - key_size - 1, 0>(btb_[key].first, tag);
}

void BranchPredictor::swapBuckets(uint32_t key) {
    std::swap(bht_[key].first, bht_[key].second);
    std::swap(btb_[key].first, btb_[key].second);
}

std::bitset<2> BranchPredictor::calcPrediction(std::bitset<bht_bucket_size> bht_bucket, bool comp) {
    auto pred = sub_range<bht_bucket_size - 2, bht_bucket_size - 3>(bht_bucket);
    switch (pred.to_ulong()) {
        case 0b00:  //  strongly not taken
            return comp ? std::bitset<2>{0b01} : pred;
        case 0b01:  //  weakly not taken
            return comp ? std::bitset<2>{0b10} : std::bitset<2>{0b00};
        case 0b10:  //  weakly taken
            return comp ? std::bitset<2>{0b11} : std::bitset<2>{0b01};
        case 0b11:  //  strongly taken
            return comp ? pred : std::bitset<2>{0b00};
    }

    return {};
}

bool BranchPredictor::getPrediction(const PC &cur_pc) const noexcept {
    std::bitset<32> pc{cur_pc.realVal()};
    std::bitset<key_size> key = sub_range<key_size + 1, 2>(pc);
    if (bht_.find(key.to_ulong()) == bht_.end()) {
        return false;
    }

    auto tag = sub_range<31, key_size + 2>(pc);
    auto &bht_bucket1 = bht_.at(key.to_ulong()).first;
    auto &bht_bucket2 = bht_.at(key.to_ulong()).second;
    auto tag1 = sub_range<30 - key_size - 1, 0>(bht_bucket1);
    auto tag2 = sub_range<30 - key_size - 1, 0>(bht_bucket2);

    std::bitset<bht_bucket_size> bht_bucket;
    if (/* bit valid */ bht_bucket1[bht_bucket_size - 1] && tag == tag1) {
        bht_bucket = bht_bucket1;
    } else if (/* bit valid */ bht_bucket2[bht_bucket_size - 1] && tag == tag2) {
        bht_bucket = bht_bucket2;
    } else {
        return false;
    }

    auto pred = sub_range<bht_bucket_size - 2, bht_bucket_size - 3>(bht_bucket);
    switch (pred.to_ulong()) {
        case 0b00:  //  strongly not taken
        case 0b01:  //  weakly not taken
            return false;
        case 0b10:  //  weakly taken
        case 0b11:  //  strongly taken
            return true;
    }

    return false;
}

PC BranchPredictor::getTarget(bool pred, const PC &cur_pc) const noexcept {
    if (!pred) {
        return cur_pc;
    }

    std::bitset<32> pc{cur_pc.realVal()};
    auto tag = sub_range<31, key_size + 2>(pc);
    std::bitset<key_size> key = sub_range<key_size + 1, 2>(pc);
    auto &btb_bucket1 = btb_.at(key.to_ulong()).first;
    auto &btb_bucket2 = btb_.at(key.to_ulong()).second;
    auto tag1 = sub_range<30 - key_size - 1, 0>(btb_bucket1);
    auto tag2 = sub_range<30 - key_size - 1, 0>(btb_bucket2);

    std::bitset<32> target;
    if (tag == tag1) {
        target = sub_range<btb_bucket_size - 1, 30 - key_size>(btb_bucket1);
    } else if (tag == tag2) {
        target = sub_range<btb_bucket_size - 1, 30 - key_size>(btb_bucket2);
    } else {
        std::cerr << "Error in branch prediction\n";
        return cur_pc;
    }

    return PC{static_cast<uint32_t>(target.to_ulong() / 4)};
}
