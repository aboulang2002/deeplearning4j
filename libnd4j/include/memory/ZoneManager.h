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

#ifndef SD_ZONEMANAGER_H
#define SD_ZONEMANAGER_H

#include <memory/MemoryZone.h>
#include <cstdint>

namespace sd {
    /**
     * Abstract class that defines common methods for zone managers
     */
    class ZoneManager {
    public:
        ZoneManager() = default;
        ~ZoneManager() = default;

        /**
         * This method returns id of the current zone served by this manager instance
         * @return MemoryZone enum
         */
        virtual MemoryZone zone() const = 0;

        /**
         * This method returns amount of memory available in this zone
         * @return number of bytes
         */
        virtual uint64_t available() const = 0;

        /**
         * This method returns amount of memory currently used in this zone
         * @return number of bytes
         */
        virtual uint64_t used() const = 0;
    };
}


#endif //SD_ZONEMANAGER_H