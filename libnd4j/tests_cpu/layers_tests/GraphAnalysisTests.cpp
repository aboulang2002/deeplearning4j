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

#include "testlayers.h"
#include <ops/declarable/CustomOperations.h>
#include <array/NDArray.h>
#include <legacy/NativeOps.h>
#include <fstream>
#include <graph/Graph.h>

using namespace sd;
using namespace sd::graph;

class GraphAnalysisTests : public testing::Test {
public:
    GraphAnalysisTests() {
        ///
    }
};

TEST_F(GraphAnalysisTests, basic_toposort_test_1) {
    Graph graph;

    // A
    graph.addVariable("A", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    // B
    graph.addVariable("B", NDArrayFactory::create<int>('c', {3}, {2, 2, 2}));

    // C
    graph.addVariable("C", NDArrayFactory::create<int>('c', {3}, {3, 3, 3}));

    Node a(sd::ops::multiply(), "multiply");
    Node b(sd::ops::add(), "add");

    graph.addNode(a, {"A", "B"});
    graph.addNode(b, {"multiply", "C"});

    // we just check that nodes were really added
    ASSERT_EQ(2, graph.size());

    auto optimized = graph.optimizedGraph();

    // we expect that OptimizedGraph has exactly 1 layer
    ASSERT_EQ(1, optimized.layers());

    auto layer = optimized.layer(0);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(1, layer.width());
    auto sequence = layer[0];

    // we expect that OpSequence has exactly 2 ops
    ASSERT_EQ(2, sequence.length());

    ASSERT_EQ(4, sequence.at(0).protoContext().nodeId());
    ASSERT_EQ(5, sequence.at(1).protoContext().nodeId());
}

TEST_F(GraphAnalysisTests, basic_toposort_test_2) {
    Graph graph;

    // A
    graph.addVariable("A", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    // B
    graph.addVariable("B", NDArrayFactory::create<int>('c', {3}, {2, 2, 2}));

    // C
    graph.addVariable("C", NDArrayFactory::create<int>('c', {3}, {3, 3, 3}));

    // D
    graph.addVariable("D", NDArrayFactory::create<int>('c', {3}, {4, 4, 4}));

    Node a(sd::ops::multiply(), "multiply");
    Node b(sd::ops::add(), "add");
    Node c(sd::ops::subtract(), "subtract");


    graph.addNode(a, {"A", "B"});
    graph.addNode(b, {"multiply", "C"});
    graph.addNode(c, {"multiply", "D"});

    // we just check that nodes were really added
    ASSERT_EQ(3, graph.size());

    auto optimized = graph.optimizedGraph();

    // we expect that OptimizedGraph has exactly 2 layers
    ASSERT_EQ(2, optimized.layers());

    // checking first layer first
    auto layer0 = optimized.layer(0);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(1, layer0.width());
    auto sequence = layer0[0];

    // we expect that OpSequence has exactly 2 ops
    ASSERT_EQ(1, sequence.length());

    ASSERT_EQ(5, sequence.at(0).protoContext().nodeId());

    // checking second layer now
    auto layer1 = optimized.layer(1);

    // we expect layer has exactly 2 OpSequences
    ASSERT_EQ(2, layer1.width());

    sequence = layer1[0];

    ASSERT_EQ(1, sequence.length());
    ASSERT_EQ(6, sequence.at(0).protoContext().nodeId());

    sequence = layer1[1];

    ASSERT_EQ(1, sequence.length());
    ASSERT_EQ(7, sequence.at(0).protoContext().nodeId());
}

TEST_F(GraphAnalysisTests, basic_toposort_test_3) {
    Graph graph;

    // A
    graph.addVariable("A", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    // B
    graph.addVariable("B", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    // C
    graph.addVariable("C", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    // D
    graph.addVariable("D", NDArrayFactory::create<int>('c', {3}, {1, 1, 1}));

    Node a(sd::ops::multiply(), "multiply");
    Node b(sd::ops::add(), "add");
    Node c(sd::ops::subtract(), "subtract");
    Node d(sd::ops::add(), "add2");
    Node e(sd::ops::multiply(), "multiply2");

    graph.addNode(a, {"A", "B"});
    graph.addNode(b, {"multiply", "C"});

    graph.addNode(c, {"add", "D"});
    graph.addNode(d, {"add", "D"});

    graph.addNode(e, {"subtract", "add2"});

    // we just check that nodes were really added
    ASSERT_EQ(5, graph.size());

    auto optimized = graph.optimizedGraph();

    // we expect that OptimizedGraph has exactly 1 layer
    ASSERT_EQ(3, optimized.layers());

    // checking first layer first
    auto layer0 = optimized.layer(0);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(1, layer0.width());
    auto sequence = layer0[0];

    // we expect that OpSequence has exactly 2 ops
    ASSERT_EQ(2, sequence.length());

    ASSERT_EQ(5, sequence.at(0).protoContext().nodeId());
    ASSERT_EQ(6, sequence.at(1).protoContext().nodeId());

    // checking second layer now
    auto layer1 = optimized.layer(1);

    // we expect layer has exactly 2 OpSequences
    ASSERT_EQ(2, layer1.width());

    sequence = layer1[0];

    ASSERT_EQ(1, sequence.length());
    ASSERT_EQ(7, sequence.at(0).protoContext().nodeId());

    sequence = layer1[1];

    ASSERT_EQ(1, sequence.length());
    ASSERT_EQ(8, sequence.at(0).protoContext().nodeId());


    // checking last layer
    auto layer2 = optimized.layer(2);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(1, layer2.width());
    sequence = layer2[0];

    // we expect that OpSequence has exactly 1 ops
    ASSERT_EQ(1, sequence.length());

    ASSERT_EQ(9, sequence.at(0).protoContext().nodeId());
}

// currently does not work correctly
TEST_F(GraphAnalysisTests, DISABLED_basic_toposort_test_4) {
    Graph graph;

    // A
    graph.addVariable("A", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));

    // B
    graph.addVariable("B", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));

    // C
    graph.addVariable("C", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));

    // D
    graph.addVariable("D", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));

    // E
    graph.addVariable("E", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));

    // F
    graph.addVariable("F", NDArrayFactory::create<int>('c', { 3 }, { 1, 1, 1 }));


    Node a1(sd::ops::multiply(), "a1");
    Node a2(sd::ops::add(), "a2");

    Node b1(sd::ops::subtract(), "b1");
    Node b2(sd::ops::add(), "b2");
    Node b3(sd::ops::multiply(), "b3");

    Node d1(sd::ops::multiply(), "d1");
    Node d2(sd::ops::add(), "d2");

    Node e(sd::ops::subtract(), "e");

    graph.addNode(a1, { "A", "B" });
    graph.addNode(a2, { "C", "D" });

    graph.addNode(b1, { "a1", "E" });
    graph.addNode(b2, { "a1", "a2" });
    graph.addNode(b3, { "a2", "F" });

    graph.addNode(d1, { "b1", "b2" });
    graph.addNode(d2, { "b3", "b2" });

    graph.addNode(e, { "d1", "d2" });

    // we just check that nodes were really added
    ASSERT_EQ(8, graph.size());

    auto optimized = graph.optimizedGraph();

    // we expect that OptimizedGraph has exactly 1 layer
    ASSERT_EQ(4, optimized.layers());

    // checking first layer first
    auto layer0 = optimized.layer(0);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(2, layer0.width());
    auto sequence = layer0[0];

    // we expect that OpSequence has exactly 2 ops
    ASSERT_EQ(1, sequence.length());

    sequence = layer0[1];

    // we expect that OpSequence has exactly 2 ops
    ASSERT_EQ(1, sequence.length());

    // ASSERT_EQ(5, sequence.at(0).protoContext().nodeId());
    // ASSERT_EQ(6, sequence.at(1).protoContext().nodeId());

    // checking second layer now
    auto layer1 = optimized.layer(1);

    // we expect layer has exactly 2 OpSequences
    ASSERT_EQ(3, layer1.width());

    sequence = layer1[0];

    ASSERT_EQ(1, sequence.length());
    // ASSERT_EQ(7, sequence.at(0).protoContext().nodeId());

    sequence = layer1[1];

    ASSERT_EQ(1, sequence.length());
    // ASSERT_EQ(8, sequence.at(0).protoContext().nodeId());
    sequence = layer1[2];

    ASSERT_EQ(1, sequence.length());

    auto layer2 = optimized.layer(2);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(2, layer2.width());
    sequence = layer2[0];

    // we expect that OpSequence has exactly 1 ops
    ASSERT_EQ(1, sequence.length());

    // ASSERT_EQ(9, sequence.at(0).protoContext().nodeId());

    sequence = layer2[1];

    // we expect that OpSequence has exactly 1 ops
    ASSERT_EQ(1, sequence.length());
    // ASSERT_EQ(9, sequence.at(0).protoContext().nodeId());

    // checking last layer
    auto layer3 = optimized.layer(3);

    // we expect layer has exactly 1 OpSequence
    ASSERT_EQ(1, layer2.width());
    sequence = layer2[0];

    // we expect that OpSequence has exactly 1 ops
    ASSERT_EQ(1, sequence.length());
    // ASSERT_EQ(9, sequence.at(0).protoContext().nodeId());
}
