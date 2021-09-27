//
//  ASNetworkImageNode.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <AsyncDisplayKit/ASImageNode.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ASNetworkImageNodeDelegate, ASImageCacheProtocol, ASImageDownloaderProtocol;
@class ASNetworkImageLoadInfo;


/**
 * ASNetworkImageNode is a simple image node that can download and display an image from the network, with support for a
 * placeholder image (<defaultImage>).  The currently-displayed image is always available in the inherited ASImageNode
 * <image> property.
 *
 * @see ASMultiplexImageNode for a more powerful counterpart to this class.
 */
@interface ASNetworkImageNode : ASImageNode

/**
 * The designated initializer. Cache and Downloader are WEAK references.
 *
 * @param cache The object that implements a cache of images for the image node.  Weak reference.
 * @param downloader The object that implements image downloading for the image node.  Must not be nil.  Weak reference.
 *
 * @discussion If `cache` is nil, the receiver will not attempt to retrieve images from a cache before downloading them.
 *
 * @return An initialized ASNetworkImageNode.
 */
- (instancetype)initWithCache:(nullable id<ASImageCacheProtocol>)cache downloader:(id<ASImageDownloaderProtocol>)downloader NS_DESIGNATED_INITIALIZER;

/**
 * Convenience initializer.
 *
 * @return An ASNetworkImageNode configured to use the NSURLSession-powered ASBasicImageDownloader, and no extra cache.
 */
- (instancetype)init;

/**
 * The delegate, which must conform to the <ASNetworkImageNodeDelegate> protocol.
 */
@property (nullable, weak) id<ASNetworkImageNodeDelegate> delegate;

/**
 * The image to display.
 *
 * @discussion By setting an image to the image property the ASNetworkImageNode will act like a plain ASImageNode.
 * As soon as the URL is set the ASNetworkImageNode will act like an ASNetworkImageNode and the image property
 * will be managed internally. This means the image property will be cleared out and replaced by the placeholder 
 * (<defaultImage>) image while loading and the final image after the new image data was downloaded and processed.
 * If you want to use a placholder image functionality use the defaultImage property instead.
 */
@property (nullable) UIImage *image;

/**
 * A placeholder image to display while the URL is loading. This is slightly different than placeholderImage in the
 * ASDisplayNode superclass as defaultImage will *not* be displayed synchronously. If you wish to have the image
 * displayed synchronously, use @c placeholderImage.
 */
@property (nullable) UIImage *defaultImage;

/**
 * The URL of a new image to download and display.
 *
 * @discussion By setting an URL, the image property of this node will be managed internally. This means previously
 * directly set images to the image property will be cleared out and replaced by the placeholder (<defaultImage>) image
 * while loading and the final image after the new image data was downloaded and processed.
 */
@property (nullable, copy) NSURL *URL;

@end


#pragma mark -

/**
 * The methods declared by the ASNetworkImageNodeDelegate protocol allow the adopting delegate to respond to
 * notifications such as finished decoding and downloading an image.
 */
@protocol ASNetworkImageNodeDelegate <NSObject>
@optional

/**
 * Notification that the image node started to load
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeDidStartFetchingData:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node will load image from cache
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeWillLoadImageFromCache:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node finished loading image from cache
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeDidLoadImageFromCache:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node failed to load image from cache
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeDidFailToLoadImageFromCache:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node will load image from network
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeWillLoadImageFromNetwork:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node will start display
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeWillStartDisplayAsynchronously:(ASNetworkImageNode *)imageNode;

/**
 * Notification that the image node finished downloading an image, with additional info.
 * If implemented, this method will be called instead of `imageNode:didLoadImage:`.
 *
 * @param imageNode The sender.
 * @param image The newly-loaded image.
 * @param info Additional information about the image load.
 *
 * @discussion Called on the main thread if useMainThreadDelegateCallbacks=YES (the default), otherwise on a background thread.
 */
- (void)imageNode:(ASNetworkImageNode *)imageNode didLoadImage:(UIImage *)image info:(ASNetworkImageLoadInfo *)info;

/**
 * Notification that the image node finished downloading an image.
 *
 * @param imageNode The sender.
 * @param image The newly-loaded image.
 *
 * @discussion Called on the main thread if useMainThreadDelegateCallbacks=YES (the default), otherwise on a background thread.
 */
- (void)imageNode:(ASNetworkImageNode *)imageNode didLoadImage:(UIImage *)image;

/**
 * Notification that the image node failed to download the image.
 *
 * @param imageNode The sender.
 * @param error The error with details.
 *
 * @discussion Called on the main thread if useMainThreadDelegateCallbacks=YES (the default), otherwise on a background thread.
 */
- (void)imageNode:(ASNetworkImageNode *)imageNode didFailWithError:(NSError *)error;

/**
 * Notification that the image node finished decoding an image.
 *
 * @param imageNode The sender.
 *
 * @discussion Called on the main thread.
 */
- (void)imageNodeDidFinishDecoding:(ASNetworkImageNode *)imageNode;


@end

NS_ASSUME_NONNULL_END
