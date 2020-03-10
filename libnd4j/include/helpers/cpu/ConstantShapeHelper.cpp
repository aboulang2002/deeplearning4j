/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
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
//  @author raver119@gmail.com
//

#ifndef __CUDABLAS__

#include <helpers/ConstantShapeHelper.h>
#include <helpers/logger.h>
#include <helpers/ShapeBuilders.h>
#include <helpers/ShapeUtils.h>

namespace sd {
    ConstantShapeHelper::ConstantShapeHelper() {
        _cache.resize(32);
        for (int e = 0; e < 32; e++) {
            MAP_IMPL<ShapeDescriptor, ConstantDataBuffer> cache;
            _cache[e] = cache;
        }
    }

    ConstantShapeHelper* ConstantShapeHelper::getInstance() {
        if (!_INSTANCE)
            _INSTANCE = new ConstantShapeHelper();

        return _INSTANCE;
    }

    ConstantDataBuffer ConstantShapeHelper::bufferForShapeInfo(sd::DataType dataType, char order, const std::vector<Nd4jLong> &shape) {
        ShapeDescriptor descriptor(dataType, order, shape);
        return bufferForShapeInfo(descriptor);
    }

    ConstantDataBuffer ConstantShapeHelper::bufferForShapeInfo(const sd::DataType dataType, const char order, const int rank, const Nd4jLong* shape) {
        ShapeDescriptor descriptor(dataType, order, shape, rank);
        return bufferForShapeInfo(descriptor);
    }


    ConstantDataBuffer ConstantShapeHelper::bufferForShapeInfo(const ShapeDescriptor &descriptor) {
        int deviceId = 0;

        _mutex.lock();

        if (_cache[deviceId].count(descriptor) == 0) {
            auto hPtr = descriptor.toShapeInfo();
            ConstantDataBuffer buffer(hPtr, nullptr, shape::shapeInfoLength(hPtr)*sizeof(Nd4jLong), DataType::INT64);
            ShapeDescriptor descriptor1(descriptor);
            _cache[deviceId][descriptor1] = buffer;
            auto r = _cache[deviceId][descriptor1];
            _mutex.unlock();

            return r;
        } else {
            auto r = _cache[deviceId].at(descriptor);
            _mutex.unlock();

            return r;
        }
    }

    ConstantDataBuffer ConstantShapeHelper::bufferForShapeInfo(const Nd4jLong *shapeInfo) {
        ShapeDescriptor descriptor(shapeInfo);
        return bufferForShapeInfo(descriptor);
    }

    bool ConstantShapeHelper::checkBufferExistenceForShapeInfo(ShapeDescriptor &descriptor) {
        bool result;
        int deviceId = 0;
        _mutex.lock();

        if (_cache[deviceId].count(descriptor) == 0)
            result = false;
        else
            result = true;

        _mutex.unlock();

        return result;
    }

    Nd4jLong* ConstantShapeHelper::createShapeInfo(const sd::DataType dataType, const char order, const int rank, const Nd4jLong* shape) {
        ShapeDescriptor descriptor(dataType, order, shape, rank);
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::createShapeInfo(const sd::DataType dataType, const Nd4jLong* shapeInfo) {
        return ConstantShapeHelper::createShapeInfo(dataType, shape::order(shapeInfo), shape::rank(shapeInfo), shape::shapeOf(const_cast<Nd4jLong*>(shapeInfo)));
    }

    Nd4jLong* ConstantShapeHelper::emptyShapeInfo(const sd::DataType dataType) {
        auto descriptor = ShapeDescriptor::emptyDescriptor(dataType);
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::scalarShapeInfo(const sd::DataType dataType) {
        auto descriptor = ShapeDescriptor::scalarDescriptor(dataType);
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::vectorShapeInfo(const Nd4jLong length, const sd::DataType dataType) {
        auto descriptor = ShapeDescriptor::vectorDescriptor(length, dataType);
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::createShapeInfo(const sd::DataType dataType, const char order, const std::vector<Nd4jLong> &shape) {
        ShapeDescriptor descriptor(dataType, order, shape);
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::createShapeInfo(const ShapeDescriptor &descriptor) {
        return bufferForShapeInfo(descriptor).primaryAsT<Nd4jLong>();
    }

    Nd4jLong* ConstantShapeHelper::createFromExisting(Nd4jLong *shapeInfo, bool destroyOriginal) {
        ShapeDescriptor descriptor(shapeInfo);
        auto result = createShapeInfo(descriptor);

        if (destroyOriginal)
            RELEASE(shapeInfo, nullptr)

        return result;
    }

    Nd4jLong* ConstantShapeHelper::createFromExisting(Nd4jLong *shapeInfo, sd::memory::Workspace *workspace) {
        ShapeDescriptor descriptor(shapeInfo);
        auto result = createShapeInfo(descriptor);

        RELEASE(shapeInfo, workspace);

        return result;
    }


////////////////////////////////////////////////////////////////////////
ConstantDataBuffer ConstantShapeHelper::createShapeInfoWithUnitiesForBroadcast(const Nd4jLong* maxShapeInfo, const Nd4jLong* minShapeInfo, sd::memory::Workspace* workspace, const std::vector<int> dimensions) {

    Nd4jLong* newShapeInfo = nullptr;
    ALLOCATE(newShapeInfo, workspace, shape::shapeInfoLength(shape::rank(maxShapeInfo)), Nd4jLong);

    newShapeInfo[0] = shape::rank(maxShapeInfo);

    sd::ArrayOptions::copyDataType(newShapeInfo, minShapeInfo);                     // type
    newShapeInfo[2 * newShapeInfo[0] + 2] = shape::elementWiseStride(minShapeInfo); // ews
    newShapeInfo[2 * newShapeInfo[0] + 3] = shape::order(minShapeInfo);             // order

    if(!dimensions.empty()) {

        for(uint k = 0, j = 0, i = 0; i < shape::rank(maxShapeInfo); ++i) {

            if(j < dimensions.size() && dimensions[j] == i) {
                shape::shapeOf(newShapeInfo)[i] = shape::shapeOf(minShapeInfo)[k];
                shape::stride(newShapeInfo)[i]  = shape::stride(minShapeInfo)[k++];
                ++j;
            }
            else{
                shape::shapeOf(newShapeInfo)[i] = 1;
                shape::stride(newShapeInfo)[i]  = 0;
                if(shape::sizeAt(minShapeInfo, k) == 1 && dimensions.size() != shape::rank(minShapeInfo))
                    ++k;
            }
        }
    }
    else{

        for(int j = shape::rank(minShapeInfo) - 1, i = shape::rank(maxShapeInfo) - 1; i >=0 ; --i) {

            if(j >= 0) {
                shape::shapeOf(newShapeInfo)[i] = shape::shapeOf(minShapeInfo)[j];
                shape::stride(newShapeInfo)[i]  = shape::shapeOf(minShapeInfo)[j] == 1 ? 0 : shape::stride(minShapeInfo)[j];
                --j;
            }
            else {
                shape::shapeOf(newShapeInfo)[i] = 1;
                shape::stride(newShapeInfo)[i]  = 0;
            }
        }
    }

    ShapeDescriptor descriptor(newShapeInfo);

    RELEASE(newShapeInfo, workspace);

    return bufferForShapeInfo(descriptor);
}


sd::ConstantShapeHelper* sd::ConstantShapeHelper::_INSTANCE = 0;

}

#endif