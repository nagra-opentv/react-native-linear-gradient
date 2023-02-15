/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/utils/RnsUtils.h"

// When useAngle is true convert it to cartesian degrees
#define RNS_BVLG_CARTESIAN_ANGLE_CORRECTION 180

namespace facebook {
namespace react {

using namespace std;

class RSkComponentBVLinearGradient : public RSkComponent {
 public:
  RSkComponentBVLinearGradient(const ShadowView &shadowView);
  ~RSkComponentBVLinearGradient() = default;
  RnsShell::LayerInvalidateMask updateComponentProps(SharedProps newviewProps,bool forceUpdate) override;
  void handleCommand(std::string commandName, folly::dynamic args) override;

 protected:
  void OnPaint(SkCanvas *canvas) override;

 private:
  void willInvalidateModules();
};

} // namespace react
} // namespace facebook
