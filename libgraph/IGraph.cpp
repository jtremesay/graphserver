/**
 * Copyright 2010,
 *     Jonathan Tremesaygues <killruana@gmaiil.com>
 *     Damien Cheneaux <ikio.dv@mail.com>
 * 
 * This software is a computer program whose purpose is to [describe
 * functionalities and technical features of your software].
 * 
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
 
#include "IGraph.h"

IGraph::IGraph(int max_nodes) :
	_max_nodes(max_nodes)
{
}

IGraph::~IGraph(void)
{
}

bool IGraph::createNode(int first, int last, int user, float coef)
{
	if (size() == _max_nodes)
		return false;

	return doCreateNode(first, last, user, coef);
}

bool IGraph::readNode(int first, int last, int user, Node *node)
{
	if (size() == 0)
		return false;

	if (!nodeExist(first, last, user))
		return false;

	return doReadNode(first, last, user, node);
}

bool IGraph::updateNode(int old_first, int old_last, int old_user, float old_coef, int new_first, int new_last, int new_user, float new_coef)
{
	if (size() == 0)
		return false;
		
	if (!nodeExist(old_first, old_last, old_user))
		return false;

	return doUpdateNode(old_first, old_last, old_user, old_coef, new_first, new_last, new_user, new_coef);
}

bool IGraph::deleteNode(int first, int last, int user)
{
	if (size() <= 0)
		return false;
	
	if (!nodeExist(first, last, user))
		return false;

	return doDeleteNode(first, last, user);
}

Cycle * IGraph::cycleFinding(int first, int last, int user)
{
	if (size() == 0)
		return 0;
	
	if (!nodeExist(first, last, user))
		return 0;

	return doCycleFinding(first, last, user);
}