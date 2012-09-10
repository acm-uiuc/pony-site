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

#include <stdio.h>

typedef struct SplayNode
{
    struct SplayNode *left, *right, *parent, *leftmin;
    void *obj;
}
SNode;

typedef struct SplayTree
{
    int (*compare) ();
    SNode *root;
}
STree;

STree *CreateSplay(int (*compare) ());
SNode *SplayCreateNode();
void SplayDelete(STree *, void *);
void SplayInsert(STree *, void *);
void SplayJoin(STree *, STree *);
void SplaySplit(STree *, STree *, void *);
void *SplayAccess(STree *, void *);
void SplayNode(STree *, SNode *);
void S_splayr(STree *, SNode *);
void S_splayl(STree *, SNode *);
void S_splayrl(STree *, SNode *);
void S_splayrr(STree *, SNode *);
void S_splayll(STree *, SNode *);
void S_splaylr(STree *, SNode *);
STree *SplayCopyTree(STree *, void (*CopyNode) ());
void SplayTraverse(STree *, void (*function) ());
SNode *SplayMinOfSubTree(SNode *);
void SplayCopySubTree(SNode *, SNode *, void (*CopyNode) ());
void SplayInOrderTraverse(SNode * sn, void (*function) ());
void SplayDeleteTree(STree *);
void SplayDeleteSubTree(SNode *);
void SplayDestroyTree(STree *);
void SplayDestroySubTree(SNode *);
void SplayFreeNode(SNode *);



#define SNODE_MALLOC_SIZE 100


static SNode *SplayNodeFreeList = NULL;


void
 SplayFreeNode(SNode * sn)
{
    if (sn)
    {
	sn->right = SplayNodeFreeList;
#ifdef DEBUG
	sn->left = NULL;
	sn->parent = NULL;
	sn->obj = NULL;
	sn->leftmin = NULL;
#endif
	SplayNodeFreeList = sn;
    }
}



STree *
 CreateSplay(int (*compare) ())
{
    STree *stree;
    stree = (STree *) malloc(sizeof(STree));
    stree->compare = compare;
    stree->root = NULL;
    return stree;
}

SNode *
 SplayCreateNode()
{
    SNode *tmp;

    if (SplayNodeFreeList)
    {
	tmp = SplayNodeFreeList;
	SplayNodeFreeList = tmp->right;
    }
    else
    {
	int i;
	tmp = (SNode *) malloc(sizeof(SNode) * SNODE_MALLOC_SIZE);
	tmp += (SNODE_MALLOC_SIZE - 1);
	tmp->right = SplayNodeFreeList;
	tmp--;
	for (i = SNODE_MALLOC_SIZE - 2; i; i--, tmp--)
	    tmp->right = (tmp + 1);
	SplayNodeFreeList = tmp + 1;
    }
    tmp->leftmin = tmp;
    tmp->obj = tmp->left = tmp->right = tmp->parent = NULL;
    return tmp;

}

void *
 SplayAccess(STree * stree, void *obj)
{
    SNode *snode;
    int res, done = 0;
    int (*compare) ();


    compare = stree->compare;
    snode = stree->root;
    if (snode == NULL)
	return NULL;
    if ((compare) (obj, snode->obj) == 0)
	return snode->obj;

/*  if(snode->left == NULL && snode->right == NULL) return NULL; */
    if (snode->left == NULL && snode->right == NULL)
	return snode->obj;

    while (!done)
    {
	res = (compare) (obj, snode->obj);
	if (res == 0)
	    done = 2;
	else if (res > 0)
	    if (snode->right != NULL)
	    {
		snode = snode->right;
	    }
	    else
		done = 1;
	else if (snode->left != NULL)
	{
	    snode = snode->left;
	}
	else
	    done = 1;
    }
    SplayNode(stree, snode);
    if (done == 2)
	return snode->obj;
    else
	/*  return NULL; */
	return snode->obj;

}

void
 SplayNode(STree * stree, SNode * snode)
{
    SNode *par, *gpar, *ll, *lr, *l, *r, *rr, *rl;




    while ((par = snode->parent) != NULL)
    {
	if (par->parent != NULL)
	{
	    gpar = par->parent;
	    ll = lr = rr = rl = NULL;
	    l = gpar->left;
	    r = gpar->right;
	    if (l != NULL)
	    {
		ll = l->left;
		lr = l->right;
	    }
	    if (r != NULL)
	    {
		rl = r->left;
		rr = r->right;
	    }
	    if (snode == ll)
		S_splayll(stree, snode);
	    else if (snode == lr)
		S_splaylr(stree, snode);
	    else if (snode == rr)
		S_splayrr(stree, snode);
	    else if (snode == rl)
		S_splayrl(stree, snode);
	}
	else
	{
	    l = par->left;
	    r = par->right;
	    if (snode == l)
		S_splayl(stree, snode);
	    else if (snode == r)
		S_splayr(stree, snode);
	    else
		fprintf(stderr, "really fucked. go home and die.\n");
	}
    }

}

