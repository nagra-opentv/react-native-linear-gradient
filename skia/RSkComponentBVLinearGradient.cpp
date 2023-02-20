/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "include/core/SkPaint.h"
#include "include/core/SkRRect.h"
#include "include/effects/SkGradientShader.h"

#include "react/renderer/components/rnx-gradient/Props.h"

#include "RSkComponentBVLinearGradient.h"

namespace facebook {
namespace react {

RSkComponentBVLinearGradient::RSkComponentBVLinearGradient(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

RnsShell::LayerInvalidateMask  RSkComponentBVLinearGradient::updateComponentProps(SharedProps newViewProps, bool forceUpdate) {
  RnsShell::LayerInvalidateMask invalidateMask = RnsShell::LayerInvalidateNone;

  auto const &newBVLinearGradientProps = *std::static_pointer_cast<LinearGradientViewProps const>(newViewProps);

  RNS_LOG_DEBUG(" Start: [" << newBVLinearGradientProps.start.x << "," << newBVLinearGradientProps.start.y <<
                  "], End: [" << newBVLinearGradientProps.end.x << "," << newBVLinearGradientProps.end.y <<
                  "], Colors: " << newBVLinearGradientProps.colors.size() <<
                  ", Locations: " << newBVLinearGradientProps.locations.size() <<
                  ", UseAngle: " << newBVLinearGradientProps.useAngle <<
                  ", Angle: " << newBVLinearGradientProps.angle <<
                  ", AngleCenter: [" << newBVLinearGradientProps.angleCenter.x << "," << newBVLinearGradientProps.angleCenter.y << "]");

  RNS_UNUSED(newBVLinearGradientProps);
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

  SkPoint startEndPoints[] = { //Initialize with default values {start{0.5,0} end{0.5,0.1}}
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

  SkPaint paint;
  SkRRect roundRect;
  SkVector radii[4]={{borderMetrics.borderRadii.topLeft,borderMetrics.borderRadii.topLeft},
                      {borderMetrics.borderRadii.topRight,borderMetrics.borderRadii.topRight},
                      {borderMetrics.borderRadii.bottomRight,borderMetrics.borderRadii.bottomRight },
                      {borderMetrics.borderRadii.bottomLeft,borderMetrics.borderRadii.bottomLeft}};
  auto gradientShader = SkGradientShader::MakeLinear(startEndPoints,
                                          colors,
                                          (locationsMapAvailable ? pos : nullptr),
                                          numColors,
                                          SkTileMode::kClamp,
                                          0/*flags*/, &matrx);
  paint.setShader(gradientShader);
  roundRect.setRectRadii({frameX, frameY, frameX+frameWidth, frameY+frameHeight}, radii);
  canvas->drawRRect(roundRect, paint);

  drawBorder(canvas, frame, borderMetrics, linearGradientProps.backgroundColor);
}

} // namespace react
} // namespace facebook
