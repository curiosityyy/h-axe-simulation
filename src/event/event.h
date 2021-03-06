// Copyright 2018 H-AXE
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cmath>
#include <cstdint>
#include <map>
#include <memory>

namespace axe {
namespace simulation {

enum class EventType : int {
  TASK_FINISH = 0,    // SCH tells JM the task req is finished, for JM
  RESOURCE_AVAILABLE, // JM tells SCH there are avaliable resources on workers,
                      // for SCH
  JOB_FINISH,         // JM tells SCH the job is finished, for SCH
  JOB_ADMISSION,      // SCH accepts the job, for JM
  PLACEMENT_DECISION, // SCH tells the JM about the scheduling decision, for JM
  NEW_TASK_REQ,       // JM sends the task req to SCH, for SCH
  NEW_JOB,            // JM sends SCH the new job, for SCH
};

const int kScheduler = -1;

/**
 * It is actually not elegant design because
 * you don not always need work id or task id.
 * I will try to optimize it later.
 */

class Event {
public:
  Event(EventType event_type, double time, int priority, int event_principal)
      : event_type_(event_type), time_(time), priority_(priority),
        event_principal_(event_principal), event_id_(event_num++) {}
  virtual ~Event(){};

  const double eps = 1e-6;

  inline EventType GetEventType() const { return event_type_; }
  inline int GetEventPrincipal() const { return event_principal_; }
  inline int GetEventID() const { return event_id_; }
  inline double GetTime() const { return time_; }
  inline int GetPriority() const { return priority_; }
  void SetTime(double time) { time_ = time; }

  bool operator<(const Event &rhs) const {
    if (std::fabs(time_ - rhs.GetTime()) > eps)
      return time_ < rhs.GetTime();
    else {
      if (event_type_ != rhs.GetEventType())
        return event_type_ < rhs.GetEventType();
      else
        return priority_ < rhs.GetPriority();
    }
  }

private:
  EventType event_type_;
  int event_principal_;
  double time_;
  int priority_;
  int event_id_;
  static int event_num;
};

} // namespace simulation
} // namespace axe
