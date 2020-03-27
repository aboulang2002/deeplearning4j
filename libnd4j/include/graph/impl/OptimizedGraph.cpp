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

#include <graph/OptimizedGraph.h>

namespace sd {
    namespace graph {
        OptimizedGraph::OptimizedGraph(const OptimizedGraph &other) noexcept {
            _onion = other._onion;
        }

        OptimizedGraph &OptimizedGraph::operator=(const OptimizedGraph &other) noexcept {
            if (this == &other)
                return *this;

            _onion = other._onion;

            return *this;
        }

        OptimizedGraph::OptimizedGraph(OptimizedGraph &&other) noexcept {
            _onion = std::move(other._onion);
        }

        OptimizedGraph &OptimizedGraph::operator=(OptimizedGraph &&other) noexcept {
            if (this == &other)
                return *this;

            _onion = std::move(other._onion);

            return *this;
        }

        uint64_t OptimizedGraph::layers() const {
            return _onion.size();
        }

        const std::vector<OpSequence> &OptimizedGraph::layer(uint64_t index) const {
            return _onion.at(index);
        }

        void OptimizedGraph::append(const std::vector<OpSequence> &layer) {
            _onion[_onion.size()] = layer;
        }

        void OptimizedGraph::append(OpSequence &sequence) {
            append(std::vector<OpSequence>{sequence});
        }
    }
}