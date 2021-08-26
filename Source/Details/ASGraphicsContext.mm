//
//  ASGraphicsContext.mm
//  Texture
//
//  Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASGraphicsContext.h>
#import <AsyncDisplayKit/ASAssert.h>
#import <AsyncDisplayKit/ASInternalHelpers.h>
#import <AsyncDisplayKit/ASAvailability.h>


#if AS_AT_LEAST_IOS13
#define ASPerformBlockWithTraitCollection(work, traitCollection) \
    if (@available(iOS 13.0, tvOS 13.0, *)) { \
      UITraitCollection *uiTraitCollection = ASPrimitiveTraitCollectionToUITraitCollection(traitCollection); \
      [uiTraitCollection performAsCurrentTraitCollection:^{ \
        work(); \
      }];\
    } else { \
      work(); \
    }
#else
#define ASPerformBlockWithTraitCollection(work, traitCollection) work();
#endif


NS_AVAILABLE_IOS(10)
UIImage *ASGraphicsCreateImageWithOptions(CGSize size, BOOL opaque, CGFloat scale, UIImage *sourceImage,
                                          asdisplaynode_iscancelled_block_t NS_NOESCAPE isCancelled,
                                          void (^NS_NOESCAPE work)())
{
  return ASGraphicsCreateImage(ASPrimitiveTraitCollectionMakeDefault(), size, opaque, scale, sourceImage, isCancelled, work);
}

UIImage *ASGraphicsCreateImage(ASPrimitiveTraitCollection traitCollection, CGSize size, BOOL opaque, CGFloat scale, UIImage * sourceImage, asdisplaynode_iscancelled_block_t NS_NOESCAPE isCancelled, void (NS_NOESCAPE ^work)()) {
  // Bad OS or experiment flag. Use UIGraphics* API.
  UIGraphicsBeginImageContextWithOptions(size, opaque, scale);
  ASPerformBlockWithTraitCollection(work, traitCollection)
  UIImage *image = nil;
  if (isCancelled == nil || !isCancelled()) {
    image = UIGraphicsGetImageFromCurrentImageContext();
  }
  UIGraphicsEndImageContext();
  return image;
}

UIImage *ASGraphicsCreateImageWithTraitCollectionAndOptions(ASPrimitiveTraitCollection traitCollection, CGSize size, BOOL opaque, CGFloat scale, UIImage * sourceImage, void (NS_NOESCAPE ^work)()) {
  return ASGraphicsCreateImage(traitCollection, size, opaque, scale, sourceImage, nil, work);
}
