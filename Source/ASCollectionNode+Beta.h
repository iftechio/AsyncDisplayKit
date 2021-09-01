//
//  ASCollectionNode+Beta.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASCollectionNode.h>

@protocol ASCollectionLayoutDelegate, ASBatchFetchingDelegate;
@class ASElementMap;

NS_ASSUME_NONNULL_BEGIN

@interface ASCollectionNode (Beta)

/**
 * Allows providing a custom subclass of ASCollectionView to be managed by ASCollectionNode.
 *
 * @default [ASCollectionView class] is used whenever this property is unset or nil.
 */
@property (nullable, nonatomic) Class collectionViewClass;

/**
 * The elements that are currently displayed. The "UIKit index space". Must be accessed on main thread.
 */
@property (nonatomic, readonly) ASElementMap *visibleElements;

@property (nullable, nonatomic, weak) id<ASBatchFetchingDelegate> batchFetchingDelegate;

/**
 *  Returns YES if the ASCollectionNode contents are completely synchronized with the underlying collection-view layout.
 */
@property (nonatomic, readonly, getter=isSynchronized) BOOL synchronized;

/**
 *  Schedules a block to be performed (on the main thread) as soon as the completion block is called
 *  on performBatchUpdates:.
 *
 *  When isSynchronized == YES, the block is run block immediately (before the method returns).
 */
- (void)onDidFinishSynchronizing:(void (^)(void))didFinishSynchronizing;

- (instancetype)initWithFrame:(CGRect)frame collectionViewLayout:(UICollectionViewLayout *)layout;

- (instancetype)initWithLayoutDelegate:(id<ASCollectionLayoutDelegate>)layoutDelegate;

@end

NS_ASSUME_NONNULL_END
