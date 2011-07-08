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

#include "Graph.h"

Graph::Graph(int max_nodes) :
	IGraph(max_nodes)
{
}

Graph::~Graph()
{
	QMap<int, Node * >::iterator it;
	while (!_nodes.isEmpty())
	{
		it = _nodes.begin();
		delete it.value();
		_nodes.remove(it.key());
	}
}

bool Graph::doCreateNode(int first, int last, int user, float coef)
{
	Node *node;
	int index;

	node = new Node(first, last, user, coef);
	index = getAvaibleIndex();
	
	_nodes.insert(index, node);

	_firsts_to_indexes[first].append(index);
	_lasts_to_indexes[last].append(index);

	addEdge(index, first, last);

	return true;
}

bool Graph::doReadNode(int first, int last, int user, Node *node)
{
	int index;
	
	index = getIndexOfNode(first, last, user);

	node = new Node(*(_nodes.value(index)));

	return true;
}

bool Graph::doUpdateNode(int old_first, int old_last, int old_user, float old_coef, int new_first, int new_last, int new_user, float new_coef)
{
	if (old_first == new_first && old_last == new_last)
	{
		if (old_user != new_user)
			return updateUser(old_first, old_last, old_user, new_user);
		
		if (old_coef != new_coef)
			return updateCoef(old_first, old_last, old_user, new_coef);
	}

	doDeleteNode(old_first, old_last, old_user);
	doCreateNode(new_first, new_last, new_user, new_coef);

	return true;
}

bool Graph::doDeleteNode(int first, int last, int user)
{
	int index;
	Node node;

	index = getIndexOfNode(first, last, user);

	// Remove node
	delete _nodes[index];
	_nodes.remove(index);

	// Reuse index
	_indexes.push(index);

	// Remove node to sons edges
	//_edges.remove(index);

	// Clean up _firsts_to_indexes
	_firsts_to_indexes[first].removeOne(index);
	if (_firsts_to_indexes[first].isEmpty())
		_firsts_to_indexes.remove(first);

	// Remove fathers to node edges
	/*
	QList<int>::iterator i;
    for (i = _lasts_to_indexes[first].begin(); i != _lasts_to_indexes[first].end(); ++i)
    {
        _edges[*i].removeOne(index);
        if (_edges[*i].empty())
            _edges.remove(*i);
    }
	*/

	// Clean up _lasts_to_indexes
	_lasts_to_indexes[last].removeOne(index);
	if (_lasts_to_indexes[last].isEmpty())
		_lasts_to_indexes.remove(index);
		
	return true;
}

Cycle * Graph::doCycleFinding(int first, int last, int user)
{
	const int source = getIndexOfNode(first, last, user);

	QMap<int, int> previous;
	QMap<int, float> dist;
	QVector<int> nodes = _nodes.keys().toVector();	

	foreach (int node, nodes)
	{
		previous.insert(node, -1);
		dist.insert(node, -1.0);
	}
	
	dist[source] = 1.0;

	int u;
	while (!nodes.empty() && previous.value(source) == -1)
	{
		u = nodes.value(0);
		foreach (int node, nodes)
			if (dist.value(node) > dist.value(u))
				u = node;
		if (dist.value(u) == -1)
			break;

		nodes.remove(nodes.indexOf(u));

		foreach (int v, _firsts_to_indexes.value(_nodes.value(u)->getLast()))
		{
			const float alt = dist[u] * _nodes.value(v)->getCoef();
			if (alt > dist.value(v) || v == source)
			{
				dist[v] = alt;
				previous[v] = u;
			}
		}
	}

	Cycle *cycle = new Cycle();
	u = source;
	int ubis;
	CycleStruct cs;
	Node *n;
	while (u != source || previous.value(source) != -1)
	{
		ubis = previous.value(u);
		previous[u] = -1;
		u = ubis;

		n = _nodes.value(u);
		cycle->prepend(InitCycleStruct(cs, n->getFirst(), n->getLast(), n->getUser()));
	}

	return cycle;
}

bool Graph::nodeExist(int first, int last, int user)
{
	QMap<int, Node * >::const_iterator i;
	Node node(first, last, user, 0);

	for (i = _nodes.constBegin(); i != _nodes.constEnd(); ++i)
	{
		if (*(i.value()) == node)
			return true;
	}

	return false;
}

int Graph::size()
{
	return _nodes.size();
}

int Graph::getAvaibleIndex()
{
	if (_nodes.isEmpty())
		return 0;
	else if (!_indexes.isEmpty())
		return _indexes.pop();
	else
		return _nodes.keys().last() + 1;
}

void Graph::addEdge(int node_index, int first, int last)
{
    // Parcours de l'index pour savoir à quels noueds relier notre noeud
    // Dans le sens notre noeud -> les autres noeuds
    if (_firsts_to_indexes.contains(last))
    {
        QListIterator<int> i = QListIterator<int>(_firsts_to_indexes[last]);
        while (i.hasNext())
        {
            addEdge(node_index, i.next());
        }
    }

    // Parcours de l'index pour savoir à quels noueds relier notre noeud
    // Dans le sens les noeuds existants -> notre noeud
    if (_lasts_to_indexes.contains(first))
    {
        QListIterator<int> i = QListIterator<int>(_lasts_to_indexes[first]);
        while (i.hasNext())
        {
            addEdge(i.next(), node_index);
        }
    }
}

void Graph::addEdge(int father_index, int son_index)
{
	/*
	if (!_edges.contains(father_index)
			|| !_edges.value(father_index).contains(son_index))
		_edges[father_index].append(son_index);
	*/
}

int Graph::getIndexOfNode(int first, int last, int user)
{
	QMap<int, Node * >::const_iterator i;
	Node node(first, last, user, 0);

	for (i = _nodes.constBegin(); i != _nodes.constEnd(); ++i)
	{
		if (*(i.value()) == node)
			return i.key();
	}
}

Node * Graph::getNode(int first, int last, int user)
{
	return _nodes[getIndexOfNode(first, last, user)];
}

bool Graph::updateUser(int first, int last, int old_user, int new_user)
{
	getNode(first, last, old_user)->modifyUser(new_user);

	return true;
}

bool Graph::updateCoef(int first, int last, int user, float new_coef)
{
	getNode(first, last, user)->modifyCoef(new_coef);

	return true;
}