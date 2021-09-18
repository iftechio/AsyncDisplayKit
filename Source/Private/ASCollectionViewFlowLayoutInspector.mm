//
//  ASCollectionViewFlowLayoutInspector.mm
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASCollectionViewFlowLayoutInspector.h>
#import <AsyncDisplayKit/ASCollectionView.h>
#import <AsyncDisplayKit/ASEqualityHelpers.h>
#import <AsyncDisplayKit/ASCollectionNode.h>
#import <AsyncDisplayKit/ASCollectionView+Undeprecated.h>

#define kDefaultItemSize CGSizeMake(50, 50)

#pragma mark - ASCollectionViewFlowLayoutInspector

@interface ASCollectionViewFlowLayoutInspector ()
@property (nonatomic, weak) UICollectionViewFlowLayout *layout;
@end
 
@implementation ASCollectionViewFlowLayoutInspector {
  struct {
    unsigned int implementsSizeRangeForHeader:1;
    unsigned int implementsSizeRangeForFooter:1;
    unsigned int implementsConstrainedSizeForItemAtIndexPath:1;
  } _delegateFlags;
}

#pragma mark Lifecycle

- (instancetype)initWithFlowLayout:(UICollectionViewFlowLayout *)flowLayout;
{
  NSParameterAssert(flowLayout);
  
  self = [super init];
  if (self != nil) {
    _layout = flowLayout;
  }
  return self;
}

#pragma mark ASCollectionViewLayoutInspecting

- (void)didChangeCollectionViewDelegate:(id<ASCollectionDelegate>)delegate;
{
  if (delegate == nil) {
    memset(&_delegateFlags, 0, sizeof(_delegateFlags));
  } else {
    _delegateFlags.implementsSizeRangeForHeader = [delegate respondsToSelector:@selector(collectionNode:sizeRangeForHeaderInSection:)];
    _delegateFlags.implementsSizeRangeForFooter = [delegate respondsToSelector:@selector(collectionNode:sizeRangeForFooterInSection:)];
    _delegateFlags.implementsConstrainedSizeForItemAtIndexPath = [delegate respondsToSelector:@selector(collectionNode:constrainedSizeForItemAtIndexPath:)];
  }
}

- (ASSizeRange)collectionView:(ASCollectionView *)collectionView constrainedSizeForNodeAtIndexPath:(NSIndexPath *)indexPath
{
  ASSizeRange result = ASSizeRangeUnconstrained;
  if (_delegateFlags.implementsConstrainedSizeForItemAtIndexPath) {
    result = [collectionView.asyncDelegate collectionNode:collectionView.collectionNode constrainedSizeForItemAtIndexPath:indexPath];
  } else {
    // With 2.0 `collectionView:constrainedSizeForNodeAtIndexPath:` was moved to the delegate. Assert if not implemented on the delegate but on the data source
    ASDisplayNodeAssert(YES, @"collectionView:constrainedSizeForNodeAtIndexPath: was moved from the ASCollectionDataSource to the ASCollectionDelegate.");
  }

  // If we got no size range:
  if (ASSizeRangeEqualToSizeRange(result, ASSizeRangeUnconstrained)) {
    // Use itemSize if they set it.
    CGSize itemSize = _layout.itemSize;
    if (CGSizeEqualToSize(itemSize, kDefaultItemSize) == NO) {
      result = ASSizeRangeMake(itemSize, itemSize);
    } else {
      // Compute constraint from scroll direction otherwise.
      result = NodeConstrainedSizeForScrollDirection(collectionView);
    }
  }
  
  return result;
}

- (ASSizeRange)collectionView:(ASCollectionView *)collectionView constrainedSizeForSupplementaryNodeOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath
{
  ASSizeRange result = ASSizeRangeZero;
  if (ASObjectIsEqual(kind, UICollectionElementKindSectionHeader)) {
    if (_delegateFlags.implementsSizeRangeForHeader) {
      result = [[self delegateForCollectionView:collectionView] collectionNode:collectionView.collectionNode sizeRangeForHeaderInSection:indexPath.section];
    } else {
      result = ASSizeRangeMake(_layout.headerReferenceSize);
    }
  } else if (ASObjectIsEqual(kind, UICollectionElementKindSectionFooter)) {
    if (_delegateFlags.implementsSizeRangeForFooter) {
      result = [[self delegateForCollectionView:collectionView] collectionNode:collectionView.collectionNode sizeRangeForFooterInSection:indexPath.section];
    } else {
      result = ASSizeRangeMake(_layout.footerReferenceSize);
    }
  } else {
    ASDisplayNodeFailAssert(@"Unexpected supplementary kind: %@", kind);
    return ASSizeRangeZero;
  }

  if (_layout.scrollDirection == UICollectionViewScrollDirectionVertical) {
    result.min.width = result.max.width = CGRectGetWidth(collectionView.bounds);
  } else {
    result.min.height = result.max.height = CGRectGetHeight(collectionView.bounds);
  }
  return result;
}

- (NSUInteger)collectionView:(ASCollectionView *)collectionView supplementaryNodesOfKind:(NSString *)kind inSection:(NSUInteger)section
{
  ASSizeRange constraint = [self collectionView:collectionView constrainedSizeForSupplementaryNodeOfKind:kind atIndexPath:[NSIndexPath indexPathForItem:0 inSection:section]];
  if (_layout.scrollDirection == UICollectionViewScrollDirectionVertical) {
    return (constraint.max.height > 0 ? 1 : 0);
  } else {
    return (constraint.max.width > 0 ? 1 : 0);
  }
}

- (ASScrollDirection)scrollableDirections
{
  return (self.layout.scrollDirection == UICollectionViewScrollDirectionHorizontal) ? ASScrollDirectionHorizontalDirections : ASScrollDirectionVerticalDirections;
}

#pragma mark - Private helpers

- (id<ASCollectionDelegateFlowLayout>)delegateForCollectionView:(ASCollectionView *)collectionView
{
  return (id<ASCollectionDelegateFlowLayout>)collectionView.asyncDelegate;
}

@end
