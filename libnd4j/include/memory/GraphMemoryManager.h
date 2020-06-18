/*******************************************************************************
 * Copyright (c) 2020 Konduit K.K.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author raver119@gmail.com
//

#ifndef SD_GRAPHMEMORYMANAGER_H
#define SD_GRAPHMEMORYMANAGER_H

#include <memory/MemoryDescriptor.h>
#include <memory/MemoryZone.h>
#include <memory/ZoneManager.h>
#include <memory>
#include <map>
#include <array/PointerWrapper.h>
#include <vector>

using namespace sd::memory;

namespace sd {
namespace graph {
class GraphMemoryManager {
 protected:
  std::map<MemoryZone, ZoneManager *> _zones;

  mutable std::vector<std::shared_ptr<PointerWrapper>> _attached;
 public:
  GraphMemoryManager();
  ~GraphMemoryManager();

  /**
   * This method does allocation (probably) and returns structure that describes
   * it
   * @param numBytes - number of bytes to be allocated
   * @param zone - memory zone for allocation
   * @return
   */
  virtual MemoryDescriptor allocate(size_t numBytes, MemoryZone zone);

  /**
   * This method releases (probably) memory chunk described by given descriptor
   * @param descriptor
   */
  virtual void release(MemoryDescriptor &descriptor);

  /**
   * This method allows to store reference to certain memory regions and keep them alive as long as Graph is alive
   * @param ptr
   */
  virtual void track(const std::shared_ptr<PointerWrapper> &ptr) const;
};
}  // namespace graph
}  // namespace sd

#endif  // SD_GRAPHMEMORYMANAGER_H