// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "crash_report.h"

namespace Core {
  
#ifdef ENABLE_CRASH_REPORTS
  static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
    [[maybe_unused]] void* context, bool succeeded) {
    printf("Dump path: %s\n", descriptor.path());
    return succeeded;
  }
  
  CrashReport::CrashReport(std::string path, [[maybe_unused]] std::string pipe /*= ""*/) : _exception_handler(google_breakpad::MinidumpDescriptor(path), NULL, dumpCallback, NULL, true, -1)
  {
  }
#else
  CrashReport::CrashReport([[maybe_unused]] std::string path, [[maybe_unused]] std::string pipe /*= ""*/)
  {
  }
#endif
}