/*
 **     Copyright (C) 1995  Rick Romero
 **
 **     This program is free software; you can redistribute it and/or modify
 **     it under the terms of the GNU General Public License as published by
 **     the Free Software Foundation; either version 1, or (at your option)
 **     any later version.
 **     
 **     This program is distributed in the hope that it will be useful,
 **     but WITHOUT ANY WARRANTY; without even the implied warranty of
 **     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **     GNU General Public License for more details.
 **     
 **     You should have received a copy of the GNU General Public License
 **     along with this program; if not, write to the Free Software
 **     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SPLAYTREE_H_
#define _SPLAYTREE_H_

typedef struct SplayNode
{
    struct SplayNode *left, *right, *parent, *leftmin;
    void *obj;
}
SplayNode_t, *SplayNode_p;

typedef struct SplayTree
{
    int (*compare) ();
    SplayNode_p root;
}
SplayTree_t, *SplayTree_p;

SplayTree_p CreateSplay(int (*compare) ());
SplayNode_p SplayCreateNode();
void SplayFreeNode(SplayNode_p);
void SplayDelete(SplayTree_p, void *);
void SplayInsert(SplayTree_p, void *);
void SplayJoin(SplayTree_p, SplayTree_p);
void SplaySplit(SplayTree_p, SplayTree_p, void *);
void *SplayAccess(SplayTree_p, void *);
void SplayNode(SplayTree_p, SplayNode_p);
void S_splayr(SplayTree_p, SplayNode_p);
void S_splayl(SplayTree_p, SplayNode_p);
void S_splayrl(SplayTree_p, SplayNode_p);
void S_splayrr(SplayTree_p, SplayNode_p);
void S_splayll(SplayTree_p, SplayNode_p);
void S_splaylr(SplayTree_p, SplayNode_p);
SplayTree_p SplayCopyTree(SplayTree_p, void (*CopyNode) ());
void SplayTraverse(SplayTree_p, void (*function) ());
SplayNode_p SplayMinOfSubTree(SplayNode_p);
void SplayCopySubTree(SplayNode_p, SplayNode_p, void (*CopyNode) ());
void SplayInOrderTraverse(SplayNode_p, void (*function) ());
void SplayDeleteTree(SplayTree_p);
void SplayDeleteSubTree(SplayNode_p);
void SplayDestroyTree(SplayTree_p);
void SplayDestroySubTree(SplayNode_p);



#endif