void
 S_splayl(STree * stree, SNode * snode)
{
    stree->root->left = snode->right;
    stree->root->parent = snode;
    if (snode->right != NULL)
    {
	stree->root->leftmin = snode->right->leftmin;
	snode->right->parent = stree->root;
    }
    else
    {
	stree->root->leftmin = stree->root;
    }
    snode->right = stree->root;
    stree->root = snode;
    snode->parent = NULL;

}

void
 S_splayr(STree * stree, SNode * snode)
{
    stree->root->right = snode->left;
    stree->root->parent = snode;
    if (snode->left != NULL)
	snode->left->parent = stree->root;
    snode->left = stree->root;
    stree->root = snode;
    snode->parent = NULL;
    snode->leftmin = snode->left->leftmin;
}

void
 S_splayll(STree * stree, SNode * snode)
{
    SNode *ggpar, *gpar, *par;

    par = snode->parent;
    gpar = par->parent;

    /* Do the min left operations */
    if (par->right != NULL)
	gpar->leftmin = par->right->leftmin;
    else
	gpar->leftmin = gpar;
    if (snode->right != NULL)
	par->leftmin = snode->right->leftmin;
    else
	par->leftmin = par;

    /* Do the splay rotations */
    ggpar = gpar->parent;
    gpar->left = par->right;
    if (gpar->left != NULL)
	gpar->left->parent = gpar;
    gpar->parent = par;
    par->right = gpar;
    par->left = snode->right;
    if (par->left != NULL)
	par->left->parent = par;
    par->parent = snode;
    snode->right = par;
    snode->parent = ggpar;
    if (ggpar != NULL)
    {
	if (ggpar->right == gpar)
	    ggpar->right = snode;
	else
	    ggpar->left = snode;
    }
    else
    {
	stree->root = snode;
    }
}


void
 S_splayrr(STree * stree, SNode * snode)
{
    SNode *ggpar, *gpar, *par;

    par = snode->parent;
    gpar = par->parent;
    ggpar = gpar->parent;
    /* This min tracking is easy--gpar has min of all x, y, z */
    par->leftmin = snode->leftmin = gpar->leftmin;

    /* Do rotations */
    gpar->right = par->left;
    if (gpar->right != NULL)
	gpar->right->parent = gpar;
    gpar->parent = par;
    par->left = gpar;
    par->right = snode->left;
    if (par->right != NULL)
	par->right->parent = par;
    par->parent = snode;
    snode->left = par;
    snode->parent = ggpar;
    if (ggpar != NULL)
    {
	if (ggpar->right == gpar)
	    ggpar->right = snode;
	else
	    ggpar->left = snode;
    }
    else
    {
	stree->root = snode;
    }
}


void
 S_splaylr(STree * stree, SNode * snode)
{
    SNode *ggpar, *gpar, *par;

    par = snode->parent;
    gpar = par->parent;

    /* Do the min calcs */
    if (snode->right != NULL)
	gpar->leftmin = snode->right->leftmin;
    else
	gpar->leftmin = gpar;
    snode->leftmin = par->leftmin;

    /* Do the rotations */
    ggpar = gpar->parent;
    par->right = snode->left;
    if (par->right != NULL)
	par->right->parent = par;
    par->parent = snode;
    gpar->left = snode->right;
    if (gpar->left != NULL)
	gpar->left->parent = gpar;
    gpar->parent = snode;
    snode->left = par;
    snode->right = gpar;
    snode->parent = ggpar;
    if (ggpar != NULL)
    {
	if (ggpar->right == gpar)
	    ggpar->right = snode;
	else
	    ggpar->left = snode;
    }
    else
    {
	stree->root = snode;
    }
}

void
 S_splayrl(STree * stree, SNode * snode)
{
    SNode *ggpar, *gpar, *par;

    par = snode->parent;
    gpar = par->parent;

    /* Do the min swaps */
    snode->leftmin = gpar->leftmin;
    if (snode->right != NULL)
	par->leftmin = snode->right->leftmin;
    else
	par->leftmin = par;

    /* do the rotations */
    ggpar = gpar->parent;
    gpar->right = snode->left;
    if (gpar->right != NULL)
	gpar->right->parent = gpar;
    gpar->parent = snode;
    par->left = snode->right;
    if (par->left != NULL)
	par->left->parent = par;
    par->parent = snode;
    snode->left = gpar;
    snode->right = par;
    snode->parent = ggpar;
    if (ggpar != NULL)
	if (ggpar->right == gpar)
	    ggpar->right = snode;
	else
	    ggpar->left = snode;
    else
	stree->root = snode;
}


void
 SplaySplit(STree * stree, STree * stree2, void *obj)
{
    SplayAccess(stree, obj);
    if ((stree->compare) (stree->root->obj, obj) > 0)
    {
	stree2->root = stree->root;
	stree->root = stree->root->left;
	if (stree->root)
	{
	    stree->root->parent = NULL;
	}
    }
    else
    {
	stree2->root = stree->root->right;
	if (stree2->root != NULL)
	{
	    stree2->root->parent = NULL;
	}
	stree->root->right = NULL;
    }
}

