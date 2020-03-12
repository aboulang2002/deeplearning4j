/*******************************************************************************
 * Copyright (c) 2015-2019 Skymind, Inc.
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
#include <ops/ops.h>
#include <helpers/GradCheck.h>
#include <array>


using namespace sd;


class DeclarableOpsTests18 : public testing::Test {
public:

    DeclarableOpsTests18() {
    }
};

TEST_F(DeclarableOpsTests18, test_bitcast_1) {
    auto x = NDArrayFactory::create<double>(0.23028551377579154);
    auto z = NDArrayFactory::create<Nd4jLong>(0);
    auto e = NDArrayFactory::create<Nd4jLong>(4597464930322771456L);

    sd::ops::bitcast op;
    auto status = op.execute({&x}, {&z}, {}, {(Nd4jLong) sd::DataType::INT64}, {});
    ASSERT_EQ(Status::OK(), status);

    ASSERT_EQ(e, z);
}

TEST_F(DeclarableOpsTests18, test_tanh_1) {
    auto x = NDArrayFactory::create<float>('c', {8}, {0.23f, -0.23f, 0.35f, -0.35f, 0.64f, -0.64f, 100000.f, -100000.f});
    auto z = x.ulike();
    auto e = NDArrayFactory::create<float>('c', {8}, {0.226028f, -0.226028f, 0.336376f, -0.336376f, 0.564900f, -0.564900f, 1.f, -1.f});

    sd::ops::tanh op;
    op.execute({&x}, {&z});

    ASSERT_EQ(e, z);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests18, test_tanh_2) {

    NDArray x('c', { 2, 2, 3, 3, 4, 4 }, sd::DataType::FLOAT32);
    NDArray z('c', { 2, 2, 3, 3, 4, 4 }, sd::DataType::FLOAT32);

    x.linspace(-1., 0.003);

    NDArray e('c', { 2, 2, 3, 3, 4, 4 }, { -0.761594, -0.760331, -0.759063, -0.757788, -0.756508, -0.755222, -0.753930, -0.752633, -0.751329, -0.750020, -0.748704, -0.747383, -0.746056, -0.744723, -0.743383, -0.742038, -0.740687, -0.739330, -0.737967, -0.736598, -0.735222, -0.733841, -0.732453, -0.731060, -0.729660, -0.728254, -0.726842, -0.725424, -0.724000, -0.722569, -0.721132, -0.719689, -0.718240, -0.716784, -0.715323, -0.713854, -0.712380, -0.710899, -0.709412, -0.707919, -0.706419, -0.704913, -0.703401, -0.701882, -0.700357, -0.698825, -0.697287, -0.695742, -0.694191, -0.692634, -0.691069, -0.689499, -0.687922, -0.686338, -0.684748, -0.683152, -0.681548, -0.679939, -0.678322, -0.676699, -0.675070, -0.673434, -0.671791, -0.670142, -0.668486, -0.666823, -0.665153, -0.663477, -0.661795, -0.660105, -0.658409, -0.656706, -0.654997, -0.653280, -0.651557, -0.649827, -0.648091, -0.646348, -0.644597, -0.642841, -0.641077, -0.639306, -0.637529, -0.635745, -0.633954, -0.632157, -0.630352, -0.628541, -0.626722, -0.624897, -0.623065, -0.621227, -0.619381, -0.617528, -0.615669, -0.613803, -0.611929, -0.610049, -0.608162, -0.606269, -0.604368, -0.602460, -0.600546, -0.598624, -0.596696, -0.594760, -0.592818, -0.590869, -0.588913, -0.586950, -0.584980, -0.583003, -0.581019, -0.579029, -0.577031, -0.575026, -0.573015, -0.570996, -0.568971, -0.566939, -0.564900, -0.562853, -0.560800, -0.558740, -0.556674, -0.554600, -0.552519, -0.550431, -0.548337, -0.546235, -0.544127, -0.542012, -0.539890, -0.537761, -0.535625, -0.533482, -0.531332, -0.529176, -0.527013, -0.524842, -0.522665, -0.520482, -0.518291, -0.516093, -0.513889, -0.511678, -0.509460, -0.507235, -0.505004, -0.502765, -0.500520, -0.498268, -0.496010, -0.493745, -0.491472, -0.489194, -0.486908, -0.484616, -0.482318, -0.480012, -0.477700, -0.475381, -0.473056, -0.470724, -0.468385, -0.466040, -0.463689, -0.461330, -0.458966, -0.456594, -0.454216, -0.451832, -0.449441, -0.447044, -0.444640, -0.442230, -0.439814, -0.437391, -0.434962, -0.432526, -0.430084, -0.427636, -0.425181, -0.422721, -0.420254, -0.417780, -0.415301, -0.412815, -0.410323, -0.407825, -0.405321, -0.402811, -0.400295, -0.397773, -0.395244, -0.392710, -0.390170, -0.387623, -0.385071, -0.382513, -0.379949, -0.377379, -0.374803, -0.372222, -0.369635, -0.367042, -0.364443, -0.361839, -0.359229, -0.356613, -0.353992, -0.351365, -0.348732, -0.346095, -0.343451, -0.340802, -0.338148, -0.335488, -0.332823, -0.330153, -0.327477, -0.324796, -0.322110, -0.319419, -0.316723, -0.314021, -0.311314, -0.308602, -0.305886, -0.303164, -0.300437, -0.297705, -0.294969, -0.292227, -0.289481, -0.286730, -0.283975, -0.281214, -0.278449, -0.275679, -0.272905, -0.270126, -0.267343, -0.264555, -0.261763, -0.258966, -0.256165, -0.253360, -0.250550, -0.247737, -0.244919, -0.242097, -0.239270, -0.236440, -0.233606, -0.230768, -0.227925, -0.225079, -0.222229, -0.219376, -0.216518, -0.213657, -0.210792, -0.207923, -0.205051, -0.202176, -0.199297, -0.196414, -0.193528, -0.190639, -0.187746, -0.184850, -0.181951, -0.179049, -0.176144, -0.173235, -0.170324, -0.167409, -0.164492, -0.161572, -0.158649, -0.155723, -0.152794, -0.149863, -0.146929, -0.143992, -0.141053, -0.138112, -0.135168, -0.132221, -0.129273, -0.126322, -0.123368, -0.120413, -0.117455, -0.114496, -0.111534, -0.108570, -0.105605, -0.102637, -0.099668, -0.096697, -0.093724, -0.090750, -0.087774, -0.084796, -0.081817, -0.078836, -0.075854, -0.072871, -0.069886, -0.066900, -0.063913, -0.060924, -0.057935, -0.054945, -0.051953, -0.048961, -0.045968, -0.042974, -0.039979, -0.036983, -0.033987, -0.030990, -0.027993, -0.024995, -0.021996, -0.018998, -0.015999, -0.012999, -0.010000, -0.007000, -0.004000, -0.001000, 0.002000, 0.005000, 0.008000, 0.011000, 0.013999, 0.016998, 0.019997, 0.022996, 0.025994, 0.028992, 0.031989, 0.034986, 0.037982, 0.040977, 0.043972, 0.046965, 0.049958, 0.052950, 0.055942, 0.058932, 0.061921, 0.064909, 0.067895, 0.070881, 0.073865, 0.076848, 0.079830, 0.082810, 0.085789, 0.088766, 0.091741, 0.094715, 0.097687, 0.100658, 0.103627, 0.106594, 0.109558, 0.112521, 0.115482, 0.118441, 0.121398, 0.124353, 0.127305, 0.130256, 0.133204, 0.136149, 0.139092, 0.142033, 0.144971, 0.147907, 0.150840, 0.153771, 0.156698, 0.159623, 0.162545, 0.165465, 0.168381, 0.171294, 0.174205, 0.177112, 0.180017, 0.182918, 0.185816, 0.188711, 0.191602, 0.194490, 0.197375, 0.200257, 0.203135, 0.206009, 0.208880, 0.211747, 0.214611, 0.217471, 0.220327, 0.223180, 0.226028, 0.228873, 0.231714, 0.234551, 0.237384, 0.240213, 0.243038, 0.245858, 0.248675, 0.251487, 0.254296, 0.257099, 0.259899, 0.262694, 0.265485, 0.268271, 0.271053, 0.273830, 0.276603, 0.279371, 0.282135, 0.284894, 0.287648, 0.290397, 0.293142, 0.295882, 0.298617, 0.301347, 0.304072, 0.306792, 0.309507, 0.312217, 0.314922, 0.317622, 0.320317, 0.323006, 0.325691, 0.328370, 0.331044, 0.333712, 0.336376, 0.339033, 0.341686, 0.344333, 0.346974, 0.349611, 0.352241, 0.354866, 0.357485, 0.360099, 0.362707, 0.365310, 0.367907, 0.370498, 0.373083, 0.375663, 0.378236, 0.380804, 0.383366, 0.385922, 0.388473, 0.391017, 0.393555, 0.396088, 0.398614, 0.401134, 0.403649, 0.406157, 0.408659, 0.411155, 0.413644, 0.416128, 0.418605, 0.421077, 0.423542, 0.426000, 0.428453, 0.430899, 0.433339, 0.435772, 0.438199, 0.440620, 0.443034, 0.445442, 0.447844, 0.450239, 0.452628, 0.455010, 0.457385, 0.459755, 0.462117, 0.464473, 0.466823, 0.469166, 0.471502, 0.473832, 0.476155, 0.478471, 0.480781, 0.483085, 0.485381, 0.487671, 0.489954, 0.492231, 0.494500, 0.496763, 0.499020, 0.501269, 0.503512, 0.505748, 0.507977, 0.510200, 0.512416, 0.514624, 0.516827, 0.519022, 0.521210, 0.523392, 0.525567, 0.527735, 0.529896, 0.532050, 0.534197, 0.536338, 0.538471, 0.540598, 0.542718, 0.544831, 0.546937, 0.549036, 0.551128, 0.553213, 0.555292, 0.557363, 0.559428, 0.561486, 0.563536, 0.565580, 0.567617, 0.569647, 0.571670, 0.573686, 0.575695, 0.577697, 0.579693, 0.581681, 0.583663, 0.585637, 0.587605, 0.589566, 0.591519, 0.593466, 0.595406, 0.597339, 0.599265, 0.601184, 0.603097, 0.605002, 0.606901, 0.608792, 0.610677, 0.612555, 0.614425, 0.616289, 0.618147, 0.619997 }, sd::DataType::FLOAT32);

    sd::ops::tanh op;
    op.execute({ &x }, { &z });
    ASSERT_EQ(e, z);
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests18, TestSoftMax_bp_TEST) {

    NDArray input('c', { 2, 2 }, { 1,2,3,4 }, DataType::FLOAT32);
    NDArray epsilon('c', { 2, 2 }, { .1, .2, .3, .4 }, DataType::FLOAT32);

    int axis = 1;

    NDArray output('c', { 2, 2 }, DataType::FLOAT32);

    NDArray exp('c', { 2, 2 }, { -0.019661, 0.019661, -0.019661, 0.019661 }, DataType::FLOAT32);

    sd::ops::softmax_bp op;

    Nd4jStatus status = op.execute({ &input, &epsilon }, { &output }, {}, { axis });
    ASSERT_EQ(ND4J_STATUS_OK, status);
    ASSERT_TRUE(output.equalsTo(exp));

}
//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests18, TestSoftMax_bp_TEST2) {

    NDArray input('c', { 4, 5, 2, 3 }, DataType::FLOAT32);
    NDArray epsilon('c', { 4, 5, 2, 3 }, { -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855 }, DataType::FLOAT32);
    input.linspace(0.1, 0.2);

    int axis = -1;

    NDArray output('c', { 4, 5, 2, 3 }, DataType::FLOAT32);
    NDArray exp('c', { 4, 5, 2, 3 }, { -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253, -0.009387, -0.002866, 0.012253 }, DataType::FLOAT32);

    sd::ops::softmax_bp op;

    Nd4jStatus status = op.execute({ &input, &epsilon }, { &output }, {}, { axis });
    ASSERT_EQ(ND4J_STATUS_OK, status);
    ASSERT_TRUE(output.equalsTo(exp));

}
//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests18, TestSoftMax_bp_TEST3) {

    NDArray input('f', { 4, 5, 2, 3 }, DataType::FLOAT32);
    NDArray epsilon('f', { 4, 5, 2, 3 }, { -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855, -0.030498, -0.004357, 0.034855 }, DataType::FLOAT32);
    input.linspace(-5., 0.5);

    int axis = 1;

    NDArray output('f', { 4, 5, 2, 3 }, DataType::FLOAT32);
    NDArray expC('c', { 4, 5, 2, 3 }, { -0.0, -0.0, 0.0, 0.0, -0.0, -0.0, 0.0, 0.0, -0.0, -0.0, 0.0, 0.0, 0.000095, -0.000149, 0.000054, 0.000054, 0.000095, -0.000149, -0.001183, -0.001760, 0.002943, 0.002943, -0.001183, -0.001760, 0.001088, 0.001909, -0.002997, -0.002997, 0.001088, 0.001909, -0.000000, 0.000000, -0.000000, -0.000000, -0.000000, 0.000000, 0.000000, -0.000000, 0.000000, 0.000000, 0.000000, -0.000000, -0.000149, 0.000054, 0.000095, 0.000095, -0.000149, 0.000054, -0.001760, 0.002943, -0.001183, -0.001183, -0.001760, 0.002943, 0.001909, -0.002997, 0.001088, 0.001088, 0.001909, -0.002997, 0.000000, -0.000000, -0.000000, -0.000000, 0.000000, -0.000000, -0.000000, 0.000000, 0.000000, 0.000000, -0.000000, 0.000000, 0.000054, 0.000095, -0.000149, -0.000149, 0.000054, 0.000095, 0.002943, -0.001183, -0.001760, -0.001760, 0.002943, -0.001183, -0.002997, 0.001088, 0.001909, 0.001909, -0.002997, 0.001088, -0.000000, -0.000000, 0.000000, 0.000000, -0.000000, -0.000000, 0.000000, 0.000000, -0.000000, -0.000000, 0.000000, 0.000000, 0.000095, -0.000149, 0.000054, 0.000054, 0.000095, -0.000149, -0.001183, -0.001760, 0.002943, 0.002943, -0.001183, -0.001760, 0.001088, 0.001909, -0.002997, -0.002997, 0.001088, 0.001909 }, DataType::FLOAT32);

    NDArray exp('f', { 4, 5, 2, 3 }, DataType::FLOAT32);
    exp.assign(expC);

    sd::ops::softmax_bp op;

    Nd4jStatus status = op.execute({ &input, &epsilon }, { &output }, {}, { axis });
    ASSERT_EQ(ND4J_STATUS_OK, status);
    ASSERT_TRUE(output.equalsTo(exp));

}
