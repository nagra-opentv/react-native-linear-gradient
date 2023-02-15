/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "include/core/SkPaint.h"
#include "include/effects/SkGradientShader.h"

#include "cxxreact/ModuleRegistry.h"

#include "react/renderer/components/rnx-gradient/Props.h"
#include "rns_shell/compositor/layers/PictureLayer.h"

#include "ReactSkia/views/common/RSkDrawUtils.h"

#include "RSkComponentBVLinearGradient.h"

namespace facebook {
namespace react {

RSkComponentBVLinearGradient::RSkComponentBVLinearGradient(const ShadowView &shadowView)
    : RSkComponent(shadowView) {
  RNS_LOG_TODO("Listen to notifications about module invalidation default notification center : RCTWillInvalidateModulesNotification");
}

void RSkComponentBVLinearGradient::handleCommand(std::string commandName, folly::dynamic args){
  RNS_LOG_WARN("[RSkComponentBVLinearGradient][handleCommand] commandName : "<< commandName);
  RNS_LOG_NOT_IMPL;
  return;
}

RnsShell::LayerInvalidateMask  RSkComponentBVLinearGradient::updateComponentProps(SharedProps newViewProps, bool forceUpdate) {
  RnsShell::LayerInvalidateMask invalidateMask = RnsShell::LayerInvalidateNone;

  auto const &newBVLinearGradientProps = *std::static_pointer_cast<LinearGradientViewProps const>(newViewProps);
  auto const &oldBVLinearGradientProps = *std::static_pointer_cast<LinearGradientViewProps const>(getComponentData().props);

  RNS_LOG_INFO(" Start: [" << newBVLinearGradientProps.start.x << "," << newBVLinearGradientProps.start.y <<
                  "], End: [" << newBVLinearGradientProps.end.x << "," << newBVLinearGradientProps.end.y <<
                  "], Colors: " << newBVLinearGradientProps.colors.size() <<
                  ", Locations: " << newBVLinearGradientProps.locations.size() <<
                  ", UseAngle: " << newBVLinearGradientProps.useAngle <<
                  ", Angle: " << newBVLinearGradientProps.angle <<
                  ", AngleCenter: [" << newBVLinearGradientProps.angleCenter.x << "," << newBVLinearGradientProps.angleCenter.y << "]");

  RNS_UNUSED(newBVLinearGradientProps);
  RNS_UNUSED(oldBVLinearGradientProps);
  return invalidateMask;
}

void RSkComponentBVLinearGradient::OnPaint(SkCanvas *canvas) {

  auto component = getComponentData();
  auto const &linearGradientProps = *std::static_pointer_cast<LinearGradientViewProps const>(component.props);

  /* apply view style props */
  auto borderMetrics=linearGradientProps.resolveBorderMetrics(component.layoutMetrics);
  size_t numColors = linearGradientProps.colors.size();

  Rect frame = component.layoutMetrics.frame;
  SharedColor bgColour = linearGradientProps.backgroundColor;
  SkScalar frameX = frame.origin.x;
  SkScalar frameY = frame.origin.y;
  SkScalar frameWidth = frame.size.width;
  SkScalar frameHeight = frame.size.height;

  // If background color is transparent then need to find atleast one non-transparent colour in gradient colours so that we can draw shadow properly.
  for(int i = 0; (!(bgColour && (colorComponentsFromColor(bgColour).alpha != 0.0)) && i < numColors); i++) {
    bgColour = linearGradientProps.colors.at(i);
  }

  /*Draw Order : 1. Shadow 2. Gradient 3 Border*/
  auto layerRef=layer();
  if(layerRef->isShadowVisible) {
    drawShadow(canvas,frame,borderMetrics,
                bgColour, // Used only to decide on what type of shadow to draw.
                layerRef->shadowColor,layerRef->shadowOffset,layerRef->shadowOpacity,
                layerRef->opacity,
                layerRef->shadowImageFilter,layerRef->shadowMaskFilter);
  }

  RNS_LOG_DEBUG(std::this_thread::get_id() << "] Layer " << layer().get() <<" Draw Picture Here with Frame  [" << frameX << "," << frameY << "," << frameWidth << "," << frameHeight << "]");

  SkColor colors[numColors];
  SkScalar pos[linearGradientProps.locations.size()];

  bool locationsMapAvailable = (linearGradientProps.locations.size() == numColors);

  for (std::size_t i = 0; i != numColors; ++i) {
    colors[i] = RSkColorFromSharedColor(linearGradientProps.colors.at(i), SK_ColorTRANSPARENT);
    if(locationsMapAvailable) {
      pos[i] = linearGradientProps.locations.at(i);
    }
  }

  SkPoint startEndPoints[] = { //Initialize with default value
    {(0.5 * frameWidth) + frameX, (0 * frameHeight) + frameY},
    {(0.5 * frameWidth)+ frameX, (1 * frameHeight) + frameY}};

  SkMatrix matrx = SkMatrix::I();

  if(linearGradientProps.useAngle) {
    // Apply angle and center to matrix
    matrx.setRotate(linearGradientProps.angle + RNS_BVLG_CARTESIAN_ANGLE_CORRECTION,
                    (linearGradientProps.angleCenter.x * frameWidth)  + frameX,
                    (linearGradientProps.angleCenter.y * frameHeight) + frameY);
    // Use default startEnd points
  } else {
    startEndPoints[0].set(((linearGradientProps.start.x * frameWidth) + frameX), ((linearGradientProps.start.y * frameHeight) + frameY));
    startEndPoints[1].set(((linearGradientProps.end.x * frameWidth) + frameX), ((linearGradientProps.end.y * frameHeight) + frameY));
  }

  auto lgs = SkGradientShader::MakeLinear(startEndPoints,
                                          colors,
                                          (locationsMapAvailable ? pos : nullptr),
                                          numColors,
                                          SkTileMode::kClamp,
                                          0/*flags*/, &matrx);

  SkPaint paint;
  paint.setShader(lgs);
  canvas->drawRoundRect({frameX, frameY, frameX+frameWidth, frameY+frameHeight},
                        borderMetrics.borderRadii.topLeft,
                        borderMetrics.borderRadii.topRight, paint);
  drawBorder(canvas, frame, borderMetrics, linearGradientProps.backgroundColor);
}

} // namespace react
} // namespace facebook
