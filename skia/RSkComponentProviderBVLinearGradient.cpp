/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RSkComponentProviderBVLinearGradient.h"
#include "RSkComponentBVLinearGradient.h"

#include "react/renderer/components/rnx-gradient/ComponentDescriptors.h"

namespace facebook {
namespace react {

RSkComponentProviderBVLinearGradient::RSkComponentProviderBVLinearGradient() {}

ComponentDescriptorProvider RSkComponentProviderBVLinearGradient::GetDescriptorProvider() {
  return concreteComponentDescriptorProvider<LinearGradientViewComponentDescriptor>();
}

std::shared_ptr<RSkComponent> RSkComponentProviderBVLinearGradient::CreateComponent(
    const ShadowView &shadowView) {
  return std::static_pointer_cast<RSkComponent>(
      std::make_shared<RSkComponentBVLinearGradient>(shadowView));
}

#ifdef __cplusplus
extern "C" {
#endif
RNS_EXPORT_COMPONENT_PROVIDER(BVLinearGradient)
#ifdef __cplusplus
}
#endif

} // namespace react
} // namespace facebook
