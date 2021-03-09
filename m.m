// Not working
// #import <os/availability.h>
// #define __OS_AVAILABILITY__
// #undef API_UNAVAILABLE
// #import <SystemConfiguration/SCNetworkConfiguration.h>

// https://unix.stackexchange.com/questions/495421/how-to-mount-a-file-on-another-file
// https://www.mardy.it/blog/2018/10/how-to-bind-mount-single-file.html
// paccheck --sha256sum theos-ios-sdks-git
// sudo pacman -U ~/.cache/yay/theos-ios-sdks-git/theos-ios-sdks-git-r25.40a7cee1-1-any.pkg.tar
// #import "./patched/SCNetworkConfiguration.h"
#import <SystemConfiguration/SCNetworkConfiguration.h>

// #import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>

// Re-implement with CFPropertyList in c.c
// https://developer.apple.com/documentation/corefoundation/cfpropertylist?language=objc
void ns_plist(const char *const path){@autoreleasepool{

  // Check if path exists
  // Check if path is readable
  // Check if path is writable

  // /private/var/mobile/Data.plist
  // CFShow ((__bridge CFTypeRef)[NSBundle mainBundle]);
  // NSString *path = [[NSBundle mainBundle] pathForResource:@"Data" ofType:@"plist"];
  // contentArray = [NSArray arrayWithContentsOfFile:filePath];

  // void *err=nil;
  // id err=nil;
  // NSString *err=nil;
  NSError *err=nil;
  NSPropertyListFormat fmt=-1;
  puts([[NSString stringWithFormat:@"%@",[NSPropertyListSerialization
    propertyListWithData:[NSData dataWithContentsOfFile:[[NSString new]initWithUTF8String:path]]
    options:NSPropertyListMutableContainersAndLeaves
    format:&fmt
    error:&err
  ]]UTF8String]);
  assert(!err);
  assert(fmt==NSPropertyListBinaryFormat_v1_0);
  // printf("%[%lu,%lu,%lu] %lu\n",
  //   NSPropertyListOpenStepFormat,
  //   NSPropertyListXMLFormat_v1_0,
  //   NSPropertyListBinaryFormat_v1_0,
  //   fmt);

}}
