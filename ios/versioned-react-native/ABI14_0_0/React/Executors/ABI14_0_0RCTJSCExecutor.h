/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <JavaScriptCore/JavaScriptCore.h>

#import <ReactABI14_0_0/ABI14_0_0RCTJavaScriptExecutor.h>

typedef void (^ABI14_0_0RCTJavaScriptValueCallback)(JSValue *result, NSError *error);

/**
 * Default name for the JS thread
 */
ABI14_0_0RCT_EXTERN NSString *const ABI14_0_0RCTJSCThreadName;

/**
 * This notification fires on the JS thread immediately after a `JSContext`
 * is fully initialized, but before the JS bundle has been loaded. The object
 * of this notification is the `JSContext`. Native modules should listen for
 * notification only if they need to install custom functionality into the
 * context. Note that this notification won't fire when debugging in Chrome.
 */
ABI14_0_0RCT_EXTERN NSString *const ABI14_0_0RCTJavaScriptContextCreatedNotification;

/**
 * A key to a reference to a JSContext class, held in the the current thread's
 *  dictionary. The reference would point to the JSContext class in the JS VM
 *  used in ReactABI14_0_0 (or ComponenetScript). It is recommended not to access it
 *  through the thread's dictionary, but rather to use the `FBJSCurrentContext()`
 *  accessor, which will return the current JSContext in the currently used VM.
 */
ABI14_0_0RCT_EXTERN NSString *const ABI14_0_0RCTFBJSContextClassKey;

/**
 * A key to a reference to a JSValue class, held in the the current thread's
 *  dictionary. The reference would point to the JSValue class in the JS VM
 *  used in ReactABI14_0_0 (or ComponenetScript). It is recommended not to access it
 *  through the thread's dictionary, but rather to use the `FBJSValue()` accessor.
 */
ABI14_0_0RCT_EXTERN NSString *const ABI14_0_0RCTFBJSValueClassKey;

/**
 * @experimental
 * May be used to pre-create the JSContext to make ABI14_0_0RCTJSCExecutor creation less costly.
 * Avoid using this; it's experimental and is not likely to be supported long-term.
 */
@interface ABI14_0_0RCTJSContextProvider : NSObject

- (instancetype)initWithUseCustomJSCLibrary:(BOOL)useCustomJSCLibrary
                                tryBytecode:(BOOL)tryBytecode;

/**
 * Marks whether the provider uses the custom implementation of JSC and not the system one.
 */
@property (nonatomic, readonly, assign) BOOL useCustomJSCLibrary;

/**
 * Marks whether it is safe to try and run bytecode if given the choice.
 */
@property (nonatomic, readonly) BOOL tryBytecode;

@end

/**
 * Uses a JavaScriptCore context as the execution engine.
 */
@interface ABI14_0_0RCTJSCExecutor : NSObject <ABI14_0_0RCTJavaScriptExecutor>

/**
 * Returns whether executor uses custom JSC library.
 * This value is used to initialize ABI14_0_0RCTJSCWrapper.
 * @default is NO.
 */
@property (nonatomic, readonly, assign) BOOL useCustomJSCLibrary;

/**
 * Returns the bytecode file format that the underlying runtime supports.
 */
@property (nonatomic, readonly) int32_t bytecodeFileFormatVersion;

/**
 * Specify a name for the JSContext used, which will be visible in debugging tools
 * @default is "ABI14_0_0RCTJSContext"
 */
@property (nonatomic, copy) NSString *contextName;

/**
 * Inits a new executor instance with given flag that's used
 * to initialize ABI14_0_0RCTJSCWrapper.
 */
- (instancetype)initWithUseCustomJSCLibrary:(BOOL)useCustomJSCLibrary;

/**
 * @experimental
 * Inits a new executor instance with given configuration flags. Please refer to
 * the documentation for `ABI14_0_0RCTJSContextProvider` for more information as to their
 * purpose.
 */
- (instancetype)initWithUseCustomJSCLibrary:(BOOL)useCustomJSCLibrary
                                tryBytecode:(BOOL)tryBytecode;

/**
 * @experimental
 * Pass a ABI14_0_0RCTJSContextProvider object to use an NSThread/JSContext pair that have already been created.
 * The underlying JSContext will be returned in the JSContext pointer if it is non-NULL.
 */
+ (instancetype)initializedExecutorWithContextProvider:(ABI14_0_0RCTJSContextProvider *)JSContextProvider
                                             JSContext:(JSContext **)JSContext;

/**
 * @experimental
 * synchronouslyExecuteApplicationScript:sourceURL:JSContext:error:
 *
 * Run the provided JS Script/Bundle, blocking the caller until it finishes.
 * If there is an error during execution, it is returned, otherwise `NULL` is
 * returned.
 */
- (NSError *)synchronouslyExecuteApplicationScript:(NSData *)script
                                         sourceURL:(NSURL *)sourceURL;

/**
 * Invokes the given module/method directly. The completion block will be called with the
 * JSValue returned by the JS context.
 *
 * Currently this does not flush the JS-to-native message queue.
 */
- (void)callFunctionOnModule:(NSString *)module
                      method:(NSString *)method
                   arguments:(NSArray *)args
             jsValueCallback:(ABI14_0_0RCTJavaScriptValueCallback)onComplete;

/**
 * Get the JavaScriptCore context associated with this executor instance.
 */
- (JSContext *)jsContext;

@end
