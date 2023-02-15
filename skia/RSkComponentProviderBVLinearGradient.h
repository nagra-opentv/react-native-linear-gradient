/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ReactSkia/components/RSkComponentProvider.h"
#include "ReactSkia/utils/RnsUtils.h"

namespace facebook {
namespace react {

class RSkComponentProviderBVLinearGradient : public RSkComponentProvider {
 public:
  RSkComponentProviderBVLinearGradient();

 public:
  ComponentDescriptorProvider GetDescriptorProvider() override;
  std::shared_ptr<RSkComponent> CreateComponent(
      const ShadowView &shadowView) override;
};

} // namespace react
} // namespace facebook