void
 SplayJoin(STree * stree1, STree * stree2)
{
    SplayNode(stree2, stree2->root->leftmin);
    stree2->root->left = stree1->root;
    stree1->root->parent = stree2->root;
    stree1->root = stree2->root;
    stree1->root->leftmin = stree1->root->left->leftmin;
    free(stree2);
}

void
 SplayInsert(STree * stree, void *obj)
{
    int (*compare) ();
    SNode *snode;

    snode = SplayCreateNode();
    snode->obj = obj;

    compare = stree->compare;
    if (stree->root == NULL)
    {
	stree->root = snode;
	return;
    }
    SplayAccess(stree, obj);
    if ((compare) (stree->root->obj, obj) < 0)
    {
	snode->left = stree->root;
	if (snode->left != NULL)
	{
	    snode->leftmin = snode->left->leftmin;
	    snode->left->parent = snode;
	}
	else
	{
	    snode->leftmin = snode;
	}
	snode->right = stree->root->right;
	if (snode->right != NULL)
	    snode->right->parent = snode;
	stree->root->right = NULL;
	stree->root = snode;
	snode->parent = NULL;
    }
    else
    {
	snode->right = stree->root;
	if (snode->right != NULL)
	{
	    snode->right->parent = snode;
	    snode->right->leftmin = snode->right;
	}
	snode->left = stree->root->left;
	if (snode->left != NULL)
	{
	    snode->leftmin = snode->left->leftmin;
	    snode->left->parent = snode;
	}
	else
	{
	    snode->leftmin = snode;
	}
	stree->root->left = NULL;
	stree->root = snode;
	snode->parent = NULL;
    }
}


void
 SplayDelete(STree * stree, void *obj)
{
    STree stree2;
    SNode *tmp;

    if (stree)
    {
	if (stree->root == NULL)
	    return;
    }
    else
    {
	return;
    }

    if ((stree->compare) (SplayAccess(stree, obj), obj))
	return;

    if (stree->root->right)
    {
	stree2.root = stree->root->right;
	stree2.root->parent = NULL;
	stree2.compare = stree->compare;
	SplayNode(&stree2, stree2.root->leftmin);
	stree2.root->left = stree->root->left;
	if (stree->root->left)
	{
	    stree->root->left->parent = stree2.root;
	    stree2.root->leftmin = stree->root->leftmin;
	}
	else
	{
	    stree2.root->leftmin = stree2.root;
	}
	tmp = stree->root;
	stree->root = stree2.root;
    }
    else
    {
	tmp = stree->root;
	stree->root = stree->root->left;
	if (stree->root)
	    stree->root->parent = NULL;
    }

    SplayFreeNode(tmp);

}

void
 CopySubTree(SNode * snode, SNode * snode2, void (*CopyNode) ())
{
    SNode *new;

    CopyNode(snode, snode2);
    if (snode->left != NULL)
    {
	snode2->left = new = SplayCreateNode();
	new->parent = snode2;
	CopySubTree(snode->left, new, CopyNode);
	snode2->leftmin = snode2->left->leftmin;
    }
    else
    {
	snode2->leftmin = snode;
    }

    if (snode->right != NULL)
    {
	snode2->right = new = SplayCreateNode();
	new->parent = snode2;
	CopySubTree(snode->right, new, CopyNode);
    }
}


STree *
 SplayCopyTree(STree * st, void (*CopyNode) ())
{
    STree *st2;

    st2 = CreateSplay(st->compare);
    if (st->root != NULL)
    {
	st2->root = SplayCreateNode();
	CopySubTree(st->root, st2->root, CopyNode);
    }
    return st2;
}



void
 SplayTraverse(STree * st, void (*function) ())
{
    if (st->root != NULL)
	SplayInOrderTraverse(st->root, function);
}

void
 SplayInOrderTraverse(SNode * sn, void (*function) ())
{
    if (sn->left != NULL)
	SplayInOrderTraverse(sn->left, function);
    (function) (sn->obj);
    if (sn->right != NULL)
	SplayInOrderTraverse(sn->right, function);
}

SNode *
 MinOfSubTree(SNode * sn)
{
    return sn->leftmin;
}

SNode *
 MinOfTree(STree * st)
{
    return st->root->leftmin;
}


void
 SplayDeleteTree(STree * st)
{
    SNode *sn;

    if ((sn = st->root) != NULL)
	SplayDeleteSubTree(sn);
    free(st);

}

void
 SplayDeleteSubTree(SNode * sn)
{

    if (sn->right != NULL)
	SplayDeleteSubTree(sn->right);
    if (sn->left != NULL)
	SplayDeleteSubTree(sn->left);
    SplayFreeNode(sn);

}

void
 SplayDestroyTree(STree * st)
{
    SNode *sn;

    if ((sn = st->root) != NULL)
	SplayDestroySubTree(sn);
    free(st);

}

void
 SplayDestroySubTree(SNode * sn)
{

    if (sn->right != NULL)
	SplayDestroySubTree(sn->right);
    if (sn->left != NULL)
	SplayDestroySubTree(sn->left);
    free(sn->obj);
    SplayFreeNode(sn);

}
