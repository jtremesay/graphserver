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
 
#pragma once

#include "Cycle.h"
#include "Node.h"

class IGraph
{
public:
	IGraph(int max_nodes=-1);
	virtual ~IGraph(void);

	bool createNode(int first, int last, int user, float coef);
	bool readNode(int first, int last, int user, Node *node);
	bool updateNode(int old_first, int old_last, int old_user, float old_coef,
			int new_first, int new_last, int new_user, float new_coef);
	bool deleteNode(int first, int last, int user);
	Cycle * cycleFinding(int first, int last, int user);

private:
	virtual bool doCreateNode(int first, int last, int user, float coef) = 0;
	virtual bool doReadNode(int first, int last, int user, Node *node) = 0;
	virtual bool doUpdateNode(int old_first, int old_last, int old_user, float old_coef,
			int new_first, int new_last, int new_user, float new_coef) = 0;
	virtual bool doDeleteNode(int first, int last, int user) = 0;
	virtual Cycle * doCycleFinding(int first, int last, int user) = 0;
	virtual bool nodeExist(int first, int last, int user) = 0;
	virtual int size() = 0;

private:
	int _max_nodes;
};
