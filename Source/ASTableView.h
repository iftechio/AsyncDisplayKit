//
//  ASTableView.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <UIKit/UIKit.h>

#import <AsyncDisplayKit/ASBaseDefines.h>
#import <AsyncDisplayKit/ASLayoutRangeType.h>
#import <AsyncDisplayKit/ASTableViewProtocols.h>

NS_ASSUME_NONNULL_BEGIN

@class ASCellNode;
@protocol ASTableDataSource;
@protocol ASTableDelegate;
@class ASTableNode;

/**
 * Asynchronous UITableView with Intelligent Preloading capabilities.
 *
 * @note ASTableNode is strongly recommended over ASTableView.  This class is provided for adoption convenience.
 */
@interface ASTableView : UITableView

/// The corresponding table node, or nil if one does not exist.
@property (nonatomic, weak, readonly) ASTableNode *tableNode;

/**
 * See ASTableNode.h for full documentation of these methods.
 */
@property (nonatomic, readonly) BOOL isProcessingUpdates;
- (void)onDidFinishProcessingUpdates:(void (^)(void))completion;

/**
 * Retrieves the node for the row at the given index path.
 */
- (nullable ASCellNode *)nodeForRowAtIndexPath:(NSIndexPath *)indexPath AS_WARN_UNUSED_RESULT;

@end

NS_ASSUME_NONNULL_END
