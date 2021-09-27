//
//  ASImageContainerProtocolCategories.mm
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASImageContainerProtocolCategories.h>

@implementation UIImage (ASImageContainerProtocol)

- (UIImage *)asdk_image
{
    return self;
}

@end

@implementation NSData (ASImageContainerProtocol)

- (UIImage *)asdk_image
{
    return nil;
}

@end
