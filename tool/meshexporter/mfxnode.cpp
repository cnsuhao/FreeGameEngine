/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#include "pch.hpp"
#include "mfxexp.hpp"

#include "mfxnode.hpp"
#include "utility.hpp"

#ifndef CODE_INLINE
#include "mfxnode.ipp"
#endif

MFXNode::MFXNode()
: node_( NULL ),
  parent_( NULL ),
  contentFlag_( false )

{
	transform_.IdentityMatrix();
}

MFXNode::MFXNode( INode *node )
: node_( node ),
  parent_( NULL ),
  contentFlag_( false ),
  include_( false )
{
	transform_.IdentityMatrix();
}

Matrix3 MFXNode::getTransform( TimeValue t, bool normalise )
{
	// Is there a max node attached to this mfxnode, if there is, return its world transform
	if( node_ )
	{
		if( normalise )
			return normaliseMatrix( node_->GetNodeTM( t ) );
		else
			return node_->GetNodeTM( t );
	}

	// do we have a parent, if so return the parents worldtransform
	if( parent_ )
	{
		if( normalise )
			return  normaliseMatrix( parent_->getTransform( t ) * transform_ );
		else
			return  parent_->getTransform( t ) * transform_;
	}

	// return the stored transform
	if( normalise )
		return normaliseMatrix( transform_ );

	return transform_;
}

void MFXNode::includeAncestors()
{
	MFXNode* parent = parent_;
	while (parent)
	{
		parent->include(true);
		parent = parent->getParent();
	}
}

Matrix3 MFXNode::getRelativeTransform( TimeValue t, bool normalise,
	MFXNode * idealParent )
{
	// Does this mfx node have a max node?
	if( node_ )
	{
		// Do we want to override the parent?
		if (parent_ && !idealParent) idealParent = parent_;

		// If there is a parent get the relative transform, from the parent, else
		// return this nodes transform.
		if( idealParent )
		{
			return getTransform( t, normalise ) *
				Inverse( idealParent->getTransform( t, normalise ) );
		}
		else
		{
			return getTransform( t, normalise );
		}
	}

	if( normalise )
		return normaliseMatrix( transform_ );

	return transform_;
}

int MFXNode::treeSize( void )
{
	int nNodes = 1;

	for( int i = 0; i < children_.size(); i++ )
	{
		nNodes += children_[ i ]->treeSize();
	}

	return nNodes;	
}

void MFXNode::removeChild( int n )
{
	if( n < children_.size() )
	{
		removeChild( children_[ n ] );
	}
}

void MFXNode::removeChild( MFXNode *n )
{
	std::vector< MFXNode * >::iterator it = std::find( children_.begin(), children_.end(), n ); ;

	if( it != children_.end() )
	{
		n->parent_ = NULL;
		children_.erase( it );
	}
}

/*
 * Removes a child node and adds all it's children to this node.
 */
void MFXNode::removeChildAddChildren( MFXNode *n )
{
	std::vector< MFXNode * >::iterator it = std::find( children_.begin(), children_.end(), n ); ;

	if( it != children_.end() )
	{
		n->parent_ = NULL;
		children_.erase( it );

		while( n->getNChildren() )
		{
			MFXNode *child = n->getChild( 0 );
			n->removeChild( child );
			addChild( child );
		}
	}
}

void MFXNode::removeChildAddChildren( int n )
{
//	MF_ASSERT( children_.size() > n );
	removeChildAddChildren( children_[ n ] );
}

void MFXNode::delChildren()
{
	children_.clear();
}


MFXNode::~MFXNode()
{
}

std::ostream& operator<<(std::ostream& o, const MFXNode& t)
{
	o << "MFXNode\n";
	return o;
}

// mfxnode.cpp
