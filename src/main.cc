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

#include "algorithm/algorithm_book.h"
#include "algorithm/task_placement.h"
#include "glog/logging.h"
#include "nlohmann/json.hpp"
#include "simulator.h"
#include "worker.h"
#include <fstream>
#include <string>

using json = nlohmann::json;

json ReadJsonFromFile(const std::string &file) {
  std::ifstream in(file, std::ios::in);
  CHECK(in.is_open()) << "Cannot open json file " << file;
  std::string ret;
  in.seekg(0, std::ios::end);
  ret.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&ret[0], ret.size());
  return json::parse(ret);
}

void PrintAlgorithm() {
  DLOG(INFO) << "Algorithm:";
  DLOG(INFO) << "TaskPlacement: "
             << axe::simulation::AlgorithmBook::GetTaskPlacementString();
}

void SetAlgorithm(const json &j) {
  DLOG(INFO) << "set alg";
  std::string task_placement_str;
  j.at("TaskPlacement").get_to(task_placement_str);
  axe::simulation::AlgorithmBook::Init(task_placement_str);
  PrintAlgorithm();
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  google::SetLogDestination(google::INFO, "log/SIMULATION.INFO");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  FLAGS_logbuflevel = -1;

  LOG(INFO) << "start simulation";
  axe::simulation::Simulator simulator(ReadJsonFromFile(argv[1]),
                                       ReadJsonFromFile(argv[2]));
  SetAlgorithm(ReadJsonFromFile(argv[3]));
  simulator.Init();
  simulator.Print();
  simulator.Serve();
  LOG(INFO) << "simulation end";

  google::FlushLogFiles(google::INFO);
  gflags::ShutDownCommandLineFlags();
  return 0;
}
