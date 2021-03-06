/*

ResourceManager.h

Singleton class responsible for loading various data files.

Oolite
Copyright (C) 2004-2013 Giles C Williams and contributors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.

*/

#import "OOCocoa.h"
#import "OOOpenGL.h"
#import "NSFileManagerOOExtensions.h"

@class OOSound, OOMusic, OOSystemDescriptionManager;


typedef enum
{
	MERGE_NONE,		// Just use the last file in search order.
	MERGE_BASIC,	// Merge files by adding the top-level items of each file.
	MERGE_SMART		// Merge files by merging the top-level elements of each file (second-order merge, but not recursive)
} OOResourceMergeMode;

/* 'All' doesn't quite mean 'all' - OXPs with the tag
 * "oolite-scenario-only" will only be loaded if required by a
 * scenario.
 *
 * Note that this means that the scenario itself must be in a
 * different OXP, or it'll never be loaded when on the start-game
 * screen.
 */
#define SCENARIO_OXP_DEFINITION_ALL    @""
#define SCENARIO_OXP_DEFINITION_NONE   @"strict"
#define SCENARIO_OXP_DEFINITION_BYID   @"id:"
#define SCENARIO_OXP_DEFINITION_BYTAG  @"tag:"
#define SCENARIO_OXP_DEFINITION_NOPLIST  @"exc:"

@interface ResourceManager : NSObject

+ (void) reset;
+ (void) resetManifestKnowledgeForOXZManager;


+ (NSArray *)rootPaths;			// Places add-ons are searched for, not including add-on paths.
+ (NSArray *)userRootPaths;		// Places users are expected to place add-ons, not including built-in data or managed add-ons directory.
+ (NSString *)builtInPath;		// Path for built-in data only.
+ (NSArray *)pathsWithAddOns;	// Root paths + add-on paths.
+ (NSArray *)paths;				// builtInPath or pathsWithAddOns, depending on useAddOns state.
+ (NSString *)useAddOns;
+ (NSArray *)OXPsWithMessagesFound;
+ (void)setUseAddOns:(NSString *)useAddOns;
+ (void)addExternalPath:(NSString *)fileName;
+ (NSEnumerator *)pathEnumerator;
+ (NSEnumerator *)reversePathEnumerator;

// get manifest data for identifier
+ (NSDictionary *)manifestForIdentifier:(NSString *)identifier;
// compatibility checks
+ (BOOL) checkVersionCompatibility:(NSDictionary *)manifest forOXP:(NSString *)title;
+ (BOOL) manifestHasConflicts:(NSDictionary *)manifest logErrors:(BOOL)logErrors;
+ (BOOL) manifestHasMissingDependencies:(NSDictionary *)manifest logErrors:(BOOL)logErrors;
+ (BOOL) manifest:(NSDictionary *)manifest HasUnmetDependency:(NSDictionary *)required logErrors:(BOOL)logErrors;
+ (BOOL) matchVersions:(NSDictionary *)rangeDict withVersion:(NSString *)version;



+ (void)handleEquipmentListMerging: (NSMutableArray *)arrayToProcess forLookupIndex:(unsigned)lookupIndex;

+ (NSString *)errors;			// Errors which occured during path scanning - essentially a list of OXPs whose requires.plist is bad.

+ (NSString *) pathForFileNamed:(NSString *)fileName inFolder:(NSString *)folderName;
+ (NSString *) pathForFileNamed:(NSString *)fileName inFolder:(NSString *)folderName cache:(BOOL)useCache;

+ (BOOL) corePlist:(NSString *)fileName excludedAt:(NSString *)path;

+ (NSDictionary *)dictionaryFromFilesNamed:(NSString *)fileName
								  inFolder:(NSString *)folderName
								  andMerge:(BOOL) mergeFiles;
+ (NSDictionary *)dictionaryFromFilesNamed:(NSString *)fileName
								  inFolder:(NSString *)folderName
								 mergeMode:(OOResourceMergeMode)mergeMode
									 cache:(BOOL)useCache;

+ (NSArray *)arrayFromFilesNamed:(NSString *)fileName
						inFolder:(NSString *)folderName
						andMerge:(BOOL) mergeFiles;
+ (NSArray *)arrayFromFilesNamed:(NSString *)fileName
						inFolder:(NSString *)folderName
						andMerge:(BOOL) mergeFiles
						   cache:(BOOL)useCache;

// These are deliberately not merged like normal plists for security reasons.
+ (NSDictionary *) whitelistDictionary;
+ (NSDictionary *) shaderBindingTypesDictionary;

// These have special merging rules.
+ (NSDictionary *) logControlDictionary;
+ (NSDictionary *) roleCategoriesDictionary;
+ (OOSystemDescriptionManager *) systemDescriptionManager;

+ (OOSound *)ooSoundNamed:(NSString *)fileName inFolder:(NSString *)folderName;
+ (OOMusic *)ooMusicNamed:(NSString *)fileName inFolder:(NSString *)folderName;

+ (NSString *) stringFromFilesNamed:(NSString *)fileName inFolder:(NSString *)folderName;
+ (NSString *) stringFromFilesNamed:(NSString *)fileName inFolder:(NSString *)folderName cache:(BOOL)useCache;

+ (NSDictionary *)loadScripts;

/*	+writeDiagnosticData:toFileNamed:
	+writeDiagnosticString:toFileNamed:
	+writeDiagnosticPList:toFileNamed:
	
	Write data to the specified path within the log directory. Slashes may be
	used as path separators in name.
 */
+ (BOOL) writeDiagnosticData:(NSData *)data toFileNamed:(NSString *)name;
+ (BOOL) writeDiagnosticString:(NSString *)string toFileNamed:(NSString *)name;
+ (BOOL) writeDiagnosticPList:(id)plist toFileNamed:(NSString *)name;

+ (NSString *) diagnosticFileLocation;

+ (NSDictionary *) materialDefaults;

// Clear ResourceManager-internal caches (not those handled by OOCacheManager)
+ (void) clearCaches;

@end
