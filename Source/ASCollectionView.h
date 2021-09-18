//
//  ASCollectionView.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <UIKit/UIKit.h>

#import <AsyncDisplayKit/ASCollectionViewProtocols.h>
#import <AsyncDisplayKit/ASBaseDefines.h>
#import <AsyncDisplayKit/ASBatchContext.h>
#import <AsyncDisplayKit/ASDimension.h>
#import <AsyncDisplayKit/ASLayoutRangeType.h>
#import <AsyncDisplayKit/ASScrollDirection.h>

@class ASCellNode;
@class ASCollectionNode;
@protocol ASCollectionDataSource;
@protocol ASCollectionDelegate;
@protocol ASCollectionViewLayoutInspecting;
@protocol ASSectionContext;

NS_ASSUME_NONNULL_BEGIN

/**
 * Asynchronous UICollectionView with Intelligent Preloading capabilities.
 *
 * @note ASCollectionNode is strongly recommended over ASCollectionView.  This class exists for adoption convenience.
 */
@interface ASCollectionView : UICollectionView <UIGestureRecognizerDelegate>

/**
 * Returns the corresponding ASCollectionNode
 *
 * @return collectionNode The corresponding ASCollectionNode, if one exists.
 */
@property (nonatomic, weak, readonly) ASCollectionNode *collectionNode;

/**
 * Retrieves the node for the item at the given index path.
 *
 * @param indexPath The index path of the requested node.
 * @return The node at the given index path, or @c nil if no item exists at the specified path.
 */
- (nullable ASCellNode *)nodeForItemAtIndexPath:(NSIndexPath *)indexPath AS_WARN_UNUSED_RESULT;

/**
 * Similar to -indexPathForCell:.
 *
 * @param cellNode a cellNode in the collection view
 *
 * @return The index path for this cell node.
 *
 * @discussion This index path returned by this method is in the _view's_ index space
 *    and should only be used with @c ASCollectionView directly. To get an index path suitable
 *    for use with your data source and @c ASCollectionNode, call @c indexPathForNode: on the
 *    collection node instead.
 */
- (nullable NSIndexPath *)indexPathForNode:(ASCellNode *)cellNode AS_WARN_UNUSED_RESULT;

/**
 * Similar to -supplementaryViewForElementKind:atIndexPath:
 *
 * @param elementKind The kind of supplementary node to locate.
 * @param indexPath The index path of the requested supplementary node.
 *
 * @return The specified supplementary node or @c nil.
 */
- (nullable ASCellNode *)supplementaryNodeForElementKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)indexPath AS_WARN_UNUSED_RESULT;

/**
 * Retrieves the context object for the given section, as provided by the data source in
 * the @c collectionNode:contextForSection: method. This method must be called on the main thread.
 *
 * @param section The section to get the context for.
 *
 * @return The context object, or @c nil if no context was provided.
 */
- (nullable id<ASSectionContext>)contextForSection:(NSInteger)section AS_WARN_UNUSED_RESULT;

@end

@interface ASCollectionView (Deprecated)

/**
 * See ASCollectionNode.h for full documentation of these methods.
 */
@property (nonatomic, readonly) BOOL isProcessingUpdates;
- (void)onDidFinishProcessingUpdates:(void (^)(void))completion;

/**
 * See ASCollectionNode.h for full documentation of these methods.
 */
@property (nonatomic, readonly, getter=isSynchronized) BOOL synchronized;
- (void)onDidFinishSynchronizing:(void (^)(void))completion;

@end

/**
 * Defines methods that let you coordinate a `UICollectionViewFlowLayout` in combination with an `ASCollectionNode`.
 */
@protocol ASCollectionDelegateFlowLayout <ASCollectionDelegate>

@optional

/**
 * Asks the delegate for the inset that should be applied to the given section.
 *
 * @see the same method in UICollectionViewDelegate. 
 */
- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section;

/**
 * Asks the delegate for the size range that should be used to measure the header in the given flow layout section.
 *
 * @param collectionNode The sender.
 * @param section The section.
 *
 * @return The size range for the header, or @c ASSizeRangeZero if there is no header in this section.
 *
 * If you want the header to completely determine its own size, return @c ASSizeRangeUnconstrained.
 *
 * @note Only the scrollable dimension of the returned size range will be used. In a vertical flow,
 * only the height will be used. In a horizontal flow, only the width will be used. The other dimension
 * will be constrained to fill the collection node.
 *
 * @discussion If you do not implement this method, ASDK will fall back to calling @c collectionView:layout:referenceSizeForHeaderInSection:
 * and using that as the exact constrained size. If you don't implement that method, ASDK will read the @c headerReferenceSize from the layout.
 */
- (ASSizeRange)collectionNode:(ASCollectionNode *)collectionNode sizeRangeForHeaderInSection:(NSInteger)section;

/**
 * Asks the delegate for the size range that should be used to measure the footer in the given flow layout section.
 *
 * @param collectionNode The sender.
 * @param section The section.
 *
 * @return The size range for the footer, or @c ASSizeRangeZero if there is no footer in this section.
 *
 * If you want the footer to completely determine its own size, return @c ASSizeRangeUnconstrained.
 *
 * @note Only the scrollable dimension of the returned size range will be used. In a vertical flow,
 * only the height will be used. In a horizontal flow, only the width will be used. The other dimension
 * will be constrained to fill the collection node.
 *
 * @discussion If you do not implement this method, ASDK will fall back to calling @c collectionView:layout:referenceSizeForFooterInSection:
 * and using that as the exact constrained size. If you don't implement that method, ASDK will read the @c footerReferenceSize from the layout.
 */
- (ASSizeRange)collectionNode:(ASCollectionNode *)collectionNode sizeRangeForFooterInSection:(NSInteger)section;

@end

NS_ASSUME_NONNULL_END
