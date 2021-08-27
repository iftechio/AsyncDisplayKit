//
//  ASImageNodeSnapshotTests.mm
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import "ASSnapshotTestCase.h"

#import <AsyncDisplayKit/AsyncDisplayKit.h>

#if AS_AT_LEAST_IOS13
static UIImage* makeImageWithColor(UIColor *color, CGSize size) {
  CGRect rect = CGRect{.origin = CGPointZero, .size = size};
  UIGraphicsBeginImageContextWithOptions(size, false, 0);
  [color setFill];
  UIRectFill(rect);
  UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
  UIGraphicsEndImageContext();
  return image;
}
#endif

@interface ASImageNodeSnapshotTests : ASSnapshotTestCase
@end

@implementation ASImageNodeSnapshotTests

- (void)setUp
{
  [super setUp];
  self.recordMode = NO;
}

- (UIImage *)testImage
{
  NSString *path = [[NSBundle bundleForClass:[self class]] pathForResource:@"logo-square"
                                                                    ofType:@"png"
                                                               inDirectory:@"TestResources"];
  return [UIImage imageWithContentsOfFile:path];
}

- (UIImage *)testGrayscaleImage
{
  NSString *path = [[NSBundle bundleForClass:[self class]] pathForResource:@"logo-square-black"
                                                                    ofType:@"png"
                                                               inDirectory:@"TestResources"];
  return [UIImage imageWithContentsOfFile:path];
}

- (void)testRenderLogoSquare
{
  // trivial test case to ensure ASSnapshotTestCase works
  ASImageNode *imageNode = [[ASImageNode alloc] init];
  imageNode.image = [self testImage];
  ASDisplayNodeSizeToFitSize(imageNode, CGSizeMake(100, 100));

  ASSnapshotVerifyNode(imageNode, nil);
}

- (void)testTintColorOnNodePropertyAlwaysTemplate
{
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
  node.tintColor = UIColor.redColor;
  ASDisplayNodeSizeToFitSize(node, test.size);
  // Tint color should change view
  ASSnapshotVerifyNode(node, @"red_tint");

  node.tintColor = UIColor.blueColor;
  // Tint color should change view
  ASSnapshotVerifyNode(node, @"blue_tint");
}

- (void)testTintColorOnGrayscaleNodePropertyAlwaysTemplate
{
  UIImage *test = [self testGrayscaleImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
  node.tintColor = UIColor.redColor;
  ASDisplayNodeSizeToFitSize(node, test.size);
  // Tint color should change view
  ASSnapshotVerifyNode(node, @"red_tint");

  node.tintColor = UIColor.blueColor;
  // Tint color should change view
  ASSnapshotVerifyNode(node, @"blue_tint");
}

- (void)testTintColorOnNodePropertyAutomatic
{
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = test;
  // Tint color should not change view since it depends on being contained within certain views
  // for automatic rendering to utilize tint color.
  node.tintColor = UIColor.redColor;
  ASDisplayNodeSizeToFitSize(node, test.size);
  ASSnapshotVerifyNode(node, nil);
}

- (void)testTintColorOnNodePropertyAlwaysOriginal
{
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
  // Tint color should not have changed since the image render mode is original
  node.tintColor = UIColor.redColor;
  ASDisplayNodeSizeToFitSize(node, test.size);
  ASSnapshotVerifyNode(node, nil);
}

- (void)testTintColorOnNodePropertyAlwaysTemplateLayerBackedNode
{
  // Test support for layerBacked image node tinting
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  [node setLayerBacked:YES];
  node.tintColor = UIColor.redColor;
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
  ASDisplayNodeSizeToFitSize(node, test.size);
  ASSnapshotVerifyNode(node, nil);
}


- (void)testTintColorInheritsFromSupernodeLayerBacked
{
   // Test support for layerBacked image node tinting
  ASDisplayNode *container = [[ASDisplayNode alloc] init];
  [container setLayerBacked:YES];
  container.tintColor = UIColor.redColor;
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  [node setLayerBacked:YES];
  node.tintColor = UIColor.redColor;
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
  [container addSubnode:node];
  container.style.preferredSize = test.size;
  ASDisplayNodeSizeToFitSize(node, test.size);
  ASSnapshotVerifyNode(node, nil);
}

- (void)testTintColorInheritsFromSupernodeViewBacked
{
  // Test support for layerBacked image node tinting
  ASDisplayNode *container = [[ASDisplayNode alloc] init];
  [container setLayerBacked:NO];
  container.tintColor = UIColor.redColor;
  UIImage *test = [self testImage];
  ASImageNode *node = [[ASImageNode alloc] init];
  [node setLayerBacked:YES];
  node.image = [test imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
  [container addSubnode:node];
  container.style.preferredSize = test.size;
  ASDisplayNodeSizeToFitSize(node, test.size);
  ASSnapshotVerifyNode(node, nil);
}

- (void)testUIGraphicsRendererDrawingExperiment
{
  ASImageNode *imageNode = [[ASImageNode alloc] init];
  imageNode.image = [self testImage];
  ASDisplayNodeSizeToFitSize(imageNode, CGSizeMake(100, 100));
  ASSnapshotVerifyNode(imageNode, nil);
}

#if AS_AT_LEAST_IOS13
- (void)testDynamicAssetImage
{
  if (@available(iOS 13.0, *)) {
    UIImage *image = [UIImage imageNamed:@"light-dark" inBundle:[NSBundle bundleForClass:[self class]] compatibleWithTraitCollection:nil];
    ASImageNode *node = [[ASImageNode alloc] init];
    node.image = image;
    ASDisplayNodeSizeToFitSize(node, image.size);

    [[UITraitCollection traitCollectionWithUserInterfaceStyle:UIUserInterfaceStyleLight] performAsCurrentTraitCollection:^{
      ASSnapshotVerifyNode(node, @"user_interface_style_light");
    }];

    [[UITraitCollection traitCollectionWithUserInterfaceStyle:UIUserInterfaceStyleDark] performAsCurrentTraitCollection:^{
      ASSnapshotVerifyNode(node, @"user_interface_style_dark");
    }];
  }
}

- (void)testDynamicTintColor
{
  if (@available(iOS 13.0, *)) {
    UIImage *image = makeImageWithColor(UIColor.redColor, CGSize{.width =  100, .height = 100});
    image = [image imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
    UIColor* tintColor = UIColor.systemBackgroundColor;
    ASImageNode *node = [[ASImageNode alloc] init];
    
    node.image = image;
    node.tintColor = tintColor;
    
    ASDisplayNodeSizeToFitSize(node, image.size);

    [[UITraitCollection traitCollectionWithUserInterfaceStyle:UIUserInterfaceStyleLight] performAsCurrentTraitCollection:^{
      ASSnapshotVerifyNode(node, @"user_interface_style_light");
    }];

    [[UITraitCollection traitCollectionWithUserInterfaceStyle:UIUserInterfaceStyleDark] performAsCurrentTraitCollection:^{
      ASSnapshotVerifyNode(node, @"user_interface_style_dark");
    }];
  }
}
#endif // #if AS_AT_LEAST_IOS13
@end
