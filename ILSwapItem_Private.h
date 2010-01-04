
#import "ILSwapItem.h"

#define kILSwapItemAttributesUTI @"net.infinite-labs.SwapKit.ItemAttributes"

@interface ILSwapItem (ILSwapItemPasteboard)

/** @internal */
- (NSDictionary*) pasteboardItemOfType:(NSString*) type;

+ (NSDictionary*) attributesFromDataOrNil:(NSData*) d;

@end