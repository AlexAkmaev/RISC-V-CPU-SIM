#ifndef UNITS_BRANCH_PREDICTION_H
#define UNITS_BRANCH_PREDICTION_H

#include "Basics.h"
#include <map>
#include <utility>

class BranchPredictor final {
public:
    explicit BranchPredictor() = default;

    void setPrediction(const PC &cur_pc, const PC &pc_disp, bool comp);

    bool getPrediction(const PC &cur_pc);

    std::bitset<32> getTarget(bool pred, const PC &cur_pc);

private:
    static constexpr const uint8_t key_size = 10;  // pc = tag + key_size + 2 bits (that are the same in RV32I)
    /*
     *  BHT bucket bits ordering:
     *  |<-- 1 -->|<------- 2 -------->|<--------------------- 30 - key_size ----------------------------->|
     *  [bit_valid|    prediction      |                           tag                                     ]
     */
    static constexpr const uint8_t bht_bucket_size = 33 - key_size;  // = 1 bit validation + 2 bits prediction + tag
    /*
     *  BTB bucket bits ordering:
     *  |<------------------------ 32 ------------------------>|<------------- 30 - key_size ------------->|
     *  [                       target pc                      |                   tag                     ]
     */
    static constexpr const uint8_t btb_bucket_size = 62 - key_size;  // = target pc + tag

    /* Two-bit dynamic branch predictor:
     *    STRONGLY_NOT_TAKEN = 00,
     *    WEAKLY_NOT_TAKEN = 01,
     *    WEAKLY_TAKEN = 10,
     *    STRONGLY_TAKEN = 11
     * Default if WEAKLY_NOT_TAKEN
     */
    std::bitset<2> calcPrediction(std::bitset<bht_bucket_size> bht_bucket, bool comp);

    void updatePrediction(std::bitset<bht_bucket_size> &bht_bucket, uint32_t key, bool comp);

    void setupBucket(std::bitset<bht_bucket_size> &bht_bucket, std::bitset<btb_bucket_size> &btb_bucket,
                     uint32_t key, bool comp, std::bitset<32> target, std::bitset<30 - key_size> tag);

    void swapBuckets(uint32_t key);

    // Branch history table with 2-way associative cache
    std::map<uint32_t, std::pair<std::bitset<bht_bucket_size>, std::bitset<bht_bucket_size>>> bht_;
    // Branch target buffer with 2-way associative cache
    std::map<uint32_t, std::pair<std::bitset<btb_bucket_size>, std::bitset<btb_bucket_size>>> btb_;
};

#endif // UNITS_BRANCH_PREDICTION_H
