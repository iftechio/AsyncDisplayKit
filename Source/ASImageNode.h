//
//  ASImageNode.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <UIKit/UIKit.h>
#import <AsyncDisplayKit/ASControlNode.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Image modification block.  Use to transform an image before display.
 *
 * @param image The image to be displayed.
 *
 * @return A transformed image.
 */
typedef UIImage * _Nullable (^asimagenode_modification_block_t)(UIImage *image, ASPrimitiveTraitCollection traitCollection);

/**
 * @abstract Draws images.
 * @discussion Supports cropping, tinting, and arbitrary image modification blocks.
 */
@interface ASImageNode : ASControlNode

/**
 * @abstract The image to display.
 *
 * @discussion The node will efficiently display stretchable images by using
 * the layer's contentsCenter property.  Non-stretchable images work too, of
 * course.
 */
@property (nullable) UIImage *image;

/**
 @abstract The placeholder color.
 */
@property (nullable, copy) UIColor *placeholderColor;

/**
 * @abstract A value that controls how the receiver's efficient cropping is aligned.
 *
 * @discussion This value defines a rectangle that is to be featured by the
 * receiver. The rectangle is specified as a "unit rectangle," using
 * fractions of the source image's width and height, e.g. CGRectMake(0.5, 0,
 * 0.5, 1.0) will feature the full right half a photo. If the cropRect is
 * empty, the content mode of the receiver will be used to determine its
 * dimensions, and only the cropRect's origin will be used for positioning. The
 * default value of this property is CGRectMake(0.5, 0.5, 0.0, 0.0).
 */
@property CGRect cropRect;

/**
 * @abstract An optional block which can perform drawing operations on image
 * during the display phase.
 *
 * @discussion Can be used to add image effects (such as rounding, adding
 * borders, or other pattern overlays) without extraneous display calls.
 */
@property (nullable) asimagenode_modification_block_t imageModificationBlock;

@end

NS_ASSUME_NONNULL_END
