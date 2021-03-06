
#import "ILSwapItem.h"

#define kILSwapItemAttributesUTI @"net.infinite-labs.SwapKit.ItemAttributes"

@interface ILSwapItem (ILSwapItemPasteboard)

/** @internal */
- (NSDictionary*) pasteboardItem;

+ (NSDictionary*) attributesFromPasteboardValue:(NSData*) d;

@end