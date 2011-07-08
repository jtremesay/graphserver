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

#include <QMap>
#include <QString>

#include "imanager.h"
#include "GraphFactory.h"

class Manager :
	public IManager
{
public:
	Manager(int max_graphs);
	~Manager(void);

private:
	bool doCreateGraph(QString graph, int max_nodes);
	bool doDeleteGraph(QString graph);
	bool doCreateNode(QString graph, int first, int last, int user, float coef);
	bool doUpdateNode(QString graph,
			int old_first, int old_last, int old_user, float old_coef,
			int new_first, int new_last, int new_user, float new_coef);
	bool doDeleteNode(QString graph, int first, int last, int user);
	Cycle * doCycleFinding(QString graph, int first, int last, int user);
	bool graphExist(QString graph);
	int size();

private:
	QMap<QString, IGraph * > _graphs;
};
