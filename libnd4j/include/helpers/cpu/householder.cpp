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
// Created by Yurii Shyrma on 18.12.2017
//

#include <helpers/householder.h>
#include <array/NDArrayFactory.h>

namespace sd {
namespace ops {
namespace helpers {


//////////////////////////////////////////////////////////////////////////
template <typename T>
NDArray Householder<T>::evalHHmatrix(const NDArray& x) {

	// input validation
	if(!x.isVector() && !x.isScalar())
		throw std::runtime_error("ops::helpers::Householder::evalHHmatrix method: input array must be vector or scalar!");

	const auto xLen = (int)x.lengthOf();

	NDArray w(x.ordering(), {xLen, 1}, x.dataType(), x.getContext());							// column-vector

	T coeff;
	T normX = x.reduceNumber(reduce::Norm2).t<T>(0);

	const auto xFirstElem = x.t<T>(0);

	if(normX*normX - xFirstElem * xFirstElem <= DataTypeUtils::min<T>() || xLen == 1) {

		normX = xFirstElem;
		coeff = 0.f;
		w = 0.f;
	}
	else {

		if(xFirstElem >= (T)0.f)
			normX = -normX;									// choose opposite sign to lessen roundoff error

		T u0 = xFirstElem - normX;
		coeff = -u0 / normX;
		w.assign(x / u0);
	}

	w.t<T>(0) = (T)1;

	NDArray identity(x.ordering(), {xLen, xLen}, x.dataType(), x.getContext());
	identity.setIdentity();																			// identity matrix

	return identity - mmul(w, w.transpose()) * coeff;
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
void Householder<T>::evalHHmatrixData(const NDArray& x, NDArray& tail, T& coeff, T& normX) {

	// input validation
	if(!x.isVector() && !x.isScalar())
		throw std::runtime_error("ops::helpers::Householder::evalHHmatrixData method: input array must be vector or scalar!");

	if(!x.isScalar() && x.lengthOf() != tail.lengthOf() + 1)
		throw std::runtime_error("ops::helpers::Householder::evalHHmatrixData method: input tail vector must have length less than unity compared to input x vector!");

	normX = x.reduceNumber(reduce::Norm2).t<T>(0);

	const auto xFirstElem = x.t<T>(0);

	if(normX*normX - xFirstElem * xFirstElem <= DataTypeUtils::min<T>() || x.lengthOf() == 1) {

		normX = xFirstElem;
		coeff = (T)0.f;
		tail = (T)0.f;
	}
	else {

		if(xFirstElem >= (T)0.f)
			normX = -normX;									// choose opposite sign to lessen roundoff error

		T u0 = xFirstElem - normX;
		coeff = -u0 / normX;

		if(x.isRowVector())
			tail.assign(x({0,0, 1,-1}) / u0);
		else
			tail.assign(x({1,-1, 0,0}) / u0);
	}
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
void Householder<T>::evalHHmatrixDataI(NDArray& x, T& coeff, T& normX) {

	int rows = (int)x.lengthOf()-1;
	int num = 1;

	if(rows == 0) {
		rows = 1;
		num = 0;
	}

	NDArray tail(x.ordering(), {rows} /*{rows,1}*/, x.dataType(), x.getContext());
	evalHHmatrixData(x, tail, coeff, normX);

	if(x.isRowVector())
		x({0,0,  num, x.sizeAt(1)}).assign(tail);
	else
		x({num,x.sizeAt(0), 0,0}).assign(tail);
}

//////////////////////////////////////////////////////////////////////////
template <typename T>
void Householder<T>::mulLeft(NDArray& matrix, const NDArray& tail, const T coeff) {

	// if(matrix.rankOf() != 2)
	// 	throw "ops::helpers::Householder::mulLeft method: input array must be 2D matrix !";

	if(matrix.sizeAt(0) == 1) {
        matrix *= (T) 1.f - coeff;
    }
    else if(coeff != (T)0.f) {

  		NDArray bottomPart = matrix({1,matrix.sizeAt(0), 0,0}, true);

		if(tail.isColumnVector()) {

    		auto resultingRow = mmul(tail.transpose(), bottomPart);
    		auto fistRow = matrix({0,1, 0,0}, true);
    		resultingRow += fistRow;
    		fistRow -= resultingRow * coeff;
    		bottomPart -= mmul(tail, resultingRow) * coeff;
		}
		else {

    		auto resultingRow = mmul(tail, bottomPart);
    		auto fistRow = matrix({0,1, 0,0}, true);
    		resultingRow += fistRow;
    		fistRow -= resultingRow * coeff;
    		bottomPart -= mmul(tail.transpose(), resultingRow) * coeff;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
template <typename T>
void Householder<T>::mulRight(NDArray& matrix, const NDArray& tail, const T coeff) {

	// if(matrix.rankOf() != 2)
	// 	throw "ops::helpers::Householder::mulRight method: input array must be 2D matrix !";

	if(matrix.sizeAt(1) == 1)
    	matrix *= (T)1.f - coeff;

  	else if(coeff != (T)0.f) {

  		NDArray rightPart = matrix({0,0, 1,matrix.sizeAt(1)}, true);
		NDArray fistCol   = matrix({0,0, 0,1}, true);

  		if(tail.isColumnVector()) {

    		auto resultingCol = mmul(rightPart, tail);
    		resultingCol += fistCol;
    		fistCol -= resultingCol * coeff;
    		rightPart -= mmul(resultingCol, tail.transpose()) * coeff;
		}
		else {

    		auto resultingCol = mmul(rightPart, tail.transpose());
    		resultingCol += fistCol;
    		fistCol -= resultingCol * coeff;
    		rightPart -= mmul(resultingCol, tail) * coeff;
		}
	}
}


template class ND4J_EXPORT Householder<float>;
template class ND4J_EXPORT Householder<float16>;
template class ND4J_EXPORT Householder<bfloat16>;
template class ND4J_EXPORT Householder<double>;







}
}
}
