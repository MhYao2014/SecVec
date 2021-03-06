/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "matrix.h"
#include "real.h"
#include "utils.h"
#include "vector.h"

namespace fasttext {

    class Loss;

    class Model {
    protected:
        std::shared_ptr<Matrix> wi_;
        std::shared_ptr<Matrix> wo_;
        std::shared_ptr<Loss> loss_;
        bool normalizeGradient_;

    public:
        Model(
                std::shared_ptr<Matrix> wi,
                std::shared_ptr<Matrix> wo,
                std::shared_ptr<Loss> loss,
                bool normalizeGradient);
        Model(const Model& model) = delete;
        Model(Model&& model) = delete;
        Model& operator=(const Model& other) = delete;
        Model& operator=(Model&& other) = delete;

        class State {
        private:
            real lossFirst_;
            real lossSecond_;
            int64_t nexamples_;

        public:
            Vector inputGrad;
            Vector inputVec;
            Vector outputVec;
            Vector secOutVec;
            Vector sumOutVec;
            std::minstd_rand rng;
            int thread_id;
            long long inId;
            real inNorm;

            State(int32_t hiddenSize, int32_t outputSize, int thread_id, int32_t seed);
            real getFirstLoss() const;
            real getSecondLoss() const;
            void incrementNExamples();
            void incrementLoss(real & tmpLossFirst, real & tmpLossSecond);
        };

        void update(
                const int32_t & input,
                const std::vector<int32_t>& targets,
                int32_t targetIndex,
                int32_t secTargetIdex,
                real lr,
                State& state);
        void computeHidden(const int32_t & input, State& state) const;
    };
} // namespace fasttext
