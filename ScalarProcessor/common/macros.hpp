#ifndef COMMON_MACROS_H
#define COMMON_MACROS_H

#define ASSERT_STATE(Run)                           \
    state = (Run);                                  \
    if (state == PipelineState::ERR) {              \
        return PipelineState::ERR;                  \
    } else if (state == PipelineState::BREAK) {     \
        return PipelineState::OK;                   \
    }

#define CYCLE_CONTROL(prev, next)           \
    ++(prev);                               \
    if ((prev) - (next) < 1) {              \
        return;                             \
    }

#endif  // COMMON_MACROS_H
