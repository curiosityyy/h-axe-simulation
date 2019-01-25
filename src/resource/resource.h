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

#include <vector>

#include "glog/logging.h"
#include "nlohmann/json.hpp"

namespace axe {
namespace simulation {

using nlohmann::json;

enum ResourceType { kCPU = 0, kMemory, kDisk, kNetwork };

const int kNumResourceTypes = 4;

class ResourcePack {
public:
  ResourcePack() {
    resource_.resize(kNumResourceTypes);
    for (int i = 0; i < resource_.size(); ++i) {
      resource_[i] = 0.0;
    }
  }

  ResourcePack(double cpu, double memory, double disk, double network) {
    resource_.resize(kNumResourceTypes);
    resource_[kCPU] = cpu;
    resource_[kMemory] = memory;
    resource_[kDisk] = disk;
    resource_[kNetwork] = network;
  }

  ResourcePack(std::vector<double> resource) {
    CHECK_EQ(resource.size(), kNumResourceTypes)
        << "invalid resource vector!!!";
    resource_ = resource;
  }

  void Print() {
    DLOG(INFO) << " cpu = " << resource_[kCPU];
    DLOG(INFO) << " memory = " << resource_[kMemory];
    DLOG(INFO) << " disk = " << resource_[kDisk];
    DLOG(INFO) << " network = " << resource_[kNetwork];
  }

  double GetCPU() const { return resource_[kCPU]; }
  double GetMemory() const { return resource_[kMemory]; }
  double GetDisk() const { return resource_[kDisk]; }
  double GetNetwork() const { return resource_[kNetwork]; }
  std::vector<double> GetResourceVector() const { return resource_; }
  double GetResourceByIndex(int idx) const { return resource_[idx]; }

  void SetCPU(double cpu) { resource_[kCPU] = cpu; }
  void SetMemory(double memory) { resource_[kMemory] = memory; }
  void SetDisk(double disk) { resource_[kDisk] = disk; }
  void SetNetwork(double network) { resource_[kNetwork] = network; }

  ResourcePack Add(const ResourcePack &rhs) const {
    ResourcePack result;
    result.SetCPU(resource_[kCPU] + rhs.GetCPU());
    result.SetMemory(resource_[kMemory] + rhs.GetMemory());
    result.SetDisk(resource_[kDisk] + rhs.GetDisk());
    result.SetNetwork(resource_[kNetwork] + rhs.GetNetwork());
    return result;
  }

  ResourcePack Subtract(const ResourcePack &rhs) const {
    ResourcePack result = SubtractWithoutMemory(rhs);
    result.SetMemory(resource_[kMemory] - rhs.GetMemory());
    return result;
  }

  ResourcePack SubtractWithoutMemory(const ResourcePack &rhs) const {
    ResourcePack result;
    result.SetCPU(resource_[kCPU] - rhs.GetCPU());
    result.SetDisk(resource_[kDisk] - rhs.GetDisk());
    result.SetNetwork(resource_[kNetwork] - rhs.GetNetwork());
    return result;
  }

  void AddToMe(const ResourcePack &rhs) {
    resource_[kCPU] += rhs.GetCPU();
    resource_[kMemory] += rhs.GetMemory();
    resource_[kDisk] += rhs.GetDisk();
    resource_[kNetwork] += rhs.GetNetwork();
  }

  void SubtractFromMe(const ResourcePack &rhs) {
    SubtractFromMeWithoutMemory(rhs);
    resource_[kMemory] -= rhs.GetMemory();
  }

  void SubtractFromMeWithoutMemory(const ResourcePack &rhs) {
    resource_[kCPU] -= rhs.GetCPU();
    resource_[kDisk] -= rhs.GetDisk();
    resource_[kNetwork] -= rhs.GetNetwork();
  }

  double DotProduct(const ResourcePack &rhs) {
    double product = 0;
    for (int i = 0; i < kNumResourceTypes; ++i) {
      product += resource_[i] * rhs.GetResourceByIndex(i);
    }
    return product;
  }

  bool FitIn(const ResourcePack &resource) {
    bool ret = true;
    for (int i = 0; i < kNumResourceTypes; ++i) {
      ret = ret && (resource.GetResourceByIndex(i) > resource_[i]);
    }
  }

  bool WeakFitIn(const ResourcePack &resource, double alpha) {
    bool ret = true;
    for (int i = 0; i < kNumResourceTypes; ++i) {
      if (i == kMemory)
        ret = ret && (resource.GetResourceByIndex(i) > resource_[i]);
      else
        ret = ret && (resource.GetResourceByIndex(i) > resource_[i] * alpha);
    }
  }

  friend void from_json(const json &j, ResourcePack &resource_pack) {
    resource_pack.resource_.resize(kNumResourceTypes);
    j.at("cpu").get_to(resource_pack.resource_[kCPU]);
    j.at("memory").get_to(resource_pack.resource_[kMemory]);
    j.at("disk").get_to(resource_pack.resource_[kDisk]);
    j.at("network").get_to(resource_pack.resource_[kNetwork]);
  }

private:
  std::vector<double> resource_;
};
} // namespace simulation
} // namespace axe
