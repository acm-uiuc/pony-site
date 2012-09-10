//------------------------------------------------------------------------------
//  <copyright from='2004' to='2005' company='WoWSharp.NET'>
//    Copyright (c) WoWSharp.NET. All Rights Reserved.
//
//    Please look in the accompanying license.htm file for the license that 
//    applies to this source code. (a copy can also be found at: 
//    http://www.wowsharp.net/license.htm)
//  </copyright>
//-------------------------------------------------------------------------------

#ifndef _PEB_HEADER
#define _PEB_HEADER

#define PEB_ADDRESS						0x7ffdf000
#define PEB_LDR_DATA_OFFSET				0x0C    // RVA to _PEB_LDR_DATA (ProcessModuleInfo)

#define LDR_DATA_IMAGE_BASE				0x18	// MODULE_ITEM.ImageBase
#define LDR_DATA_IMAGE_SIZE				0x20	// MODULE_ITEM.ImageSize
#define LDR_DATA_PATHFILENAME_OFFSET	0x24	// MODULE_ITEM.PathFileName
#define LDR_DATA_FILENAME_OFFSET		0x2C	// MODULE_ITEM.FileName



// Winnt.h :
/*typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;
*/


#endif
