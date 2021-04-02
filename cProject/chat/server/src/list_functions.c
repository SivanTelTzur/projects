#include "list_functions.h"
#include <stdio.h>


static void SwapData(ListItr _a, ListItr _b);
static ListItr InsertDest(ListItr _dest, ListItr _fromItr);

/***********************************************************************/

ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	if (_begin == NULL || _end == NULL || _predicate == NULL)
	{
		return _end;
	}
	
	while (_begin != _end)
	{
		if (_predicate(ListItr_Get(_begin), _context))
		{
			break;
		}
		_begin = ListItr_Next(_begin);
	}
	return _begin;
}

size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	size_t count = 0;
	if (_begin == NULL || _end == NULL || _predicate == NULL)
	{
		return 0;
	}
	
	while (_begin != _end)
	{
		if (_predicate(ListItr_Get(_begin), _context))
		{
			++count;
		}
		_begin = ListItr_Next(_begin);
	}
	return count;
	
}



ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	if (_begin == NULL || _end == NULL || _action == NULL )
	{
		return _end;
	}
	
	while (_begin != _end)
	{
		if (!(_action(ListItr_Get(_begin), _context)))
		{
			break;
		}
		_begin = ListItr_Next(_begin);
	}

	return _begin;
}


void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less)
{
	ListItr curr;
	int flag = 0;
	
	if (_begin == NULL || _end == NULL || _less == NULL)
	{
		return;
	}
	
	while (_begin != ListItr_Prev(_end))
	{
		curr = _begin;
		while (curr != ListItr_Prev(_end))
		{
			if (_less(ListItr_Get(curr), ListItr_Get(ListItr_Next(curr))))
			{
				SwapData(curr, ListItr_Next(curr));
				flag = 1;
			}
			curr = ListItr_Next(curr);
		}
		if (flag)
		{
			break;
		}
		_end = ListItr_Prev(_end);
	}

}

ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end)
{
	if (_begin == NULL || _end == NULL)
	{
		return _end;
	}
	
	while (_begin != _end)
	{
		_begin = ListItr_Next(_begin);				
		if (ListItr_InsertBefore(_dest, ListItr_Remove(ListItr_Prev(_begin))) == NULL)
		{
			return _end;
		} 		
	}
	return _begin;
}





ListItr ListItr_Merge(ListItr _destBegin, ListItr _firstBegin, ListItr _firstEnd,
			ListItr _secondBegin, ListItr _secondEnd, LessFunction _less)
{

	if (_destBegin == NULL || _firstBegin == NULL || _firstEnd == NULL || _secondBegin == NULL || _secondEnd == NULL || _less == NULL)
	{
		return _destBegin;
	}
	
	while (_firstBegin != _firstEnd && _secondBegin != _secondEnd)
	{
		if (_less(ListItr_Get(_firstBegin), ListItr_Get(_secondBegin)))
		{
			_firstBegin = InsertDest(_destBegin, _firstBegin);
		}
		else
		{
			_secondBegin = InsertDest(_destBegin, _secondBegin);
		}	
	}
	
	while (_firstBegin != _firstEnd)
	{
		_firstBegin = InsertDest(_destBegin, _firstBegin);
	}
	
	while(_secondBegin != _secondEnd)
	{
		_secondBegin = InsertDest(_destBegin, _secondBegin);	
	}

	return _destBegin;
}

/************************************************************************************************/
static ListItr InsertDest(ListItr _dest, ListItr _fromItr)
{
	ListItr_InsertBefore(_dest, ListItr_Get(_fromItr));
	_fromItr = ListItr_Next(_fromItr);
	ListItr_Remove(ListItr_Prev(_fromItr));
	return _fromItr;
}


static void SwapData(ListItr _a, ListItr _b)
{
	void* tempValue;
	
	tempValue = ListItr_Set(_a, ListItr_Get(_b));
	ListItr_Set(_b, tempValue);
}
	

