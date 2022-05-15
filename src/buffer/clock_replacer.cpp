//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.cpp
//
// Identification: src/buffer/clock_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/clock_replacer.h"
#include <stdio.h>

namespace bustub {

ClockReplacer::ClockReplacer(size_t num_pages) {
    ClockFrame cf = {-1, false, false};
    for(size_t i = 0; i < num_pages; i++) {
        clock_replacer.emplace_back(cf);
    }
    frame_size = 0;
    clock_hand = 0;
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
    while (Size() > 0) {
        if(!clock_replacer[clock_hand].ref_flag && !clock_replacer[clock_hand].is_unpin) {
            clock_hand++;
            if(clock_hand == clock_replacer.size()) clock_hand = 0;
        } else if (clock_replacer[clock_hand].ref_flag && clock_replacer[clock_hand].is_unpin) {
            clock_replacer[clock_hand].ref_flag = false;
            //clock_replacer[clock_hand].is_unpin = false;
            clock_hand++;
            if(clock_hand == clock_replacer.size()) clock_hand = 0;
        } else {
            clock_replacer[clock_hand].is_unpin = false;
            clock_hand++;
            *frame_id = clock_hand;
            if(clock_hand == clock_replacer.size()) clock_hand = 0;
            frame_size--;
            return true;
        }
    }
    return false; 
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    for(size_t i = 1; i < (Size() + 1); i++) {
        if (frame_id == clock_replacer[i].frame_id) {
            if (clock_replacer[i].is_unpin) {
                clock_replacer[i].ref_flag = false;
                clock_replacer[i].is_unpin = false;
                frame_size--;
            }
            return;
       }
    }
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    for(size_t i = 0; i < clock_replacer.size(); i++) {
        if (frame_id == clock_replacer[i].frame_id) {
            clock_replacer[i].ref_flag = true;
            if(!clock_replacer[i].is_unpin) {
                clock_replacer[i].is_unpin = true;
                frame_size++;
            }
            return;
       }
    }
    if (Size() < clock_replacer.size()) {
        clock_replacer[frame_size].frame_id = frame_id;
        clock_replacer[frame_size].is_unpin = true;
        clock_replacer[frame_size].ref_flag = true;
        frame_size++;
    }
}

size_t ClockReplacer::Size() { 
    return frame_size; 
}

}  // namespace bustub
